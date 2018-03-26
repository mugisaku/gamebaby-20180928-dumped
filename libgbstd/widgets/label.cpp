#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




label::
label(gbstd::u16string_view  sv) noexcept:
widget(0,16+4)
{
  set_text(sv);
}




void
label::
reform() noexcept
{
  m_width = (8*m_text.size())+4;

  widget::reform();
}


void
label::
set_text(gbstd::u16string_view  sv) noexcept
{
  m_text = sv;

  need_to_reform();
}


void
label::
render(image_cursor  cur) noexcept
{
  cur.draw_text(m_text,text_style(),2,2);
}




}}




