#include"libgbstd/canvas.hpp"
#include<cstring>




namespace gbstd{
namespace canvases{


void
canvas::
revolve() noexcept
{
  image  tmp = *m_image;

  const int  w = std::min(m_operation_rect.w,m_operation_rect.h);

  m_recorder.push(false);

    for(int  yy = 0;  yy < w;  ++yy){
    for(int  xx = 0;  xx < w;  ++xx){
      modify_dot(tmp.get_pixel(m_operation_rect.x+xx,m_operation_rect.y+yy),m_operation_rect.x+w-1-yy,m_operation_rect.y+xx);
    }}


  m_recorder.push(true);

  need_to_redraw();

    if(m_callback)
    {
      m_callback(*this);
    }
}


void
canvas::
reverse_horizontally() noexcept
{
  image  tmp = *m_image;

  m_recorder.push(false);

    for(int  yy = 0;  yy < m_operation_rect.h;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w;  ++xx){
      modify_dot(tmp.get_pixel(m_operation_rect.x+xx,m_operation_rect.y+yy),m_operation_rect.x+xx,m_operation_rect.y+m_operation_rect.h-1-yy);
    }}


  m_recorder.push(true);

  need_to_redraw();

    if(m_callback)
    {
      m_callback(*this);
    }
}


void
canvas::
reverse_vertically() noexcept
{
  image  tmp = *m_image;

  m_recorder.push(false);

    for(int  yy = 0;  yy < m_operation_rect.h;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w;  ++xx){
      modify_dot(tmp.get_pixel(m_operation_rect.x+xx,m_operation_rect.y+yy),m_operation_rect.x+m_operation_rect.w-1-xx,m_operation_rect.y+yy);
    }}


  m_recorder.push(true);

  need_to_redraw();

    if(m_callback)
    {
      m_callback(*this);
    }
}


void
canvas::
mirror_vertically() noexcept
{
  m_recorder.push(false);

    for(int  yy = 0;  yy < m_operation_rect.h  ;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w/2;  ++xx){
      auto  pix = m_image->get_pixel(m_operation_rect.x+xx,m_operation_rect.y+yy);

      modify_dot(pix,m_operation_rect.x+m_operation_rect.w-1-xx,m_operation_rect.y+yy);
    }}


  m_recorder.push(true);

  need_to_redraw();

    if(m_callback)
    {
      m_callback(*this);
    }
}




void
canvas::
shift_up(bool  rotate) noexcept
{
  image  tmp = *m_image;

  m_recorder.push(false);

    for(int  yy = 0;  yy < m_operation_rect.h-1;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w  ;  ++xx){
      modify_dot(tmp.get_pixel(m_operation_rect.x+xx,m_operation_rect.y+yy+1),m_operation_rect.x+xx,m_operation_rect.y+yy);
    }}


    if(rotate)
    {
        for(int  xx = 0;  xx < m_operation_rect.w;  ++xx)
        {
          modify_dot(tmp.get_pixel(m_operation_rect.x+xx,m_operation_rect.y),m_operation_rect.x+xx,m_operation_rect.y+m_operation_rect.h-1);
        }
    }


  m_recorder.push(true);

  need_to_redraw();

    if(m_callback)
    {
      m_callback(*this);
    }
}


void
canvas::
shift_left(bool  rotate) noexcept
{
  image  tmp = *m_image;

  m_recorder.push(false);

    for(int  yy = 0;  yy < m_operation_rect.h  ;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w-1;  ++xx){
      modify_dot(tmp.get_pixel(m_operation_rect.x+xx+1,m_operation_rect.y+yy),m_operation_rect.x+xx,m_operation_rect.y+yy);
    }}


    if(rotate)
    {
        for(int  yy = 0;  yy < m_operation_rect.h;  ++yy)
        {
          modify_dot(tmp.get_pixel(m_operation_rect.x,m_operation_rect.y+yy),m_operation_rect.x+m_operation_rect.w-1,m_operation_rect.y+yy);
        }
    }


  m_recorder.push(true);

  need_to_redraw();

    if(m_callback)
    {
      m_callback(*this);
    }
}


void
canvas::
shift_right(bool  rotate) noexcept
{
  image  tmp = *m_image;

  m_recorder.push(false);

    for(int  yy = 0;  yy < m_operation_rect.h;  ++yy){
    for(int  xx = 1;  xx < m_operation_rect.w;  ++xx){
      modify_dot(tmp.get_pixel(m_operation_rect.x+xx-1,m_operation_rect.y+yy),m_operation_rect.x+xx,m_operation_rect.y+yy);
    }}


    if(rotate)
    {
        for(int  yy = 0;  yy < m_operation_rect.h;  ++yy)
        {
          modify_dot(tmp.get_pixel(m_operation_rect.x+m_operation_rect.w-1,m_operation_rect.y+yy),m_operation_rect.x,m_operation_rect.y+yy);
        }
    }


  m_recorder.push(true);

  need_to_redraw();

    if(m_callback)
    {
      m_callback(*this);
    }
}


void
canvas::
shift_down(bool  rotate) noexcept
{
  image  tmp = *m_image;

  m_recorder.push(false);

    for(int  yy = 1;  yy < m_operation_rect.h;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w;  ++xx){
      modify_dot(tmp.get_pixel(m_operation_rect.x+xx,m_operation_rect.y+yy-1),m_operation_rect.x+xx,m_operation_rect.y+yy);
    }}


    if(rotate)
    {
        for(int  xx = 0;  xx < m_operation_rect.w;  ++xx)
        {
          modify_dot(tmp.get_pixel(m_operation_rect.x+xx,m_operation_rect.y+m_operation_rect.h-1),m_operation_rect.x+xx,m_operation_rect.y);
        }
    }


  m_recorder.push(true);

  need_to_redraw();

    if(m_callback)
    {
      m_callback(*this);
    }
}




}}




