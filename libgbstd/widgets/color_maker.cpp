#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{


class
sample: public widget
{
  static constexpr int  size = 32;

public:
  sample(color_maker&  cm) noexcept: widget(size,size){m_userdata = &cm;}

  void  render(image_cursor  cur) noexcept override
  {
    cur.fill_rectangle(reinterpret_cast<color_maker*>(m_userdata)->get_color(),0,0,size,size);
  }

};




color_maker::
color_maker(void  (*callback)(color_maker&,color)) noexcept:
m_callback(callback),
m_color(0,0,0)
{
  m_r_dial = new dial(0,7,update_color_internal);
  m_g_dial = new dial(0,7,update_color_internal);
  m_b_dial = new dial(0,7,update_color_internal);

  m_r_dial->set_userdata(this);
  m_g_dial->set_userdata(this);
  m_b_dial->set_userdata(this);

  m_sample = new sample(*this);

  append_child({new table_row({new label(u"[R]"),m_r_dial}),
                new table_row({new label(u"[G]"),m_g_dial}),
                new table_row({new label(u"[B]"),m_b_dial}),m_sample});
}




void
color_maker::
update_color_internal(dial&  dial, int  old_value, int  new_value) noexcept
{
  auto&  cm = *reinterpret_cast<color_maker*>(dial.get_userdata());

  cm.update_color();
}


void
color_maker::
update_color() noexcept
{
  m_color = color(m_r_dial->get_current(),
                  m_g_dial->get_current(),
                  m_b_dial->get_current());

    if(m_callback)
    {
      m_callback(*this,m_color);
    }


  m_sample->need_to_redraw();
}




}}




