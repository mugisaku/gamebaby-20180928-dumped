#include"libgbstd/text.hpp"
#include<cstdio>




namespace gbstd{
namespace texts{


string_form::
string_form(const char*  fmt, ...) noexcept
{
  va_list  ap;
  va_start(ap,fmt);

  auto  res = vsnprintf(buf,sizeof(buf),fmt,ap);

    if((res < -1) || (res >= sizeof(buf)))
    {
      report;
    }


  va_end(ap);
}


const char*
string_form::
operator()(const char*  fmt, ...) noexcept
{
  va_list  ap;
  va_start(ap,fmt);

  auto  res = vsnprintf(buf,sizeof(buf),fmt,ap);

    if((res < -1) || (res >= sizeof(buf)))
    {
      report;
    }


  va_end(ap);

  return buf;
}




}}




