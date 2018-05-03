#include"libgbstd/widget.hpp"
#include"libgbstd/text.hpp"



namespace gbstd{
namespace widgets{



const icon*
up_icons[] =
{
  &icons::right,
  &icons::sunken_right,

};


const icon*
down_icons[] =
{
  &icons::left,
  &icons::sunken_left,

};


constexpr const char*  format = "%2d/%2d";


dial::
dial(int  min, int  max, void  (*callback)(dial&,int,int)) noexcept:
m_current(min),
m_min(min),
m_max(max),
m_callback(callback)
{
  m_down_selector = new icon_selector(down_icons);
  m_up_selector   = new icon_selector(  up_icons);

  m_down_button = new button(new table_row({m_down_selector}),down);
  m_up_button   = new button(new table_row({  m_up_selector}),up);

  m_down_button->set_userdata(this);
  m_up_button->set_userdata(this);


  string_form  fs;

  m_label = new label(fs(format,min,max));

  static const background_style  bgst(color(6,6,6));

  m_label->set_background_style(bgst);

  append_child({m_down_button,m_up_button,m_label});
}




void
dial::
up(button&  btn)
{
  auto&  dial = *static_cast<widgets::dial*>(btn.get_userdata());

  dial.m_up_selector->set_current(btn.is_pressed()? 1:0);

    if(btn.get_count())
    {
      btn.reset_count();

        if(dial.m_current < dial.m_max)
        {
          auto  old_value = dial.m_current++;

            if(dial.m_callback)
            {
              dial.m_callback(dial,old_value,dial.m_current);
            }


          string_form  fs;

          dial.m_label->set_text(fs(format,dial.m_current,dial.m_max));
        }
    }
}


void
dial::
down(button&  btn)
{
  auto&  dial = *static_cast<widgets::dial*>(btn.get_userdata());

  dial.m_down_selector->set_current(btn.is_pressed()? 1:0);

    if(btn.get_count())
    {
      btn.reset_count();

        if(dial.m_current > dial.m_min)
        {
          auto  old_value = dial.m_current--;

            if(dial.m_callback)
            {
              dial.m_callback(dial,old_value,dial.m_current);
            }


          string_form  fs;

          dial.m_label->set_text(fs(format,dial.m_current,dial.m_max));
        }
    }
}


}}




