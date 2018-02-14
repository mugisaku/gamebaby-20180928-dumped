#ifndef LIBGBSND_HPP
#define LIBGBSND_HPP


#include<cstdint>
#include<cstdio>
#include"libgbstd/string.hpp"


namespace gbsnd{
namespace devices{


constexpr int  number_of_samples_per_seconds = 24000;//1秒あたりのサンプル数


enum class
moddir: uint8_t //Modification Direction:変更方向
{
    up,
  down,
};


class
device
{
  gbstd::string  m_name;

protected:
  uint32_t  m_time=0;

  double  m_number_of_cycles_per_seconds=0;//1秒あたりの周波数
  double  m_number_of_samples_per_cycles=0;//1周波あたりのサンプル数

  uint32_t  m_play_length=0;

//vm = Volume Modification = 音数変更
  uint32_t  m_vm_wait_count_source=0;//音量変更待機値元値
  uint32_t  m_vm_wait_count       =0;//音量変更待機値

  moddir  m_vm_moddir=moddir::up;

  uint8_t  m_volume=0;

  bool  m_play_length_flag=false;
  bool  m_keyon_flag=false;//真の時、音を鳴らす

  bool  m_need_update=true;

  void  modify_volume() noexcept;

  void  check_play_length() noexcept;

public:
  bool  operator==(gbstd::string_view  name) const noexcept{return m_name == name;}

  void                  set_name(gbstd::string_view  name)       noexcept{       m_name = name;}
  const gbstd::string&  get_name(                        ) const noexcept{return m_name       ;}

  uint32_t  get_time() const noexcept{return m_time;}

  void      set_number_of_cycles_per_seconds(double  n)       noexcept;
  uint32_t  get_number_of_cycles_per_seconds(         ) const noexcept{return m_number_of_cycles_per_seconds;}

  uint32_t  get_number_of_samples_per_cycles() const noexcept{return m_number_of_samples_per_cycles;}


  void      set_play_length(uint32_t  v)       noexcept{       m_play_length = v;}
  uint32_t  get_play_length(           ) const noexcept{return m_play_length    ;}

  bool   test_play_length_flag() const noexcept{return m_play_length_flag;}
  void    set_play_length_flag(bool  v=true) noexcept{m_play_length_flag = v;}
  void  unset_play_length_flag() noexcept{m_play_length_flag = false;}


  void     set_volume(uint8_t  v)       noexcept{       m_volume = v&0x7F;}
  uint8_t  get_volume(          ) const noexcept{return m_volume         ;}

  void      set_vm_wait_count_source(uint32_t  v)       noexcept{       m_vm_wait_count_source = v;}
  uint32_t  get_vm_wait_count_source(           ) const noexcept{return m_vm_wait_count_source    ;}
  uint32_t  get_vm_wait_count(                  ) const noexcept{return m_vm_wait_count           ;}

  void    set_vm_moddir(moddir  dir)       noexcept{       m_vm_moddir = dir;}
  moddir  get_vm_moddir(           ) const noexcept{return m_vm_moddir      ;}


  bool   test_keyon_flag() const noexcept{return m_keyon_flag;}
  void    set_keyon_flag(bool  v=true) noexcept;
  void  unset_keyon_flag() noexcept{m_keyon_flag = false;}

};


class
square_wave: public device
{
  uint32_t  m_constant_of_high_part_samples;
  uint32_t   m_constant_of_low_part_samples;

  uint32_t  m_count_of_samples;

  enum class mode{
    high,
     low,
  } m_mode;

//fm = Frequency Modification = 周波数変更
  uint32_t  m_fm_wait_count_source=0;
  uint32_t  m_fm_wait_count       =0;

  uint8_t   m_fm_shift_amount     =0;

  moddir  m_fm_moddir=moddir::up;

  uint8_t  m_duty_ratio=0;

  void  modify_frequency() noexcept;

public:
  void  update_parameters() noexcept;

  void     set_fm_shift_amount(uint8_t  a)       noexcept{       m_fm_shift_amount = a&7;}
  uint8_t  get_fm_shift_amount(          ) const noexcept{return m_fm_shift_amount      ;}

  void      set_fm_wait_count_source(uint32_t  v)       noexcept;
  uint32_t  get_fm_wait_count_source(           ) const noexcept{return m_fm_wait_count_source;}
  uint32_t  get_fm_wait_count(                  ) const noexcept{return m_fm_wait_count       ;}

  void    set_fm_moddir(moddir  d)       noexcept{       m_fm_moddir = d;}
  moddir  get_fm_moddir(         ) const noexcept{return m_fm_moddir    ;}

  void     set_duty_ratio(uint8_t  r)       noexcept;
  uint8_t  get_duty_ratio(          ) const noexcept{return m_duty_ratio;}

  void  output(uint32_t*  begin, uint32_t*  end) noexcept;

};


class
noise: public device
{
  uint16_t  m_seed=0xFFFF;

  uint16_t  m_result=1;

  bool  m_shortspan_flag=false;

public:
  void  set_keyon_flag(bool  v=true) noexcept;

  bool   test_shortspan_flag() const noexcept{return m_shortspan_flag;}
  void    set_shortspan_flag(bool  v=true) noexcept{m_shortspan_flag = v;}
  void  unset_shortspan_flag() noexcept{m_shortspan_flag = false;}

  void  output(uint32_t*  begin, uint32_t*  end) noexcept;

};



}


using devices::number_of_samples_per_seconds;
using devices::square_wave;
using devices::noise;
using devices::moddir;


}




#endif




