#include"libgbstd/image.hpp"
#include"libgbstd/text.hpp"
#include<cstring>



namespace gbstd{
namespace images{




void
image::
draw_dot(color  i, int  x, int  y) noexcept
{
    if(i)
    {
      m_pixels[(m_width*y)+x].color = i;
    }
}


void
image::
draw_dot_safely(color  i, int  x, int  y) noexcept
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
draw_vline(color  i, int  x, int  y, int  l) noexcept
{
    while(l--)
    {
      draw_dot(i,x,y);

      y += 1;
    }
}


void
image::
draw_vline_safely(color  i, int  x, int  y, int  l) noexcept
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
draw_hline(color  i, int  x, int  y, int  l, int  interval) noexcept
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
draw_hline_safely(color  i, int  x, int  y, int  l) noexcept
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
draw_line(color  i, int  x0, int  y0, int  x1, int  y1) noexcept
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
draw_rectangle(color  i, int  x, int  y, int  w, int  h) noexcept
{
    if(i)
    {
      draw_hline(i,x,y    ,w);
      draw_hline(i,x,y+h-1,w);

      draw_vline(i,x    ,y+1,h-2);
      draw_vline(i,x+w-1,y+1,h-2);
    }
}


void
image::
draw_rectangle_safely(color  i, int  x, int  y, int  w, int  h) noexcept
{
  draw_hline_safely(i,x,y    ,w);
  draw_hline_safely(i,x,y+h-1,w);

  draw_vline_safely(i,x    ,y+1,h-2);
  draw_vline_safely(i,x+w-1,y+1,h-2);
}


void
image::
fill_rectangle(color  i, int  x, int  y, int  w, int  h) noexcept
{
    while(h--)
    {
      draw_hline(i,x,y,w);

      ++y;
    }
}


void
image::
fill_rectangle_safely(color  i, int  x, int  y, int  w, int  h) noexcept
{
    while(h--)
    {
      draw_hline_safely(i,x,y,w);

      ++y;
    }
}




void
image::
draw_doubleline_rectangle(color  in, color  out, int  x, int  y, int  w, int  h) noexcept
{
  draw_rectangle(out,x  ,y  ,w  ,h  );
  draw_rectangle( in,x+1,y+1,w-2,h-2);
}


void
image::
draw_stripe_rectangle(color  first, color  second, int  interval, int  x, int  y, int  w, int  h) noexcept
{
    if(interval)
    {
        for(;;)
        {
            for(int  n = 0;  n < interval;  ++n)
            {
                if(!h--)
                {
                  return;
                }


              draw_hline(first,x,y++,w);
            }


            for(int  n = 0;  n < interval;  ++n)
            {
                if(!h--)
                {
                  return;
                }


              draw_hline(second,x,y++,w);
            }
        }
    }
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
START:
  auto  p = table[c];

    if(p)
    {
        for(int  yy = 0;  yy < h;  yy += 1)
        {
          auto   dst = &get_pixel(x,y+yy);
          auto  code = *p++;

            for(int  xx = 0;  xx < w;  xx += 1)
            {
              auto  color = style.get_color(code>>14);

                if(color)
                {
                  dst->color = color;
                }


               dst  += 1;
              code <<= 2;
            }
        }
    }

  else
    {
//printf("0x%04X\n",c);
      c = '!';

      goto START;
    }
}




void
image::
draw_text(gbstd::string_view  sv, const text_style&  style, int  x, int  y) noexcept
{
  utf8_decoder  dec(sv);

    while(dec)
    {
      auto  c = static_cast<char16_t>(dec());

        if(!c)
        {
          break;
        }


      draw_character(c,style,x,y);

      x += w;
    }
}




void
image::
draw_text(gbstd::u16string_view  sv, const text_style&  style, int  x, int  y) noexcept
{
    for(auto  c: sv)
    {
      draw_character(c,style,x,y);

      x += w;
    }
}




void
image::
draw_character_safely(char16_t  c, const text_style&  style, int  x, int  y) noexcept
{
START:
  auto  p = table[c];

    if(p)
    {
        for(int  yy = 0;  yy < h;  yy += 1)
        {
          auto  code = *p++;

          int  yyy = y+yy;

            if((yyy >=       0) &&
               (yyy < m_height))
            {
              auto   begin = &get_pixel(      0,yyy);
              auto     end = &get_pixel(m_width,yyy);

              auto   dst = &get_pixel(x,yyy);
                
                for(int  xx = 0;  xx < w;  xx += 1)
                {
                    if((dst >= begin) &&
                       (dst <    end))
                    {
                      auto  color = style.get_color(code>>14);

                        if(color)
                        {
                          dst->color = color;
                        }
                    }


                   dst  += 1;
                  code <<= 2;
                }
            }
        }
    }

  else
    {
//printf("0x%04X\n",c);
      c = '!';

      goto START;
    }
}




void
image::
draw_text_safely(gbstd::string_view  sv, const text_style&  style, int  x, int  y) noexcept
{
  utf8_decoder  dec(sv);

    while(dec)
    {
      auto  c = static_cast<char16_t>(dec());

        if(!c)
        {
          break;
        }


      draw_character_safely(c,style,x,y);

      x += w;
    }
}




void
image::
draw_text_safely(gbstd::u16string_view  sv, const text_style&  style, int  x, int  y) noexcept
{
    for(auto  c: sv)
    {
      draw_character_safely(c,style,x,y);

      x += w;
    }
}




}}





