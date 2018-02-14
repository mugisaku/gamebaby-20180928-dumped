#include"libgbstd/image.hpp"
#include"libgbstd/unicode.hpp"




namespace gbstd{
namespace images{




void
image::
print(char16_t  c, point  pt, const font&  fnt, ro_ptr<color_index>  coloring) noexcept
{
  auto  p = fnt.get_glyph_data(c);

    if(p)
    {
      int  w = fnt.get_width();
      int  h = fnt.get_height();

      int  bpp = fnt.get_bits_per_pixel();

      int  const shift_amount = (bpp == 1)? 31:30;

        for(int  yy = 0;  yy < h;  yy += 1)
        {
          auto   dst = &get_pixel(pt.x,pt.y+yy);
          auto  code = *p++;

            for(int  xx = 0;  xx < w;  xx += 1)
            {
              auto  src = coloring[code>>shift_amount];

                if(src)
                {
                  dst->index = src;
                }


              ++dst;

              code <<= (32-shift_amount);
            }
        }
    }
}




void
image::
print(gbstd::string_view  sv, point  pt, const font&  fnt, ro_ptr<color_index>  coloring) noexcept
{
  utf8_decoder  dec(sv);

    while(dec)
    {
      print(static_cast<char16_t>(dec()),pt,fnt,coloring);

      pt.x += fnt.get_width();
    }
}




void
image::
print(gbstd::u16string_view  sv, point  pt, const font&  fnt, ro_ptr<color_index>  coloring) noexcept
{
    for(auto  c: sv)
    {
      print(static_cast<char16_t>(c),pt,fnt,coloring);

      pt.x += fnt.get_width();
    }
}


}}





