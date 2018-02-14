#include"libgbsnd/device.hpp"


namespace gbsnd{
namespace devices{




void
square_wave::
update_parameters() noexcept
{
    if(m_number_of_samples_per_cycles == 0.0)
    {
      return;
    }


  double  high_part_samples = m_number_of_samples_per_cycles;

    switch(m_duty_ratio)
    {
  case(0): high_part_samples *= 0.125;break;
  case(1): high_part_samples *= 0.250;break;
  case(2): high_part_samples *= 0.500;break;
  case(3): high_part_samples *= 0.750;break;
    }


    if(high_part_samples >= 0.0)
    {
      m_constant_of_high_part_samples = high_part_samples;
      m_constant_of_low_part_samples  = (m_number_of_samples_per_cycles-high_part_samples);

//printf("%8d %8d\n",m_constant_of_high_part_samples,
//          m_constant_of_low_part_samples);

      auto  rem = m_time%static_cast<uint32_t>(m_number_of_samples_per_cycles);

        if(rem < m_constant_of_high_part_samples)
        {
          m_mode = mode::high;

          m_count_of_samples = m_constant_of_high_part_samples-rem;
        }

      else
        {
          m_mode = mode::low;

          m_count_of_samples = rem-m_constant_of_high_part_samples;
        }
    }

  else
    {
      m_constant_of_high_part_samples = 0;
    }
}


void
square_wave::
set_duty_ratio(uint8_t  r) noexcept
{
  m_duty_ratio = (r%4);

  m_need_update = true;
}


void
square_wave::
set_fm_wait_count_source(uint32_t  v) noexcept
{
  m_fm_wait_count_source = v;
  m_fm_wait_count        = v;
}


void
square_wave::
modify_frequency() noexcept
{
    if(m_fm_shift_amount)
    {
        if(m_fm_wait_count)
        {
          --m_fm_wait_count;
        }


        if(!m_fm_wait_count)
        {
          m_fm_wait_count = m_fm_wait_count_source;


          auto  n = m_number_of_cycles_per_seconds;

               if(m_fm_moddir == moddir::up  ){n = n+(n/(1+1*m_fm_shift_amount));}
          else if(m_fm_moddir == moddir::down){n = n-(n/(1+1*m_fm_shift_amount));}


          set_number_of_cycles_per_seconds((n > 0.0)? n:0.0);
        }
    }
}


void
square_wave::
output(uint32_t*  begin, uint32_t*  end) noexcept
{
    if(m_need_update)
    {
      update_parameters();

      m_need_update = false;
    }


  auto  it = begin;

  int  v = 0;

    if(!m_constant_of_high_part_samples)
    {
      return;
    }


    while(it != end)
    {
REDO:
        if(test_keyon_flag())
        {
            if(m_mode == mode::high)
            {
              v = m_volume;

                if(!--m_count_of_samples)
                {
                  m_count_of_samples = m_constant_of_low_part_samples;

                  m_mode = mode::low;
                }
            }

          else
            {
              v = -m_volume;

                if(!--m_count_of_samples)
                {
                  m_count_of_samples = m_constant_of_high_part_samples;

                  m_mode = mode::high;
                }
            }
        }


      *it++ += (v<<16);

      ++m_time;

      modify_frequency();
      modify_volume();
      check_play_length();
    }
}




}}




