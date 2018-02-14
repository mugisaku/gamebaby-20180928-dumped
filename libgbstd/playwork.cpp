#include"libgbstd/playwork.hpp"
#include"libgbstd/controller.hpp"
#include<new>




namespace gbstd{
namespace playworks{


namespace{


bool
debug_flag;


class
context
{
  uint32_t  m_pc;

  const char*  m_label;

  callback      m_step;
  coreturn  m_coreturn;

public:
  context() noexcept{}
  context(const char*  label, callback  step) noexcept:
  m_pc(0),
  m_label(label),
  m_coreturn(nullptr),
  m_step(step){}

  void  step() noexcept
  {
      if(debug_flag)
      {
        printf("[coprocess report] %s %8d\n",m_label,m_pc);
      }


    m_step(m_pc);
  }


  const char*  get_label() const noexcept{return m_label;}

  void  set_coreturn(coreturn  ret) noexcept{m_coreturn = ret;}

  void  do_return(int  v) noexcept
  {
      if(m_coreturn)
      {
        m_coreturn(v);
      }
  }

};


constexpr uint32_t  key_flags = (controller::p_button_flag|
                                 controller::n_button_flag);


uint32_t
flags;


constexpr int  number_of_contexts_max = 80;


context
stack[number_of_contexts_max];


int
number_of_contexts;


int
returned_value;


bool
need_to_pop;


bool
pushed;


}




void
debug(bool  v) noexcept
{
  debug_flag = v;
}


void
push(coreturn  ret, const playwork&  pw) noexcept
{
    if(number_of_contexts >= number_of_contexts_max)
    {
      printf("これ以上pushできない\n");

      return;
    }


    if(!pw.get_step())
    {
      printf("stepがない\n");

      return;
    }


    if(pushed)
    {
      printf("連続でpushはできない\n");

      return;
    }


    if(!need_to_pop)
    {
        if(ret && number_of_contexts)
        {
          stack[number_of_contexts-1].set_coreturn(ret);
        }


      new(&stack[number_of_contexts++]) context(pw.get_label(),pw.get_step());

      pushed = true;

      wait_until_button_is_released();
    }

  else
    {
      printf("popが内部実行されるまで、pushできない\n");
    }
}


void
pop() noexcept
{
    if(!need_to_pop)
    {
      need_to_pop = true;
    }

  else
    {
      printf("連続でpopはできない\n");
    }
}


void
pop(int  v) noexcept
{
    if(!need_to_pop)
    {
      returned_value = v;

      need_to_pop = true;
    }

  else
    {
      printf("連続でpopはできない\n");
    }
}


void
wait_until_button_is_released() noexcept
{
  flags = 0xFFFFFFFF;
}


void
call() noexcept
{
    if(!number_of_contexts_max)
    {
      printf("スタックが空\n");

      return;
    }


  flags &= ctrl.get();

    if(!(flags&key_flags))
    {
      pushed = false;

      stack[number_of_contexts-1].step();

        if(need_to_pop)
        {
          --number_of_contexts;

          need_to_pop = false;

            if(number_of_contexts)
            {
              stack[number_of_contexts-1].do_return(returned_value);
           }
        }
    }
}




}}




