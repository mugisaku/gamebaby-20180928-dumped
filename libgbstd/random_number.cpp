#include"libgbstd/random_number.hpp"
#include<random>




namespace gbstd{
namespace random_number{


namespace{


std::default_random_engine
engine;


struct
initializer
{
  initializer() noexcept
  {
    engine = std::default_random_engine(std::random_device()());
  }

} init;


}




double
uniform_distribution::
operator()(double  min, double  max) const noexcept
{
  std::uniform_real_distribution<>  dist(min,max);

  return dist(engine);
}


double
normal_distribution::
operator()(double  mean, double  stddev) const noexcept
{
  std::normal_distribution<>  dist(mean,stddev);

  return dist(engine);
}




void
seed::
reset() noexcept
{
  std::random_device  randev;

  x = randev();
  y = randev();
  z = randev();
  w = randev();
}


void
seed::
reset(uint32_t  x_, uint32_t  y_, uint32_t  z_, uint32_t  w_) noexcept
{
  x = x_;
  y = y_;
  z = z_;
  w = w_;
}


}}




