#include"utility.hpp"
#include<cstdio>




namespace gbstd{


int  reporting_counter;


tmpstr::
tmpstr(const char*  fmt, ...) noexcept
{
  va_list  ap;
  va_start(ap,fmt);

  vsnprintf(buf,sizeof(buf),fmt,ap);

  va_end(ap);
}


const char*
tmpstr::
operator()(const char*  fmt, ...) noexcept
{
  va_list  ap;
  va_start(ap,fmt);

  vsnprintf(buf,sizeof(buf),fmt,ap);

  va_end(ap);

  return buf;
}




void
printf_with_indent(int  indent, const char*  fmt, ...) noexcept
{
  char  buf[256];

  va_list  ap;
  va_start(ap,fmt);

  vsnprintf(buf,sizeof(buf),fmt,ap);

  va_end(ap);

  const char*  p = buf;

    while(*p)
    {
      fputc(*p,stdout);

        if(*p++ == '\n')
        {
            for(int  n = 0;  n < indent;  ++n)
            {
              fputc(' ',stdout);
            }
        }
    }
}




}




