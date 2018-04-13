#ifndef wave_hpp_included
#define wave_hpp_included


#include"libgbstd/ios/riff.hpp"
#include<cstdio>


namespace gbstd{
namespace waves{


struct
wave_format
{
  uint16_t  id;
  uint16_t  number_of_channels;
  uint32_t  sampling_rate;
  uint32_t  byte_rate;
  uint16_t  block_align;
  uint16_t  number_of_bits_per_sample;

};


class
wave
{
  wave_format  m_format;

  uint32_t  m_length=0;

  const uint8_t*  m_data=nullptr;

  void  read_fmt( const riff_subchunk_view&  rv) noexcept;
  void  read_data(const riff_subchunk_view&  rv) noexcept;

public:
  wave() noexcept{}
  wave(const riff_subchunk_view&  rv) noexcept{assign(rv);}

  void  assign(const riff_subchunk_view&  rv) noexcept;
  void  assign(const uint8_t*  data, size_t  length, const wave_format&  fmt) noexcept;

  const wave_format&  get_format() const noexcept{return m_format;}

  uint32_t  length() const noexcept{return m_length;}

  const uint8_t*  data() const noexcept{return m_data;}

  void  save_to_file(FILE*  f) const noexcept;

  void  print() const noexcept;

};


}


using waves::wave;
using waves::wave_format;


}


#endif




