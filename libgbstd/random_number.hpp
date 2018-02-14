#ifndef GMBB_RANDOM_NUMBER_HPP_INCLUDED
#define GMBB_RANDOM_NUMBER_HPP_INCLUDED


#include<cstdint>


namespace gbstd{
namespace random_number{


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
fill(T  begin, T  end, gbstd::random_number::seed  seed) noexcept
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


}}


#endif




