#include"libgbstd/window.hpp"
#include<cstring>




namespace gbstd{
namespace windows{


namespace{


constexpr uint8_t
top_piece[8][8] =
{
  {0,0,3,3,3,3,3,3},
  {0,3,3,2,2,2,2,2},
  {3,3,2,2,3,3,3,3},
  {3,2,2,3,3,1,1,1},
  {3,2,3,3,1,1,1,1},
  {3,2,3,1,1,1,1,1},
  {3,2,3,1,1,1,1,1},
  {3,2,3,1,1,1,1,1},

};


constexpr uint8_t
bottom_piece[8][8] =
{
  {3,2,3,1,1,1,1,1},
  {3,2,3,1,1,1,1,1},
  {3,2,3,3,1,1,1,1},
  {3,2,2,3,3,1,1,1},
  {3,2,2,2,3,3,3,3},
  {3,3,2,2,2,2,2,2},
  {0,3,3,2,2,2,2,2},
  {0,0,3,3,3,3,3,3},
};


void
draw_frame_top(image&  dst, int  x, int  y, int  w, pixel const*  pixels) noexcept
{
    for(int  yy = 0;  yy < 8;  yy += 1){
    for(int  xx = 0;  xx < 8;  xx += 1){
      auto  v = top_piece[yy][xx];

        if(v)
        {
          dst.draw_dot(pixels[v],x+xx    ,y+yy);
          dst.draw_dot(pixels[v],x+w-1-xx,y+yy);
        }
    }}


  x +=  8;
  w -= 16;

  dst.draw_hline(pixels[3],x,y++,w);
  dst.draw_hline(pixels[2],x,y++,w);
  dst.draw_hline(pixels[3],x,y++,w);
  dst.draw_hline(pixels[1],x,y++,w);
  dst.draw_hline(pixels[1],x,y++,w);
  dst.draw_hline(pixels[1],x,y++,w);
  dst.draw_hline(pixels[1],x,y++,w);
  dst.draw_hline(pixels[1],x,y++,w);
}


void
draw_frame_body(image&  dst, int  x, int  y, int  w, int  h, pixel const*  pixels) noexcept
{
  dst.draw_vline(pixels[3],x+0,y,h);
  dst.draw_vline(pixels[2],x+1,y,h);
  dst.draw_vline(pixels[3],x+2,y,h);

  dst.draw_vline(pixels[3],x+w-1-0,y,h);
  dst.draw_vline(pixels[2],x+w-1-1,y,h);
  dst.draw_vline(pixels[3],x+w-1-2,y,h);


    while(h--)
    {
      dst.draw_hline(pixels[1],x+3,y++,w-6);
    }
}


void
draw_frame_bottom(image&  dst, int  x, int  y, int  w, pixel const*  pixels) noexcept
{
    for(int  yy = 0;  yy < 8;  yy += 1){
    for(int  xx = 0;  xx < 8;  xx += 1){
      auto  v = bottom_piece[yy][xx];

        if(v)
        {
          dst.draw_dot(pixels[v],x+xx    ,y+yy);
          dst.draw_dot(pixels[v],x+w-1-xx,y+yy);
        }
    }}


  x +=  8;
  w -= 16;

  dst.draw_hline(pixels[1],x,y++,w);
  dst.draw_hline(pixels[1],x,y++,w);
  dst.draw_hline(pixels[1],x,y++,w);
  dst.draw_hline(pixels[1],x,y++,w);
  dst.draw_hline(pixels[3],x,y++,w);
  dst.draw_hline(pixels[2],x,y++,w);
  dst.draw_hline(pixels[2],x,y++,w);
  dst.draw_hline(pixels[3],x,y  ,w);
}


}


void
window::
draw_frame(image&  dst, point  offset) const noexcept
{
  int  x = get_base_point().x+offset.x;
  int  y = get_base_point().y+offset.y;
  int  w = m_width;
  int  h = m_height;

  draw_frame_top(   dst,x,y    ,w     ,m_pixels);
  draw_frame_body(  dst,x,y  +8,w,h-16,m_pixels);
  draw_frame_bottom(dst,x,y+h-8,w     ,m_pixels);
}




}}





