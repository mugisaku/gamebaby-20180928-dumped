#include"libgbscr/process.hpp"
#include"libgbscr/expr.hpp"
#include"libgbscr/stmt.hpp"
#include"libgbscr/stream.hpp"
#include<memory>


namespace     gbscr{
namespace processes{




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
process::
frame
{
  frame*  previous;

  calling_preparation*  calling;

  gbstd::string  routine_name;

  const stmt*    begin;
  const stmt*  current;
  const stmt*      end;

  values::table  table;

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




process::
process() noexcept
{
}


process::
~process()
{
  clear();
}




void
process::
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


  m_global_table.clear();

  m_top_frame = nullptr;

  m_number_of_frames = 0;

  m_state = state::not_ready;
}


void
process::
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
      frm.table.append(*arg_it++,p.data());
    }


  m_state = state::ready;
}


void
process::
call(gbstd::string_view  routine_name, const value_list&  argument_list, value*  return_value) noexcept
{
  gbstd::string_copy  sc(routine_name);

  auto  varptr = m_global_table.find(routine_name);

    if(!varptr || !varptr->get_value().is_routine())
    {
      printf("%sというルーチンが見つからない",sc.data());

      m_state = state::not_ready;

      return;
    }


  call(varptr->get_value().get_routine(),argument_list,return_value);
}


void
process::
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




/*
void
process::
prepare_call(const method_calling&  mc, const expr_list&  argument_list, value*  return_value) noexcept
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


      frm.calling->routine = nullptr;
      frm.calling->method_calling = mc;

      frm.calling->expr_list = argument_list;

      frm.calling->expr_it     = frm.calling->expr_list.begin();
      frm.calling->expr_it_end = frm.calling->expr_list.end();

      frm.calling->eval_it     = nullptr;
      frm.calling->eval_it_end = nullptr;

      frm.calling->return_value = return_value;
    }

  else
    {
//      *return_value = (**mc.method)(mc.data,{});
    }
}
*/


value
process::
get_value(gbstd::string_view  name) const noexcept
{
  auto&  frm = *m_top_frame;

  auto  frm_varptr = frm.table.find(name);

    if(frm_varptr)
    {
      return value(frm_varptr->get_reference());
    }


  auto  varptr = m_global_table.find(name);

    if(varptr)
    {
      return value(varptr->get_reference());
    }


  return value(frm.table.append(value(),name).get_reference());
}


void
process::
finish_stmt() noexcept
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
      m_sleeping_time = (stack.size()? stack.top().evaluate(this).get_integer_safely():0);

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
process::
return_(const value&  v) noexcept
{
    if(m_top_frame)
    {
      auto  previous = m_top_frame->previous;

        if(m_top_frame->return_value)
        {
          *m_top_frame->return_value = std::move(v);
        }


        if(v.is_reference())
        {
//
        }


      delete m_top_frame           ;
             m_top_frame = previous;

      --m_number_of_frames;
    }


    if(!m_top_frame)
    {
      m_state = state::exited;
    }
}


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
          auto  previous = cal.previous;

          std::vector<value>  buf;

            for(auto&  o: cal.operand_stack)
            {
              buf.emplace_back(o.evaluate(this));
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
          operate_stack(frame.operand_stack,*frame.eval_it++,this);
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


void
process::
run() noexcept
{
    if(!is_ready())
    {
      return;
    }


    while(m_number_of_frames)
    {
        if(is_exited() || is_sleeping() || is_not_ready())
        {
          break;
        }


      step();
    }
}




bool
process::
append_variable(const value&  value, gbstd::string_view  name) noexcept
{
  m_global_table.append(value,name);

  return true;
}




void
process::
load_file(const char*  filepath) noexcept
{
  auto  ss = make_string_from_file(filepath);

  stream  s(ss.data());

  token_string  toks(s,0,0);

  token_cursor  cur(toks);

    while(cur)
    {
      using sv = gbstd::string_view;

        if(cur[0].is_identifier() &&
           cur[1].is_identifier())
        {
          sv  type_name(cur[0].get_string().view());
          sv   var_name(cur[1].get_string().view());

          cur += 2;

            if(type_name == sv("routine"))
            {
                if(cur[0].is_token_string('(',')') &&
                   cur[1].is_token_string('{','}'))
                {
                  auto&  parals = cur[0].get_token_string();
                  auto&   block = cur[1].get_token_string();

                  
                  auto  rt = new routine(parals,block);

                  m_global_table.append(value(*rt),var_name);

                  cur += 2;
                }
            }
        }

      else
        if(cur[0].is_semicolon())
        {
          cur += 1;
        }

      else
        {
          printf("不明な構文\n");

          break;
        }
    }
}




void
process::
print() const noexcept
{
  m_global_table.print();

  printf("\n");
}




}}




