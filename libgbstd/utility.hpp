#ifndef utility_hpp_was_included
#define utility_hpp_was_included


#include<cstdarg>
#include<cstdlib>
#include<cstdio>
#include<cstdint>
#include<random>


#ifndef report
#define report printf("[report %8d] %s, %s, %d\n",gbstd::reporting_counter++,__FILE__,__func__,__LINE__);
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


void      set_caption(const char*  s) noexcept;
void  set_description(const char*  s) noexcept;
void  generate_saved_image_link(int  w, int  h) noexcept;


extern std::random_device
random_device;


extern std::default_random_engine
random_number_engine;


class
normal_rand
{
  std::normal_distribution<>  m_dist;

public:
  normal_rand() noexcept{}
  normal_rand(double  mean, double  stddev) noexcept{reset(mean,stddev);}

  void  reset(double  mean, double  stddev) noexcept{m_dist = std::normal_distribution<>(mean,stddev);}

  double  operator()() noexcept{return m_dist(random_number_engine);}

};


class
uniform_rand
{
  std::uniform_real_distribution<>  m_dist;

public:
  uniform_rand() noexcept{}
  uniform_rand(double  min, double  max) noexcept{reset(min,max);}

  void  reset(double  min, double  max) noexcept{m_dist = std::uniform_real_distribution<>(min,max);}

  double  operator()() noexcept{return m_dist(random_number_engine);}

};


}


#endif




