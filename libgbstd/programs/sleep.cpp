#include"libgbstd/program.hpp"




namespace gbstd{
namespace programs{




sleep::
sleep(uint32_t  t) noexcept: m_time(g_time+t)
{
}




void
sleep::
step() noexcept
{
    if(g_time >= m_time)
    {
      end();
    }
}




}}




