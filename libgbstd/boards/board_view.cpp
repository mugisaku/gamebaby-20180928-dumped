#include"libgbstd/board.hpp"
#include"libgbstd/space.hpp"
#include"libgbstd/text.hpp"




namespace gbstd{
namespace boards{




void
board_view::
chase_object(const spaces::object&  obj, int  speed) noexcept
{
  auto&  base_point = obj.get_base_point();

  auto  dst_point = point(static_cast<int>(base_point.x)-(m_width /2),
                          static_cast<int>(base_point.y)-(m_height/2));

  auto  screen_w = m_board->get_image_width() ;
  auto  screen_h = m_board->get_image_height();

    if(dst_point.x < 0)
    {
      dst_point.x = 0;
    }

  else
    if(dst_point.x > (screen_w-m_width))
    {
      dst_point.x = (screen_w-m_width);
    }


    if(dst_point.y < 0)
    {
      dst_point.y = 0;
    }

  else
    if(dst_point.y > (screen_h-m_height))
    {
      dst_point.y = (screen_h-m_height);
    }


    if(m_offset.x < dst_point.x)
    {
      m_offset.x += speed;

        if(m_offset.x > dst_point.x)
        {
          m_offset.x = dst_point.x;
        }
    }

  else
    if(m_offset.x > dst_point.x)
    {
      m_offset.x -= speed;

        if(m_offset.x < dst_point.x)
        {
          m_offset.x = dst_point.x;
        }
    }


    if(m_offset.y < dst_point.y)
    {
      m_offset.y += speed;

        if(m_offset.y > dst_point.y)
        {
          m_offset.y = dst_point.y;
        }
    }

  else
    if(m_offset.y > dst_point.y)
    {
      m_offset.y -= speed;

        if(m_offset.y < dst_point.y)
        {
          m_offset.y = dst_point.y;
        }
    }
}




void
board_view::
reset(const board&  brd, int  w, int  h) noexcept
{
  m_board = &brd;

  m_width  = w;
  m_height = h;
}


namespace{
int
get_corrected_value(int  value, int  size) noexcept
{
  value %= size;

    if(value < 0)
    {
      value += size;
    }


  return value;
}
}


struct
board_view::
rendering_context
{
  int  square_size;

  int  board_width ;
  int  board_height;

  int  board_image_width ;
  int  board_image_height;

  int  output_line;

  image*  target;

};


void
board_view::
render_line(const rendering_context&  ctx) const noexcept
{
  int  point_x = (m_offset.x%ctx.board_image_width);

    if(point_x < 0)
    {
      point_x += ctx.board_image_width;
    }


  int  point_y = (m_offset.y+ctx.output_line)%ctx.board_image_height;

    if(point_y < 0)
    {
      point_y += ctx.board_image_height;
    }


  int  board_x           = point_x/ctx.square_size;
  int        x_remainder = point_x%ctx.square_size;

  int  board_y  = point_y/ctx.square_size;
  int  square_y = point_y%ctx.square_size;

  auto  begin_square = &m_board->get_square(              0,board_y);
  auto    end_square = &m_board->get_square(ctx.board_width,board_y);

  auto  current_square = begin_square+board_x;


  auto  current_dst_pixel = &ctx.target->get_pixel(                      0,ctx.output_line);
  auto      end_dst_pixel = &ctx.target->get_pixel(ctx.target->get_width(),ctx.output_line);

  int  x_offset = 0;

    if(x_remainder)
    {
      x_offset = x_remainder;

        if(x_remainder < 0)
        {
          x_offset = x_remainder+ctx.square_size;
        }
    }


    for(;;)
    {
      auto  dat = current_square++->get_data();

        if(current_square >= end_square)
        {
          current_square = begin_square;
        }


        if(dat)
        {
          auto  srcimg_pt = dat->get_image_point();

          auto  current_src_pixel = &m_source_image->get_const_pixel(srcimg_pt.x+x_offset       ,srcimg_pt.y+square_y);
          auto      end_src_pixel = &m_source_image->get_const_pixel(srcimg_pt.x+ctx.square_size,srcimg_pt.y+square_y);

          x_offset = 0;

            while(current_src_pixel < end_src_pixel)
            {
              *current_dst_pixel++ = *current_src_pixel++;

                if(current_dst_pixel >= end_dst_pixel)
                {
                  return;
                }
            }
        }

      else
        {
          int  n = ctx.square_size-x_offset    ;
                                   x_offset = 0;

            while(n--)
            {
              *current_dst_pixel++ = pixel();

                if(current_dst_pixel >= end_dst_pixel)
                {
                  return;
                }
            }
        }
    }
}


void
board_view::
render(image&  dst, void  (*callback)(board_view&  bv, int  output_line)) noexcept
{
  rendering_context  ctx;

  ctx.square_size = m_board->get_square_size();

  ctx.board_width  = m_board->get_width() ;
  ctx.board_height = m_board->get_height();

  ctx.board_image_width  = m_board->get_image_width() ;
  ctx.board_image_height = m_board->get_image_height();

  ctx.target = &dst;

  ctx.output_line = 0;

  int  n = std::min(dst.get_height(),m_height);

    while(ctx.output_line < n)
    {
        if(callback)
        {
          callback(*this,ctx.output_line);
        }


      render_line(ctx);

      ++ctx.output_line;
    }
}




}}




