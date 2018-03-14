#include"libgbscr/processes/process_private.hpp"


namespace     gbscr{
namespace processes{




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




value
process::
get_value(gbstd::string_view  name) const noexcept
{
  auto&  frm = *m_top_frame;

  auto  frm_varptr = frm.table.find(name);

    if(frm_varptr)
    {
      return value(frm_varptr);
    }


  auto  varptr = m_global_table.find(name);

    if(varptr)
    {
      return value(varptr);
    }


  return value(frm.table.append(value(),name));
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

           if(stmt.is_evaluate_and_dump()     ){}
      else if(stmt.is_evaluate_and_save()     ){frame.saved_value =  i;}
      else if(stmt.is_evaluate_and_zero()     ){frame.condition   = !i;}
      else if(stmt.is_evaluate_and_not_zero() ){frame.condition   =  i;}
      else if(stmt.is_evaluate_and_equal()    ){frame.condition   = (frame.saved_value == i);}
      else if(stmt.is_evaluate_and_not_equal()){frame.condition   = (frame.saved_value != i);}
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
          v.get_reference()->set_carry_flag();

          m_top_frame->table.carry(previous? previous->table:m_global_table);
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
print() const noexcept
{
  m_global_table.print();

  printf("\n");
}




}}




