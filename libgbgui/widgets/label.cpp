#include"libgbgui/widget.hpp"
#include"libgbstd/text.hpp"



namespace gbgui{
namespace widgets{




label::
label(gbstd::string_view  sv, const text_style&  ts) noexcept:
widget(0,16+4),
m_text_style(ts)
{
  set_text(sv);
}


label::
label(gbstd::u16string_view  sv, const text_style&  ts) noexcept:
widget(0,16+4),
m_text_style(ts)
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
  modify_text(sv);

  need_to_reform();
}


void
label::
set_text(gbstd::string_view  sv) noexcept
{
  modify_text(sv);

  need_to_reform();
}


void
label::
modify_text(gbstd::u16string_view  sv) noexcept
{
  m_text = sv;

  need_to_redraw();
}


void
label::
modify_text(gbstd::string_view  sv) noexcept
{
  utf8_decoder  dec(sv);

  m_text.resize(0);

    while(dec)
    {
      auto  c = static_cast<char16_t>(dec());

        if(!c)
        {
          break;
        }


      m_text.append(c);
    }


  need_to_redraw();
}


void
label::
set_style(const text_style&  style) noexcept
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




