#include"libgbsnd/execution.hpp"
#include"libgbsnd/object.hpp"
#include"libgbsnd/stmt.hpp"


namespace gbsnd{


struct
calling_preparation
{
  calling_preparation*  previous;

  const stmts::routine*  routine;

  exprs::operand_stack  operand_stack;

  exprs::expr_list  expr_list;

  const expr*  expr_it;
  const expr*  expr_it_end;

  const expr_element*  eval_it;
  const expr_element*  eval_it_end;

  value*  return_value;

};


struct
execution_context::
frame
{
  frame*  previous;

  calling_preparation*  calling;

  gbstd::string  routine_name;

  const stmt*    begin;
  const stmt*  current;
  const stmt*      end;

  std::list<object>  object_list;

  int  saved_value;

  bool  condition;

  exprs::operand_stack  operand_stack;

  const expr_element*  eval_it;
  const expr_element*  eval_it_end;

  value*  return_value;

  void  jump(gbstd::string_view  label) noexcept
  {
    auto  it = begin;

      while(it != end)
      {
          if(it->is_label() && (it->get_label() == label))
          {
            current = it;

            return;
          }


        ++it;
      }


    printf("jump error: %sというラベルが見つからない\n",label.data());
  }

};




void
execution_context::
reset(const script&  scr) noexcept
{
  clear();

  m_script = scr;
}




void
execution_context::
clear() noexcept
{
  auto  current = m_top_frame;

    while(current)
    {
      auto  previous = current->previous;

      auto  c = current->calling;

        while(c)
        {
          auto  c_previous = c->previous;

          delete c             ;
                 c = c_previous;
        }


      delete current           ;
             current = previous;
    }


  m_top_frame = nullptr;

  m_number_of_frames = 0;

  m_state = state::not_ready;
}


void
execution_context::
call(const stmts::routine&  routine, const value_list&  argument_list, value*  return_value) noexcept
{
  auto&  paras = routine.get_parameter_list();

   if(paras.size() != argument_list.size())
   {
      printf("引数の数が一致しない paras(%d) != args(%d)\n",paras.size(),argument_list.size());

      m_state = state::not_ready;

      return;
    }


    if(!m_top_frame)
    {
      m_top_frame = new frame;

      m_top_frame->previous = nullptr;
    }

  else
    {
      auto  previous = m_top_frame            ;
                       m_top_frame = new frame;

      m_top_frame->previous = previous;
    }


  ++m_number_of_frames;

//  printf("number of frames %d\n,",m_number_of_frames);


  auto&  frm = *m_top_frame;

  frm.calling      = nullptr;
  frm.eval_it      = nullptr;
  frm.return_value = return_value;

  auto&  ls = routine.get_stmt_list();

  frm.begin   = ls.begin();
  frm.current = ls.begin();
  frm.end     = ls.end();

  auto  arg_it = argument_list.begin();

    for(auto&  p: paras)
    {
      frm.object_list.emplace_back(value(*arg_it++),p);
    }


  m_state = state::ready;
}


void
execution_context::
call(gbstd::string_view  routine_name, const value_list&  argument_list, value*  return_value) noexcept
{
  gbstd::string_copy  sc(routine_name);


  auto  r = m_script.find_routine(routine_name);

    if(!r)
    {
      printf("%sというルーチンが見つからない",sc.data());

      m_state = state::not_ready;

      return;
    }


  call(*r,argument_list,return_value);
}


void
execution_context::
prepare_call(const routine&  routine, const expr_list&  argument_list, value*  return_value) noexcept
{
    if(!m_top_frame)
    {
      printf("prepare_call error: フレームがない\n");

      return;
    }


  auto&  frm = *m_top_frame;

    if(argument_list.size())
    {
        if(!frm.calling)
        {
          frm.calling = new calling_preparation;

          frm.calling->previous = nullptr;
        }

      else
        {
          auto  previous = frm.calling                          ;
                           frm.calling = new calling_preparation;

          frm.calling->previous = previous;
        }


      frm.calling->routine = &routine;

      frm.calling->expr_list = argument_list;

      frm.calling->expr_it     = frm.calling->expr_list.begin();
      frm.calling->expr_it_end = frm.calling->expr_list.end();

      frm.calling->eval_it     = nullptr;
      frm.calling->eval_it_end = nullptr;

      frm.calling->return_value = return_value;
    }

  else
    {
      call(routine,{},return_value);
    }
}


value
execution_context::
get_value(gbstd::string_view  name) const noexcept
{
  auto&  frm = *m_top_frame;

    for(auto&  obj: frm.object_list)
    {
        if(obj.get_name() == name)
        {
          return value(reference(obj));
        }
    }


    for(auto&  obj: m_script.get_object_list())
    {
        if(obj.get_name() == name)
        {
          return value(reference(obj));
        }
    }


  frm.object_list.emplace_back(name);

  return value(reference(frm.object_list.back()));
}


void
execution_context::
finish_stmt(millisecond  ms) noexcept
{
  auto&  frame = *m_top_frame;

  auto&  stack = frame.operand_stack;

  auto&  stmt = *frame.current++;

    if(stmt.is_return())
    {
      return_(stack.size()? stack.top().evaluate(this):value());
    }

  else
    if(stmt.is_sleep())
    {
      m_rising_time = ms.value+(stack.size()? stack.top().evaluate(this).get_integer():0);

      m_state = state::sleeping;
    }

  else
    if(stmt.is_exit())
    {
        if(frame.return_value)
        {
          *frame.return_value = stack.size()? stack.top().evaluate(this):value();
        }


      m_state = state::exited;
    }

  else
    if(stmt.is_print())
    {
        if(stack.size())
        {
          printf("PRINT: ");

          stack.top().evaluate(this).print();

          printf("\n");
        }
    }

  else
    if(stack.size())
    {
      auto  i = stack.top().evaluate(this).get_integer_safely();

        if(stmt.is_evaluate_and_dump())
        {
        }

      else
        if(stmt.is_evaluate_and_save())
        {
          frame.saved_value = i;
        }

      else
        if(stmt.is_evaluate_and_zero())
        {
          frame.condition = !i;
        }

      else
        if(stmt.is_evaluate_and_not_zero())
        {
          frame.condition = i;
        }

      else
        if(stmt.is_evaluate_and_equal())
        {
          frame.condition = (frame.saved_value == i);
        }

      else
        if(stmt.is_evaluate_and_not_equal())
        {
          frame.condition = (frame.saved_value != i);
        }
    }
}


void
execution_context::
return_(value  v) noexcept
{
    if(m_top_frame)
    {
      auto  previous = m_top_frame->previous;

        if(m_top_frame->return_value)
        {
          *m_top_frame->return_value = std::move(v);
        }


      delete m_top_frame           ;
             m_top_frame = previous;

      --m_number_of_frames;
    }
}


void
execution_context::
run(millisecond  ms) noexcept
{
    if(is_sleeping() && (m_rising_time <= ms.value))
    {
      m_state = state::ready;
    }


    if(!is_ready())
    {
      return;
    }


  constexpr size_t  count_limit = 1000;

  size_t  count = 0;

    while(m_number_of_frames)
    {
        if(++count >= count_limit)
        {
          printf("run error: カウンタ上限を越えた\n");

          break;
        }


      auto&  frame = *m_top_frame;

        if(frame.calling)
        {
          auto&  cal = *frame.calling;

            if(cal.eval_it != cal.eval_it_end)
            {
              operate_stack(cal.operand_stack,*cal.eval_it++,this);
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
              auto  previous = frame.calling->previous;

              std::vector<value>  buf;

                for(auto&  o: frame.calling->operand_stack)
                {
                  buf.emplace_back(o.evaluate(this));
                }


              value_list  vals(buf.data(),buf.size());

              call(*frame.calling->routine,vals,frame.calling->return_value);

              delete frame.calling           ;
                     frame.calling = previous;
            }
        }

      else
        if(frame.eval_it)
        {
            if(frame.eval_it != frame.eval_it_end)
            {
              operate_stack(frame.operand_stack,*frame.eval_it++,this);
            }

          else
            {
              finish_stmt(ms);

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
              frame.jump(stmt.get_label());

              ++frame.current;
            }

          else
            if(stmt.is_jump_by_condition())
            {
                if(frame.condition)
                {
                  frame.jump(stmt.get_label());
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


  m_state = state::exited;
}


}




