#include"libgbstd/window.hpp"



namespace gbstd{
namespace windows{




namespace{


void
draw_frame_top(const window_style&  style, image_cursor  cur, int  x, int  y, int  w) noexcept
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
          cur.draw_dot(style.colors[v],x+xx    ,y+yy);
          cur.draw_dot(style.colors[v],x+w-1-xx,y+yy);
        }
    }}


  x +=  8;
  w -= 16;

  cur.draw_hline(style.colors[3],x,y++,w);
  cur.draw_hline(style.colors[2],x,y++,w);
  cur.draw_hline(style.colors[3],x,y++,w);
  cur.draw_hline(style.colors[1],x,y++,w);
  cur.draw_hline(style.colors[1],x,y++,w);
  cur.draw_hline(style.colors[1],x,y++,w);
  cur.draw_hline(style.colors[1],x,y++,w);
  cur.draw_hline(style.colors[1],x,y++,w);
}


void
draw_frame_top_with_header(const window_style&  style, image_cursor  cur, int  x, int  y, int  w) noexcept
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
          cur.draw_dot(style.colors[v],x+xx    ,y+yy);
          cur.draw_dot(style.colors[v],x+w-1-xx,y+yy);
        }
    }}


  x +=  8;
  w -= 16;

  cur.draw_hline(style.colors[3],x,y++,w);
  cur.draw_hline(style.colors[2],x,y++,w);
  cur.draw_hline(style.colors[2],x,y++,w);
  cur.draw_hline(style.colors[2],x,y++,w);
  cur.draw_hline(style.colors[2],x,y++,w);
  cur.draw_hline(style.colors[2],x,y++,w);
  cur.draw_hline(style.colors[2],x,y++,w);
  cur.draw_hline(style.colors[2],x,y++,w);

  cur.draw_hline(style.colors[2],x,y++,w);
  cur.draw_hline(style.colors[2],x,y++,w);
  cur.draw_hline(style.colors[2],x,y++,w);
  cur.draw_hline(style.colors[2],x,y++,w);
  cur.draw_hline(style.colors[2],x,y++,w);
  cur.draw_hline(style.colors[2],x,y++,w);
  cur.draw_hline(style.colors[2],x,y++,w);
  cur.draw_hline(style.colors[3],x,y++,w);
}


void
draw_frame_body(const window_style&  style, image_cursor  cur, int  x, int  y, int  w, int  h) noexcept
{
  cur.draw_vline(style.colors[3],x+0,y,h);
  cur.draw_vline(style.colors[2],x+1,y,h);
  cur.draw_vline(style.colors[3],x+2,y,h);

  cur.draw_vline(style.colors[3],x+w-1-0,y,h);
  cur.draw_vline(style.colors[2],x+w-1-1,y,h);
  cur.draw_vline(style.colors[3],x+w-1-2,y,h);


    while(h--)
    {
      cur.draw_hline(style.colors[1],x+3,y++,w-6);
    }
}


void
draw_frame_bottom(const window_style&  style, image_cursor  cur, int  x, int  y, int  w) noexcept
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
          cur.draw_dot(style.colors[v],x+xx    ,y+yy);
          cur.draw_dot(style.colors[v],x+w-1-xx,y+yy);
        }
    }}


  x +=  8;
  w -= 16;

  cur.draw_hline(style.colors[1],x,y++,w);
  cur.draw_hline(style.colors[1],x,y++,w);
  cur.draw_hline(style.colors[1],x,y++,w);
  cur.draw_hline(style.colors[1],x,y++,w);
  cur.draw_hline(style.colors[3],x,y++,w);
  cur.draw_hline(style.colors[2],x,y++,w);
  cur.draw_hline(style.colors[2],x,y++,w);
  cur.draw_hline(style.colors[3],x,y  ,w);
}


}




void
window::
draw_frame(const window_style&  style, image_cursor  cur) noexcept
{
  int  w = get_width() ;
  int  h = get_height();

  bool  hdr = m_state&flags::header;

    if(hdr){draw_frame_top_with_header(style,cur,0,0,w);}
  else     {draw_frame_top(            style,cur,0,0,w);}


  draw_frame_bottom(style,cur,0,h-8,w);

  draw_frame_body(style,cur,0,hdr? 16:8,w,m_root->get_height());


  unset_flag(flags::needed_to_redraw_frame);
}


}}




