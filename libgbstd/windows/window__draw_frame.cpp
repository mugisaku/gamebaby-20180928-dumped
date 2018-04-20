#include"libgbstd/window.hpp"



namespace gbstd{
namespace windows{




namespace{


void
draw_frame_top(image&  dst, int  x, int  y, int  w, const color*  colors) noexcept
{
  constexpr uint8_t  piece[8][8] =
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


    for(int  yy = 0;  yy < 8;  yy += 1){
    for(int  xx = 0;  xx < 8;  xx += 1){
      auto  v = piece[yy][xx];

        if(v)
        {
          dst.draw_dot(colors[v],x+xx    ,y+yy);
          dst.draw_dot(colors[v],x+w-1-xx,y+yy);
        }
    }}


  x +=  8;
  w -= 16;

  dst.draw_hline(colors[3],x,y++,w);
  dst.draw_hline(colors[2],x,y++,w);
  dst.draw_hline(colors[3],x,y++,w);
  dst.draw_hline(colors[1],x,y++,w);
  dst.draw_hline(colors[1],x,y++,w);
  dst.draw_hline(colors[1],x,y++,w);
  dst.draw_hline(colors[1],x,y++,w);
  dst.draw_hline(colors[1],x,y++,w);
}


void
draw_frame_top_with_header(image&  dst, int  x, int  y, int  w, const color*  colors) noexcept
{
  constexpr uint8_t  piece[16][8] =
  {
    {0,0,3,3,3,3,3,3},
    {0,3,3,2,2,2,2,2},
    {3,3,2,2,2,2,2,2},
    {3,2,2,2,2,2,2,2},
    {3,2,2,2,2,2,2,2},
    {3,2,2,2,2,2,2,2},
    {3,2,2,2,2,2,2,2},
    {3,2,2,2,2,2,2,2},

    {3,2,2,2,2,2,2,2},
    {3,2,2,2,2,2,2,2},
    {3,2,2,2,2,2,2,2},
    {3,2,2,2,2,2,2,2},
    {3,2,2,2,2,2,2,2},
    {3,2,2,2,2,2,2,2},
    {3,2,2,2,2,2,2,2},
    {3,2,3,3,3,3,3,3},

  };


    for(int  yy = 0;  yy < 16;  yy += 1){
    for(int  xx = 0;  xx <  8;  xx += 1){
      auto  v = piece[yy][xx];

        if(v)
        {
          dst.draw_dot(colors[v],x+xx    ,y+yy);
          dst.draw_dot(colors[v],x+w-1-xx,y+yy);
        }
    }}


  x +=  8;
  w -= 16;

  dst.draw_hline(colors[3],x,y++,w);
  dst.draw_hline(colors[2],x,y++,w);
  dst.draw_hline(colors[2],x,y++,w);
  dst.draw_hline(colors[2],x,y++,w);
  dst.draw_hline(colors[2],x,y++,w);
  dst.draw_hline(colors[2],x,y++,w);
  dst.draw_hline(colors[2],x,y++,w);
  dst.draw_hline(colors[2],x,y++,w);

  dst.draw_hline(colors[2],x,y++,w);
  dst.draw_hline(colors[2],x,y++,w);
  dst.draw_hline(colors[2],x,y++,w);
  dst.draw_hline(colors[2],x,y++,w);
  dst.draw_hline(colors[2],x,y++,w);
  dst.draw_hline(colors[2],x,y++,w);
  dst.draw_hline(colors[2],x,y++,w);
  dst.draw_hline(colors[3],x,y++,w);
}


void
draw_frame_body(image&  dst, int  x, int  y, int  w, int  h, const color*  colors) noexcept
{
  dst.draw_vline(colors[3],x+0,y,h);
  dst.draw_vline(colors[2],x+1,y,h);
  dst.draw_vline(colors[3],x+2,y,h);

  dst.draw_vline(colors[3],x+w-1-0,y,h);
  dst.draw_vline(colors[2],x+w-1-1,y,h);
  dst.draw_vline(colors[3],x+w-1-2,y,h);


    while(h--)
    {
      dst.draw_hline(colors[1],x+3,y++,w-6);
    }
}


void
draw_frame_bottom(image&  dst, int  x, int  y, int  w, const color*  colors) noexcept
{
  constexpr uint8_t  piece[8][8] =
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


    for(int  yy = 0;  yy < 8;  yy += 1){
    for(int  xx = 0;  xx < 8;  xx += 1){
      auto  v = piece[yy][xx];

        if(v)
        {
          dst.draw_dot(colors[v],x+xx    ,y+yy);
          dst.draw_dot(colors[v],x+w-1-xx,y+yy);
        }
    }}


  x +=  8;
  w -= 16;

  dst.draw_hline(colors[1],x,y++,w);
  dst.draw_hline(colors[1],x,y++,w);
  dst.draw_hline(colors[1],x,y++,w);
  dst.draw_hline(colors[1],x,y++,w);
  dst.draw_hline(colors[3],x,y++,w);
  dst.draw_hline(colors[2],x,y++,w);
  dst.draw_hline(colors[2],x,y++,w);
  dst.draw_hline(colors[3],x,y  ,w);
}


}




void
window::
draw_frame() noexcept
{
  int  w = get_width() ;
  int  h = get_height();

  bool  hdr = m_state&flags::header;

    if(hdr){draw_frame_top_with_header(get_image(),0,0,w,m_colors);}
  else     {draw_frame_top(            get_image(),0,0,w,m_colors);}


  draw_frame_bottom(get_image(),0,h-8,w,m_colors);

  draw_frame_body(get_image(),0,hdr? 16:8,w,m_root->get_height(),m_colors);
}


}}




