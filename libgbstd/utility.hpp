#ifndef utility_TEMPLATE
#define utility_TEMPLATE


#include<cstdarg>
#include<cstdlib>
#include<cstdio>


#ifndef report
#define report printf("[report %8d] %s, %s, %d\n",reporting_counter++,__FILE__,__func__,__LINE__);
#endif


namespace gbstd{


extern int  reporting_counter;


template<typename  T>
inline void
destruct(T&  t) noexcept
{
  t.~T();
}


template<typename  T>
inline T*
duplicate(const T*  src) noexcept
{
  return src? new T(*src):nullptr;
}


template<typename  T>
inline T*
duplicate(const T*  src, size_t  n) noexcept
{
    if(src)
    {
      auto  p = new T[n];

      auto  dst = p;

        while(n--)
        {
          *dst++ = *src++;
        }


      return p;
    }


  return nullptr;
}


class
tmpstr
{
  char  buf[256];

public:
  tmpstr() noexcept: buf{0}{}
  tmpstr(const char*  fmt, ...) noexcept;

  const char*  operator*() const noexcept{return buf;}

  const char*  operator()(const char*  fmt, ...) noexcept;

};


void  printf_with_indent(int  indent, const char*  fmt, ...) noexcept;


}


#endif




