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
}


void
text_object::
set_string(gbstd::u16string_view  sv) noexcept
{
  m_string = sv;
}


void
text_object::
render(image&  dst) const noexcept
{
  auto  pt = get_point();

  dst.draw_text(m_string,m_style,pt.x,pt.y);
}




}}




