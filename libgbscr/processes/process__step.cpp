#include"libgbscr/processes/process_private.hpp"


namespace     gbscr{
namespace processes{




void
process::
step() noexcept
{
  auto&  frame = *m_top_frame;

    if(frame.calling)
    {
      auto&  cal = *frame.calling;

        if(cal.eval_it != cal.eval_it_end)
        {
          operate_stack(cal.operand_stack,*cal.eval_it++,*this);
        }

      else
        if(cal.expr_it != cal.expr_it_end)
        {
          auto&  e = *cal.expr_it++;

          cal.eval_it     = e.begin();
          cal.eval_it_end = e.end();
        }

      else
        {
          auto  previous = cal.previous;

          std::vector<value>  buf;

            for(auto&  o: cal.operand_stack)
            {
              buf.emplace_back(o.evaluate(*this));
            }


          value_list  vals(buf.data(),buf.size());

            if(cal.routine)
            {
              call(*cal.routine,vals,cal.return_value);
            }

          else
            {
//              auto&  mc = cal.method_calling;

//              *cal.return_value = (**mc.method)(mc.data,vals);
            }


          delete frame.calling           ;
                 frame.calling = previous;
        }
    }

  else
    if(frame.eval_it)
    {
        if(frame.eval_it != frame.eval_it_end)
        {
          operate_stack(frame.operand_stack,*frame.eval_it++,*this);
        }

      else
        {
          finish_stmt();

          frame.eval_it = nullptr;

            if(is_exited() || is_sleeping())
            {
              return;
            }
        }
    }

  else
    if(frame.current < frame.end)
    {
      auto&  stmt = *frame.current;

        if(stmt.is_return()                ||
           stmt.is_sleep()                 ||
           stmt.is_exit()                  ||
           stmt.is_print()                 ||
           stmt.is_evaluate_and_dump()     ||
           stmt.is_evaluate_and_save()     ||
           stmt.is_evaluate_and_zero()     ||
           stmt.is_evaluate_and_not_zero() ||
           stmt.is_evaluate_and_equal()    ||
           stmt.is_evaluate_and_not_equal())
        {
          auto&  e = stmt.get_expr();

          frame.eval_it     = e.begin();
          frame.eval_it_end = e.end();

          frame.operand_stack.reset();
        }

      else
        if(stmt.is_jump())
        {
          frame.jump(stmt.get_label().view());

          ++frame.current;
        }

      else
        if(stmt.is_jump_by_condition())
        {
            if(frame.condition)
            {
              frame.jump(stmt.get_label().view());
            }


          ++frame.current;
        }

      else
        {
          ++frame.current;
        }
    }

  else
    {
      return_(value());
    }
}




}}




