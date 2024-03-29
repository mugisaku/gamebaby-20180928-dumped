#include"libgbsnd/device.hpp"


namespace gbsnd{
namespace devices{




void
device::
set_keyon_flag(bool  v) noexcept
{
  m_time = 0;

  m_keyon_flag = v;
}


void
device::
set_number_of_cycles_per_second(double  n) noexcept
{
  constexpr auto  dsp_freq = static_cast<double>(number_of_samples_per_second);

    if((n > 0.0) && (n <= dsp_freq))
    {
      m_number_of_cycles_per_second =          n;
      m_number_of_samples_per_cycle = dsp_freq/n;

      m_need_update = true;
    }
}


void
device::
modify_volume() noexcept
{
    if(m_vm_wait_count_source)
    {
        if(m_vm_wait_count)
        {
          --m_vm_wait_count;
        }


        if(!m_vm_wait_count)
        {
          m_vm_wait_count = m_vm_wait_count_source;


          int  n = m_volume;

               if((m_vm_moddir == moddir::up  ) && (n < 255)){set_volume(n+1);}
          else if((m_vm_moddir == moddir::down) && (n      )){set_volume(n-1);}
        }
    }
}


}}




