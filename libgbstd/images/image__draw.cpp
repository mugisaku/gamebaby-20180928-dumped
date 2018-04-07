#include"libgbstd/image.hpp"
#include"libgbstd/text.hpp"
#include<cstring>



namespace gbstd{
namespace images{




void
image::
draw_dot(color_index  i, int  x, int  y) noexcept
{
    if(i)
    {
      m_pixels[(m_width*y)+x].color_index = i;
    }
}


void
image::
draw_dot_safely(color_index  i, int  x, int  y) noexcept
{
    if((x >=        0) &&
       (x <   m_width) &&
       (y >=        0) &&
       (y <  m_height))
    {
      draw_dot(i,x,y);
    }
}


void
image::
draw_vline(color_index  i, int  x, int  y, int  l) noexcept
{
    while(l--)
    {
      draw_dot(i,x,y);

      y += 1;
    }
}


void
image::
draw_vline_safely(color_index  i, int  x, int  y, int  l) noexcept
{
    if((x >=       0) &&
       (x <  m_width))
    {
        if(y < 0)
        {
          l += y    ;
               y = 0;
        }

      else
        if(y+l >= m_height)
        {
          l = m_height-y;
        }


        if(l > 0)
        {
            while(l--)
            {
              draw_dot(i,x,y);

              y += 1;
            }
        }
    }
}


void
image::
draw_hline(color_index  i, int  x, int  y, int  l, int  interval) noexcept
{
  int  n = interval;

    while(l--)
    {
        if(n)
        {
          --n;
        }

      else
        {
          draw_dot(i,x,y);

          n = interval;
        }


      x += 1;
    }
}


void
image::
draw_hline_safely(color_index  i, int  x, int  y, int  l) noexcept
{
    if((y >=        0) &&
       (y <  m_height))
    {
        if(x < 0)
        {
          l += x    ;
               x = 0;
        }

      else
        if(x+l >= m_width)
        {
          l = m_width-x;
        }


        if(l > 0)
        {
            while(l--)
            {
              draw_dot(i,x,y);

              x += 1;
            }
        }
    }
}


void
image::
draw_line(color_index  i, int  x0, int  y0, int  x1, int  y1) noexcept
{
  line_maker  lnmk(x0,y0,x1,y1);

    for(;;)
    {
      draw_dot(i,lnmk.get_x(),lnmk.get_y());

        if(!lnmk.get_distance())
        {
          break;
        }


      lnmk.step();
    }
}


void
image::
draw_rectangle(color_index  i, int  x, int  y, int  w, int  h) noexcept
{
  draw_hline(i,x,y    ,w);
  draw_hline(i,x,y+h-1,w);

  draw_vline(i,x    ,y+1,h-2);
  draw_vline(i,x+w-1,y+1,h-2);
}


void
image::
draw_rectangle_safely(color_index  i, int  x, int  y, int  w, int  h) noexcept
{
  draw_hline_safely(i,x,y    ,w);
  draw_hline_safely(i,x,y+h-1,w);

  draw_vline_safely(i,x    ,y+1,h-2);
  draw_vline_safely(i,x+w-1,y+1,h-2);
}


void
image::
fill_rectangle(color_index  i, int  x, int  y, int  w, int  h) noexcept
{
    while(h--)
    {
      draw_hline(i,x,y,w);

      ++y;
    }
}




void
image::
draw_doubleline_rectangle(color_index  in, color_index  out, int  x, int  y, int  w, int  h) noexcept
{
  draw_rectangle(out,x  ,y  ,w  ,h  );
  draw_rectangle( in,x+1,y+1,w-2,h-2);
}




namespace{


constexpr int  w =  8;
constexpr int  h = 16;

struct
glyph_data
{
  uint16_t  unicode;

  uint16_t  data[h];

};


static
const glyph_data
data_table[] =
{
#include"libgbstd/images/font_data.cpp"
};


static
const uint16_t*
table[0x10000] = {0};


struct
initializer
{
  initializer() noexcept;

} init;


initializer::
initializer() noexcept
{
    for(auto&  gly: data_table)
    {
      table[gly.unicode] = gly.data;
    }
}


}


void
image::
draw_character(char16_t  c, const text_style&  style, int  x, int  y) noexcept
{
  auto  p = table[c];

    if(p)
    {
        for(int  yy = 0;  yy < h;  yy += 1)
        {
          auto   dst = &get_pixel(x,y+yy);
          auto  code = *p++;

            for(int  xx = 0;  xx < w;  xx += 1)
            {
              auto  i = style.get_color_index(code>>14);

              dst++->color_index = i;

              code <<= 2;
            }
        }
    }
}




void
image::
draw_text(gbstd::string_view  sv, const text_style&  style, int  x, int  y) noexcept
{
  utf8_decoder  dec(sv);

    while(dec)
    {
      draw_character(static_cast<char16_t>(dec()),style,x,y);

      x += w;
    }
}




void
image::
draw_text(gbstd::u16string_view  sv, const text_style&  style, int  x, int  y) noexcept
{
    for(auto  c: sv)
    {
      draw_character(static_cast<char16_t>(c),style,x,y);

      x += w;
    }
}




}}





