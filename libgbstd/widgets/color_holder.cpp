#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{


constexpr int  rect_size = 16;

static const background_style  bgst(color(0,0,0));


color_holder::
color_holder(int  size, void  (*callback)(color_holder&,color)) noexcept:
m_colors(size),
m_callback(callback)
{
  m_background_style = &bgst;

    for(auto&  e: m_colors)
    {
      e = color(0,0,0);
    }
}


color_holder::
color_holder(std::initializer_list<color>  colors, void  (*callback)(color_holder&,color)) noexcept:
m_colors(colors),
m_callback(callback)
{
  m_background_style = &bgst;
}




void
color_holder::
set_color(colors::color  color) noexcept
{
  m_colors[m_index] = color;

  need_to_redraw();
}


void
color_holder::
reform(point  base_pt) noexcept
{
  widget::reform(base_pt);

  m_width  = rect_size+(font_width*3);
  m_height = font_height*m_colors.size();
}


void
color_holder::
render(image_cursor  cur) noexcept
{
  render_background(cur);

  int  y = 0;

    for(auto&  color: m_colors)
    {
      cur.draw_rectangle(colors::white,0,y  ,rect_size  ,rect_size  );
      cur.fill_rectangle(color        ,1,y+1,rect_size-2,rect_size-2);


      string_form  sf("%d%d%d",color.get_r7(),color.get_g7(),color.get_b7());

      static const text_style  txst(colors::null,colors::white,colors::null,colors::null);

      cur.draw_text(*sf,txst,rect_size,y);

      y += rect_size;
    }



  cur.draw_rectangle(colors::white,0,rect_size*m_index,get_width(),rect_size);
}


void
color_holder::
update() noexcept
{
  auto  mouse = get_mouse();

    if(mouse.left_button)
    {
      m_index = mouse->y/rect_size;

        if(m_callback)
        {
          m_callback(*this,get_color());
        }


      need_to_redraw();
    }
}




}}




