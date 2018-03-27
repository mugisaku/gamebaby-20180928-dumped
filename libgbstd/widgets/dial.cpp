#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




const
icon
up_icon({
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
});


const
icon
down_icon({
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
});


constexpr const char*  format = "%2d/%2d";


dial::
dial(int  min, int  max, void  (*callback)(dial&,int,int)) noexcept:
m_current(min),
m_min(min),
m_max(max),
m_callback(callback)
{
  m_down_button = new button(down_icon,u"",down);
  m_up_button   = new button(  up_icon,u"",up);

  m_down_button->set_userdata(this);
  m_up_button->set_userdata(this);


  tmpstr  ts;

  m_label = new label(ts(format,min,max));

  m_table_row = new table_row({m_down_button,m_up_button,m_label});
}




void
dial::
up(button&  btn)
{
  auto&  dial = *static_cast<widgets::dial*>(btn.get_userdata());

    if(dial.m_current < dial.m_max)
    {
      auto  old_value = dial.m_current++;

        if(dial.m_callback)
        {
          dial.m_callback(dial,old_value,dial.m_current);
        }


      tmpstr  ts;

      dial.m_label->set_text(ts(format,dial.m_current,dial.m_max));
    }
}


void
dial::
down(button&  btn)
{
  auto&  dial = *static_cast<widgets::dial*>(btn.get_userdata());

    if(dial.m_current > dial.m_min)
    {
      auto  old_value = dial.m_current--;

        if(dial.m_callback)
        {
          dial.m_callback(dial,old_value,dial.m_current);
        }


      tmpstr  ts;

      dial.m_label->set_text(ts(format,dial.m_current,dial.m_max));
    }
}


}}




