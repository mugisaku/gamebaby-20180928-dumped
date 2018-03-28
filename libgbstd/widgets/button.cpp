#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




constexpr int  margin = 4;


button::
button(gbstd::u16string_view  sv, void  (*callback)(button&)) noexcept:
widget(0,16+(margin*2)),
m_text(sv),
m_callback(callback)
{
}


button::
button(const widgets::icon&  icon, gbstd::u16string_view  sv, void  (*callback)(button&)) noexcept:
widget(0,icon::size+(margin*2)),
m_icon(&icon),
m_text(sv),
m_callback(callback)
{
}




void
button::
do_when_cursor_got_out() noexcept
{
  m_state = state::released;

  need_to_redraw();
}


void
button::
do_when_mouse_acted(int  x, int  y) noexcept
{
    if(is_released())
    {
        if(ctrl.is_mouse_lbutton_pressed())
        {
          m_state = state::pressed;

          need_to_redraw();
        }
    }

  else
    {
        if(!ctrl.is_mouse_lbutton_pressed())
        {
          m_state = state::released;

            if(m_callback)
            {
              m_callback(*this);
            }


          do_when_cursor_got_out();
        }
    }
}


void
button::
reform(point  base_pt) noexcept
{
  m_width = (m_icon? icon::size:0)+(8*m_text.size())+(margin*2);

  widget::reform(base_pt);
}


void
button::
set_text(gbstd::u16string_view  sv) noexcept
{
  m_text = sv;

  need_to_reform();
}




void
button::
render(image_cursor  cur) noexcept
{
    if(m_icon)
    {
        for(int  y = 0;  y < icon::size;  ++y){
        for(int  x = 0;  x < icon::size;  ++x){
          cur.draw_dot(m_icon->get_color_index(x,y),margin+x,margin+y);
        }}
    }


  cur.draw_text(m_text,text_style(),margin+(m_icon? icon::size:0),margin);

  cur.draw_rectangle(predefined::white,0,0,m_width,m_height);
}




}}




