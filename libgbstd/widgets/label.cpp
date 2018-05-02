#include"libgbstd/widget.hpp"
#include"libgbstd/text.hpp"



namespace gbstd{
namespace widgets{




text_style
label::
m_default_text_style(
  predefined_color::null,
  predefined_color::black,
  predefined_color::light_gray,
  predefined_color::gray
);




label::
label(gbstd::string_view  sv) noexcept:
widget(0,16+4)
{
  set_text(sv);
}


label::
label(gbstd::u16string_view  sv) noexcept:
widget(0,16+4)
{
  set_text(sv);
}




void
label::
reform(point  base_pt) noexcept
{
  m_width = (8*m_text.size())+4;

  widget::reform(base_pt);
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
set_text(gbstd::string_view  sv) noexcept
{
  utf8_decoder  dec(sv);

  m_text.clear();

    while(dec)
    {
      m_text.append(static_cast<char16_t>(dec()));
    }


  need_to_redraw();
}


void
label::
set_text_sytle(const text_style&  style) noexcept
{
  m_text_style = style;

  need_to_redraw();
}


void
label::
render(image_cursor  cur) noexcept
{
  render_background(cur);

  cur.draw_text(m_text,m_text_style,2,2);
}




}}




