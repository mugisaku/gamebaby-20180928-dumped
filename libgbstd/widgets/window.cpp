#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




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
draw_frame() noexcept
{
  int  w = m_root.get_width() +8;
  int  h = m_root.get_height()+8;

  draw_frame_top(   m_image,0,0    ,w     ,m_pixels);
  draw_frame_body(  m_image,0,0  +8,w,h-16,m_pixels);
  draw_frame_bottom(m_image,0,0+h-8,w     ,m_pixels);
}


void
window::
update() noexcept
{
  m_root.update();

  int  w = m_root.get_width() +8;
  int  h = m_root.get_height()+8;

    if((w != m_image.get_width()) ||
       (h != m_image.get_height()))
    {
      m_image.resize(w,h);

      draw_frame();

      m_root.render(m_image,8,8);
    }

  else
    {
      m_root.render(m_image,8,8);
    }
}




}}




