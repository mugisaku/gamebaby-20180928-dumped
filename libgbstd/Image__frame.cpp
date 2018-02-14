#include"libgbstd/image.hpp"
#include<cstring>




namespace gbstd{
namespace images{


constexpr color_index
palette[] =
{
  0,
  predefined::blue,
  predefined::white,
  predefined::light_gray,
};


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
image::
draw_frame_top(int  x, int  y, int  w) noexcept
{
    for(int  yy = 0;  yy < 8;  yy += 1){
    for(int  xx = 0;  xx < 8;  xx += 1){
      auto  v = top_piece[yy][xx];

        if(v)
        {
          draw_dot(point(x+xx    ,y+yy),palette[v]);
          draw_dot(point(x+w-1-xx,y+yy),palette[v]);
        }
    }}


  x +=  8;
  w -= 16;

  draw_hline(point(x,y++),w,palette[3]);
  draw_hline(point(x,y++),w,palette[2]);
  draw_hline(point(x,y++),w,palette[3]);
  draw_hline(point(x,y++),w,palette[1]);
  draw_hline(point(x,y++),w,palette[1]);
  draw_hline(point(x,y++),w,palette[1]);
  draw_hline(point(x,y++),w,palette[1]);
  draw_hline(point(x,y  ),w,palette[1]);
}


void
image::
draw_frame_body(int  x, int  y, int  w, int  h) noexcept
{
  draw_vline(point(x+0,y),h,palette[3]);
  draw_vline(point(x+1,y),h,palette[2]);
  draw_vline(point(x+2,y),h,palette[3]);

  draw_vline(point(x+w-1-0,y),h,palette[3]);
  draw_vline(point(x+w-1-1,y),h,palette[2]);
  draw_vline(point(x+w-1-2,y),h,palette[3]);


    while(h--)
    {
      draw_hline(point(x+3,y++),w-6,palette[1]);
    }
}


void
image::
draw_frame_bottom(int  x, int  y, int  w) noexcept
{
    for(int  yy = 0;  yy < 8;  yy += 1){
    for(int  xx = 0;  xx < 8;  xx += 1){
      auto  v = bottom_piece[yy][xx];

        if(v)
        {
          draw_dot(point(x+xx    ,y+yy),palette[v]);
          draw_dot(point(x+w-1-xx,y+yy),palette[v]);
        }
    }}


  x +=  8;
  w -= 16;

  draw_hline(point(x,y++),w,palette[1]);
  draw_hline(point(x,y++),w,palette[1]);
  draw_hline(point(x,y++),w,palette[1]);
  draw_hline(point(x,y++),w,palette[1]);
  draw_hline(point(x,y++),w,palette[3]);
  draw_hline(point(x,y++),w,palette[2]);
  draw_hline(point(x,y++),w,palette[2]);
  draw_hline(point(x,y  ),w,palette[3]);
}


void
image::
draw_frame(int  x, int  y, int  w, int  h) noexcept
{
  draw_frame_top(   x,y    ,w     );
  draw_frame_body(  x,y  +8,w,h-16);
  draw_frame_bottom(x,y+h-8,w     );
}




}}





