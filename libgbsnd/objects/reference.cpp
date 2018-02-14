#include"libgbsnd/object.hpp"
#include"libgbsnd/script.hpp"
#include<new>


namespace gbsnd{
namespace objects{




namespace{
int
sq_number_of_cycles_per_seconds(square_wave*  sq, const int*  v)
{
    if(v)
    {
      sq->set_number_of_cycles_per_seconds(*v);
    }


  return sq->get_number_of_cycles_per_seconds();
}


int
sq_play_length(square_wave*  sq, const int*  v)
{
    if(v)
    {
      sq->set_play_length(*v);
    }


  return sq->get_play_length();
}


int
sq_play_length_flag(square_wave*  sq, const int*  v)
{
    if(v)
    {
      sq->set_play_length_flag(*v);
    }


  return sq->test_play_length_flag();
}


int
sq_keyon_flag(square_wave*  sq, const int*  v)
{
    if(v)
    {
      sq->set_keyon_flag(*v);
    }


  return sq->test_keyon_flag();
}


int
sq_volume(square_wave*  sq, const int*  v)
{
    if(v)
    {
      sq->set_volume(*v);
    }


  return sq->get_volume();
}


int
sq_vm_wait_count_source(square_wave*  sq, const int*  v)
{
    if(v)
    {
      sq->set_vm_wait_count_source(*v);
    }


  return sq->get_vm_wait_count_source();
}


int
sq_vm_moddir(square_wave*  sq, const int*  v)
{
    if(v)
    {
      sq->set_vm_moddir(*v? moddir::up:moddir::down);
    }


  return (sq->get_vm_moddir() == moddir::up)? 1:0;
}


int
sq_fm_shift_amount(square_wave*  sq, const int*  v)
{
    if(v)
    {
      sq->set_fm_shift_amount(*v);
    }


  return sq->get_fm_shift_amount();
}


int
sq_fm_wait_count_source(square_wave*  sq, const int*  v)
{
    if(v)
    {
      sq->set_fm_wait_count_source(*v);
    }


  return sq->get_fm_wait_count_source();
}


int
sq_fm_moddir(square_wave*  sq, const int*  v)
{
    if(v)
    {
      sq->set_fm_moddir(*v? moddir::up:moddir::down);
    }


  return (sq->get_fm_moddir() == moddir::up)? 1:0;
}


int
sq_duty_ratio(square_wave*  sq, const int*  v)
{
    if(v)
    {
      sq->set_duty_ratio(*v);
    }


  return sq->get_duty_ratio();
}




int
no_play_length(noise*  no, const int*  v)
{
    if(v)
    {
      no->set_play_length(*v);
    }


  return no->get_play_length();
}


int
no_play_length_flag(noise*  no, const int*  v)
{
    if(v)
    {
      no->set_play_length_flag(*v);
    }


  return no->test_play_length_flag();
}


int
no_keyon_flag(noise*  no, const int*  v)
{
    if(v)
    {
      no->set_keyon_flag(*v);
    }


  return no->test_keyon_flag();
}


int
no_volume(noise*  no, const int*  v)
{
    if(v)
    {
      no->set_volume(*v);
    }


  return no->get_volume();
}


int
no_shortspan_flag(noise*  no, const int*  v)
{
    if(v)
    {
      no->set_shortspan_flag(*v);
    }


  return no->test_shortspan_flag();
}




}


property
reference::
get_property(const identifier&  id) const noexcept
{
  auto&  obj = *m_pointer;

  using sv = gbstd::string_view;

  auto  name = id.view();

    if(obj.is_square_wave())
    {
      auto&  sq = obj.get_square_wave();

           if(name == sv("keyon_flag"          )){return property(sq,sq_keyon_flag);}
      else if(name == sv("volume"              )){return property(sq,sq_volume);}
      else if(name == sv("frequency"           )){return property(sq,sq_number_of_cycles_per_seconds);}
      else if(name == sv("play_length"         )){return property(sq,sq_play_length);}
      else if(name == sv("play_length_flag"    )){return property(sq,sq_play_length_flag);}
      else if(name == sv("vm_wait_count_source")){return property(sq,sq_vm_wait_count_source);}
      else if(name == sv("vm_moddir"           )){return property(sq,sq_vm_moddir);}
      else if(name == sv("fm_shift_amount"     )){return property(sq,sq_fm_shift_amount);}
      else if(name == sv("fm_wait_count_source")){return property(sq,sq_fm_wait_count_source);}
      else if(name == sv("fm_moddir"           )){return property(sq,sq_fm_moddir);}
      else if(name == sv("duty_ratio"          )){return property(sq,sq_duty_ratio);}
//  else if(name == sv("")){return property();}
      else {printf("%sという名のプロパティは見つからない\n",name.data());}


    }

  else
    if(obj.is_noise())
    {
      auto&  no = obj.get_noise();

           if(name == sv("keyon_flag"              )){return property(no,no_keyon_flag);}
      else if(name == sv("volume"                  )){return property(no,no_volume);}
      else if(name == sv("play_length"             )){return property(no,no_play_length);}
      else if(name == sv("play_length_flag"        )){return property(no,no_play_length_flag);}
      else if(name == sv("shortspan_flag"          )){return property(no,no_shortspan_flag);}
//  else if(name == sv("")){return property();}
      else {printf("%sという名のプロパティは見つからない\n",name.data());}


    }


  auto  fn = [](void*  ptr, const int*  v)->int{return 0;};

  return property(m_pointer,fn);
}


}}




