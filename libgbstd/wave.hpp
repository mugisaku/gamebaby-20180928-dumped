#ifndef wave_hpp_included
#define wave_hpp_included


#include"libgbstd/riff.hpp"
#include<cstdio>


namespace gbstd{
namespace waves{


class
wave
{
  uint16_t  m_number_of_channels=0;
  uint32_t  m_sampling_rate=0;
  uint32_t  m_byte_rate=0;
  uint16_t  m_block_align=0;
  uint16_t  m_number_of_bits_per_sample=0;

  uint32_t  m_length=0;

  const uint8_t*  m_data=nullptr;

  void  read_fmt( const riff_subchunk_view&  rv) noexcept;
  void  read_data(const riff_subchunk_view&  rv) noexcept;

public:
  wave() noexcept{}
  wave(const riff_subchunk_view&  rv) noexcept{assign(rv);}

  void  assign(const riff_subchunk_view&  rv) noexcept;

  uint16_t  number_of_channels() const noexcept{return m_number_of_channels;}
  uint32_t  sampling_rate() const noexcept{return m_sampling_rate;}
  uint32_t  byte_rate() const noexcept{return m_byte_rate;}
  uint16_t  block_align() const noexcept{return m_block_align;}
  uint16_t  number_of_bits_per_sample() const noexcept{return m_number_of_bits_per_sample;}

  uint32_t  length() const noexcept{return m_length;}

  const uint8_t*  data() const noexcept{return m_data;}

  void  save_to_file(FILE*  f) const noexcept;

  void  print() const noexcept;

};


}


using waves::wave;


}


#endif




