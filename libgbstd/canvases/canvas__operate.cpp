#include"libgbstd/canvas.hpp"
#include<cstring>




namespace gbstd{
namespace canvases{




void
canvas::
paste(point  pt, bool  layer) noexcept
{
  try_to_push_nonsolid_record();


  rectangle  src_rect = m_clipped_image.get_rectangle();
  rectangle  dst_rect(pt,m_image->get_width(),m_image->get_height());

  images::transform(src_rect,dst_rect);

    if(src_rect.w)
    {
        for(int  y = 0;  y < src_rect.h;  y += 1){
        for(int  x = 0;  x < src_rect.w;  x += 1){
          auto  pix = m_clipped_image.get_const_pixel(src_rect.x+x,src_rect.y+y);

            if(!layer || pix.color)
            {
              modify_dot(pix.color,dst_rect.x+x,dst_rect.y+y);
            }
        }}
    }


  try_to_push_solid_record();
}




void
canvas::
revolve() noexcept
{
  image  tmp = *m_image;

  const int  w = std::min(m_operation_rect.w,m_operation_rect.h);

  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < w;  ++yy){
    for(int  xx = 0;  xx < w;  ++xx){
      modify_dot(tmp.get_pixel(m_operation_rect.x+xx,m_operation_rect.y+yy),m_operation_rect.x+w-1-yy,m_operation_rect.y+xx);
    }}


  try_to_push_solid_record();
}


void
canvas::
reverse_horizontally() noexcept
{
  image  tmp = *m_image;

  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < m_operation_rect.h;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w;  ++xx){
      modify_dot(tmp.get_pixel(m_operation_rect.x+xx,m_operation_rect.y+yy),m_operation_rect.x+xx,m_operation_rect.y+m_operation_rect.h-1-yy);
    }}


  try_to_push_solid_record();
}


void
canvas::
reverse_vertically() noexcept
{
  image  tmp = *m_image;

  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < m_operation_rect.h;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w;  ++xx){
      modify_dot(tmp.get_pixel(m_operation_rect.x+xx,m_operation_rect.y+yy),m_operation_rect.x+m_operation_rect.w-1-xx,m_operation_rect.y+yy);
    }}


  try_to_push_solid_record();
}


void
canvas::
mirror_vertically() noexcept
{
  try_to_push_nonsolid_record();

    for(int  yy = 0;  yy < m_operation_rect.h  ;  ++yy){
    for(int  xx = 0;  xx < m_operation_rect.w/2;  ++xx){
      auto  pix = m_image->get_pixel(m_operation_rect.x+xx,m_operation_rect.y+yy);

      modify_dot(pix,m_operation_rect.x+m_operation_rect.w-1-xx,m_operation_rect.y+yy);
    }}


  try_to_push_solid_record();
}




void
canvas::
shift_up(bool  rotate) noexcept
{
  image  tmp = *m_image;

  try_to_push_nonsolid_record();

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


  try_to_push_solid_record();
}


void
canvas::
shift_left(bool  rotate) noexcept
{
  image  tmp = *m_image;

  try_to_push_nonsolid_record();

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


  try_to_push_solid_record();
}


void
canvas::
shift_right(bool  rotate) noexcept
{
  image  tmp = *m_image;

  try_to_push_nonsolid_record();

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


  try_to_push_solid_record();
}


void
canvas::
shift_down(bool  rotate) noexcept
{
  image  tmp = *m_image;

  try_to_push_nonsolid_record();

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


  try_to_push_solid_record();
}




}}




