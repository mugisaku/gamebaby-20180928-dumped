#include"libgbstd/board.hpp"
#include"libgbstd/text.hpp"




namespace gbstd{
namespace boards{




void
board_view::
reset(const board&  brd, int  w, int  h) noexcept
{
  m_board = &brd;

  m_width  = w;
  m_height = h;


  auto  sq_size = m_board->get_square_size();

  m_image.resize(((((w+sq_size-1))/sq_size)+1)*sq_size,
                 ((((h+sq_size-1))/sq_size)+1)*sq_size);

  update_image(point(0,0));
}


point
board_view::
get_base_point() const noexcept
{
  auto  x = m_board->get_corrected_x(m_offset.x);
  auto  y = m_board->get_corrected_y(m_offset.y);

  return point(x,y);
}


void
board_view::
update_image(point  base_square_point) noexcept
{
  auto  sq_size = m_board->get_square_size();

  auto  brd_w = m_board->get_width() ;
  auto  brd_h = m_board->get_height();


  m_base_square_point = base_square_point;

  auto   sq_y = m_base_square_point.y;
  auto  img_w = m_image.get_width() ;
  auto  img_h = m_image.get_height();

  m_image.fill();

  rectangle  srcimg_rect(0,0,sq_size,sq_size);

    for(int  y = 0;  y < img_h;  y += sq_size)
    {
      auto  sq_x = m_base_square_point.x;

        for(int  x = 0;  x < img_w;  x += sq_size)
        {
          auto&  sq = m_board->get_square(sq_x,sq_y);

          auto  dat = sq.get_data();

            if(dat)
            {
              static_cast<point&>(srcimg_rect) = dat->get_image_point();

              images::overlay(*m_source_image,srcimg_rect,m_image,point(x,y));
            }


            if(++sq_x >= brd_w)
            {
              sq_x = 0;
            }
        }


        if(++sq_y >= brd_h)
        {
          sq_y = 0;
        }
    }
}


void
board_view::
render(image&  dst) noexcept
{
  auto  sq_size = m_board->get_square_size();

  auto  base_pt = get_base_point();

  auto  sq_x = base_pt.x/sq_size;
  auto  sq_y = base_pt.y/sq_size;

    if((m_base_square_point.x != sq_x) ||
       (m_base_square_point.y != sq_y))
    {
      update_image(point(sq_x,sq_y));
    }


  int  src_x = (m_offset.x%sq_size);
  int  src_y = (m_offset.y%sq_size);

    if(src_x < 0)
    {
      src_x += sq_size;
    }


    if(src_y < 0)
    {
      src_y += sq_size;
    }


  images::overlay(m_image,rectangle(src_x,src_y,m_width,m_height),dst,point());
}




}}




