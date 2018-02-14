#include"libgbsnd/device.hpp"


namespace gbsnd{
namespace devices{




void
noise::
set_keyon_flag(bool  v) noexcept
{
  device::set_keyon_flag(v);

  m_time = m_number_of_samples_per_cycles;

  m_seed = 0xFFFF;

  m_result = 1;
}


void
noise::
output(uint32_t*  begin, uint32_t*  end) noexcept
{
  auto  it = begin;

  int  v = 0;

    while(it != end)
    {
        if(test_keyon_flag())
        {
            if(m_time >= m_number_of_samples_per_cycles)
            {
                if(!m_seed)
                {
                  m_seed = 1;
                }


              m_seed += m_seed + (((m_seed>>(m_shortspan_flag? 6:14))^
                                   (m_seed>>(m_shortspan_flag? 5:13)))&1);
              m_result ^= m_seed&1;

              v = m_result? m_volume:-m_volume;

              m_time = 0;
            }
        }


      *it++ += (v<<16);
 
      ++m_time;

      modify_volume();
      check_play_length();
    }
}


}}




