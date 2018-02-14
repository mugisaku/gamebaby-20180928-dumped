#ifndef GMBB_playwork_HPP
#define GMBB_playwork_HPP


#include<cstdint>


namespace gbstd{
namespace playworks{


using coreturn = void  (*)(int  value);
using callback = void  (*)(uint32_t&  pc);


class
playwork
{
  const char*  m_label="";

  callback  m_step=nullptr;

public:
  playwork(const char*  label, callback  step) noexcept:
  m_label(label),
  m_step(step){}

  const char*  get_label() const noexcept{return m_label;}
  callback      get_step() const noexcept{return m_step;}

};


void  debug(bool  v) noexcept;

void  push(coreturn  ret, const playwork&  pw) noexcept;

void   pop(      ) noexcept;
void   pop(int  v) noexcept;

void  wait_until_button_is_released() noexcept;

void  call() noexcept;


}


using playworks::playwork;
using playworks::push;
using playworks::pop;
using playworks::call;
using playworks::wait_until_button_is_released;


}




#endif




