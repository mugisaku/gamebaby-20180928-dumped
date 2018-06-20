#include"libgbact/routine.hpp"




namespace gbact{
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




