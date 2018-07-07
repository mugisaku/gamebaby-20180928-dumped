#include"libgbstd/routine.hpp"




namespace gbstd{
namespace routines{




void
sleep_context::
step() noexcept
{
    if(g_time >= m_time)
    {
      end();
    }
}




}}




