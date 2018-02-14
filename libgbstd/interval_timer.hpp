#ifndef GMBB_IntervalTimer_HPP
#define GMBB_IntervalTimer_HPP


#include<cstdint>


namespace gbstd{


class
interval_timer
{
  uint32_t  m_last=0;

public:
  interval_timer() noexcept{}

  bool  check_step(uint32_t  interval, uint32_t  now) noexcept
  {
      if((m_last+interval) < now)
      {
        m_last += interval;

        return true;
      }


    return false;
  }

  bool  check(uint32_t  interval, uint32_t  now) noexcept
  {
      if((m_last+interval) < now)
      {
        m_last = now;

        return true;
      }


    return false;
  }

};


}




#endif




