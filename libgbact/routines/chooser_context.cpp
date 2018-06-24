#include"libgbact/routine.hpp"




namespace gbact{
namespace routines{




void
chooser_context::
initialize(std::initializer_list<gbstd::string_view>  ls) noexcept
{
  m_index = 0;

  int  y = 0;

  m_cursor = spaces::image_object(g_misc_image,rectangle(0,0,24,24),point(-28,4));

  m_cursor.set_base_point(80,y);

  g_object_space.append(m_cursor);

    for(auto&  sv: ls)
    {
      m_text_objects.emplace_back(sv,styles::a_white_based_text_style);

      auto&  to = m_text_objects.back();

      to.set_base_point(80,y);

      y += 24;
    }



    for(auto&  to: m_text_objects)
    {
      g_object_space.append(to);
    }


  g_object_space_validity.enable();
}




void
chooser_context::
clean() noexcept
{
    for(auto&  to: m_text_objects)
    {
      to.die();
    }


  m_text_objects.clear();

  m_cursor.die();
}


void
chooser_context::
step() noexcept
{
    if(g_modified_input.test_up_button() && g_input.test_up_button() && m_index)
    {
      --m_index;

      m_cursor.add_base_point_y(-24);
    }

  else
    if(g_modified_input.test_down_button() && g_input.test_down_button() && (m_index < (m_text_objects.size()-1)))
    {
      ++m_index;

      m_cursor.add_base_point_y(24);
    }

  else
    if(g_modified_input.test_p_button() && g_input.test_p_button())
    {
      end(m_index);
    }

  else
    if(g_modified_input.test_n_button() && g_input.test_n_button())
    {
      end();
    }
}




}}




