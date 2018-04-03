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




window::
window(uint32_t  n, int  x, int  y) noexcept:
m_number(n),
m_point(x,y)
{
  m_container.set_relative_point(point(8,8));

  m_container.set_window(this);
}




void
window::
draw_frame() noexcept
{
  int  w = get_width() ;
  int  h = get_height();

  draw_frame_top(   m_image,0,0    ,w     ,m_pixels);
  draw_frame_body(  m_image,0,0  +8,w,h-16,m_pixels);
  draw_frame_bottom(m_image,0,0+h-8,w     ,m_pixels);
}


bool
window::
test_by_point(int  x, int  y) const noexcept
{
  return((x >= (m_point.x                     )) &&
         (y >= (m_point.y                     )) &&
         (x <  (m_point.x+m_image.get_width() )) &&
         (y <  (m_point.y+m_image.get_height())));
}


bool
window::
is_image_modified() noexcept
{
    if(m_modified_flag)
    {
      m_modified_flag = false;

      return true;
    }


  return false;
}


void
window::
react() noexcept
{
  m_container.reform_if_needed(point());

  auto  pt = ctrl.get_point()-m_point;

    if(ctrl.did_mouse_moved())
    {
        if(!m_current)
        {
          m_current = m_container.scan_by_point(pt.x,pt.y);

            if(m_current)
            {
              m_current->do_when_cursor_got_in();
            }
        }

      else
        {
            if(!m_current->test_by_point(pt.x,pt.y))
            {
              m_current->do_when_cursor_got_out();

              m_current = m_container.scan_by_point(pt.x,pt.y);

                if(m_current)
                {
                  m_current->do_when_cursor_got_in();
                }
            }
        }
    }


    if(m_current && ctrl.did_mouse_acted())
    {
      pt -= m_current->get_absolute_point();

      m_current->do_when_mouse_acted(pt.x,pt.y);
    }


  update();
}


void
window::
update() noexcept
{
    if(m_container.is_needed_to_redraw())
    {
      m_container.reform_if_needed(point());

      int  w = m_container.get_width() +8;
      int  h = m_container.get_height()+8;

        if((w != m_image.get_width()) ||
           (h != m_image.get_height()))
        {
          m_image.resize(w,h);

          draw_frame();
        }


      m_container.redraw(m_image);

      m_modified_flag = true;
    }
}




}}




