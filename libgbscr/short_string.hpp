#ifndef LIBGBSND_short_string_HPP
#define LIBGBSND_short_string_HPP


#include<cstdint>
#include"libgbstd/string_view.hpp"


namespace gbsnd{


class
operator_word
{
  uint32_t  m_data;

public:
  constexpr operator_word() noexcept: m_data(0){}

  constexpr operator_word(gbstd::string_view  sv) noexcept:
  m_data(((sv.size() >= 1)? (sv[0]<<24):0)|
         ((sv.size() >= 2)? (sv[1]<<16):0)|
         ((sv.size() >= 3)? (sv[2]<< 8):0)){}

  constexpr bool  operator==(const operator_word&  rhs) const noexcept{return m_data == rhs.m_data;}

  constexpr uint32_t  data() const noexcept{return m_data;}

};


class
short_string
{
  char  m_data[4];

public:
  constexpr short_string() noexcept: m_data{0}{}

  constexpr short_string(operator_word  opw) noexcept:
  m_data{static_cast<char>((opw.data()>>24)     ),
         static_cast<char>((opw.data()>>16)&0xFF),
         static_cast<char>((opw.data()>> 8)&0xFF),0}{}

  short_string&  operator=(operator_word  opw) noexcept
  {
    m_data[0] = (opw.data()>>24);
    m_data[1] = (opw.data()>>16)&0xFF;
    m_data[2] = (opw.data()>> 8)&0xFF;

    return *this;
  }


  constexpr const char*  data() const noexcept{return m_data;}

};


}




#endif




