#include"libgbscr/processes/process_private.hpp"


namespace     gbscr{
namespace processes{




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




}}




