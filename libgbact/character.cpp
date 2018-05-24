#include"libgbact/character.hpp"




namespace gbact{
namespace characters{


images::image
g_image;




void
character::
set_data(character_data*  dat) noexcept
{
  delete m_data      ;
         m_data = dat;

    if(m_data)
    {
      m_data->set_character(*this);

      m_data->initialize_character();
    }
}


const char*
character::
get_class_id() const noexcept
{
  return "character";
}


void
character::
do_when_collided(object&  other_side, spaces::position  position) noexcept
{
    if(other_side.query(0))
    {
      m_data->do_when_collided_with_character(static_cast<character&>(other_side),position);
    }

  else
   {
      m_data->do_when_collided_with_object(other_side,position);
    }
}


void
character::
update() noexcept
{
    if(m_data)
    {
      m_data->update_character();
      m_data->update_image();
    }


  object::update();
}


void
character::
render(images::image&  dst) const noexcept
{
  image_object::render(dst);


  auto  rect = get_rectangle();

  dst.draw_rectangle_safely(colors::red,rect.x,rect.y,rect.w,rect.h);
}




}}




