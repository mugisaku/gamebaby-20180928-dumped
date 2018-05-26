#include"libgbstd/space.hpp"
#include"libgbstd/text.hpp"




namespace gbstd{
namespace spaces{




void
text_object::
set_string(gbstd::string_view  sv) noexcept
{
  utf8_decoder  dec(sv);

  m_string.resize(0);

    while(dec)
    {
      auto  c = static_cast<char16_t>(dec());

        if(!c)
        {
          break;
        }


      m_string.append(c);
    }


  set_width( font_width*m_string.size());
  set_height(font_height);

  body::update();
}


void
text_object::
set_string(gbstd::u16string_view  sv) noexcept
{
  m_string = sv;

  set_width( font_width*m_string.size());
  set_height(font_height);

  body::update();
}




void
text_object::
align_left() noexcept
{
  m_rendering_offset.x = 0;
}


void
text_object::
align_right() noexcept
{
  m_rendering_offset.x = -get_width();
}


void
text_object::
align_center() noexcept
{
  m_rendering_offset.x = -(get_width()/2);
}




void
text_object::
render(image&  dst) noexcept
{
  auto  pt = get_base_point()+m_rendering_offset;

  dst.draw_text(m_string,m_style,pt.x,pt.y);
}




}}




