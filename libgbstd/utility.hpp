#ifndef utility_hpp_was_included
#define utility_hpp_was_included


#include<cstdarg>
#include<cstdlib>
#include<cstdio>
#include<cstdint>


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


void  printf_with_indent(int  indent, const char*  fmt, ...) noexcept;




namespace random_numbers{


struct
normal_distribution
{
  double  operator()(double  mean, double  stddev) const noexcept;

};


struct
uniform_distribution
{
  double  operator()(double  min, double  max) const noexcept;

};


struct
seed
{
  uint32_t  x;
  uint32_t  y;
  uint32_t  z;
  uint32_t  w;

  seed(                                                      ) noexcept{reset(           );}
  seed(uint32_t  x_, uint32_t  y_, uint32_t  z_, uint32_t  w_) noexcept{reset(x_,y_,z_,w_);}

  void  reset(                                                      ) noexcept;
  void  reset(uint32_t  x_, uint32_t  y_, uint32_t  z_, uint32_t  w_) noexcept;

};


template<typename  T>
inline void
fill(T  begin, T  end, random_numbers::seed  seed) noexcept
{
    while(begin != end)
    {
      uint32_t  t = seed.x ^ (seed.x << 11);

      seed.x = seed.y;
      seed.y = seed.z;
      seed.z = seed.w;

                 seed.w = (seed.w ^ (seed.w >> 19)) ^ (t ^ (t >> 8)); 
      *begin++ = seed.w                                             ;
    }
}


}


}


#endif




