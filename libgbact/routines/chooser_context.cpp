#include"libgbact/routine.hpp"




namespace gbact{
namespace routines{




chooser_context::
chooser_context(std::initializer_list<gbstd::string_view>  ls) noexcept
{
    for(auto&  sv: ls)
    {
      m_text_list.emplace_back(sv);
    }
}




void
chooser_context::
clean() noexcept
{
    for(auto&  to: m_text_objects)
    {
      to.die();
    }  


  m_cursor.die();
}


void
chooser_context::
step() noexcept
{
  int  i;
  int  y;

    switch(get_pc())
    {
  case(0):
      m_index = 0;

      y = 0;

      m_cursor = spaces::image_object(g_misc_image,rectangle(0,0,24,24),point(-28,4));

      m_cursor.set_base_point(80,y);

      g_object_space.append(m_cursor);

      i = 0;

        for(auto&  s: m_text_list)
        {
          auto&  to = m_text_objects[i++];

          to = spaces::text_object(s,styles::a_white_based_text_style);

          to.set_base_point(80,y);

          y += 24;

          g_object_space.append(to);
        }


      add_pc(1);
      break;
  case(1):
        if(g_modified_input.test_up_button() && g_input.test_up_button() && m_index)
        {
          --m_index;

          m_cursor.add_base_point_y(-24);
        }

      else
        if(g_modified_input.test_down_button() && g_input.test_down_button() && (m_index < (m_text_list.size()-1)))
        {
          ++m_index;

          m_cursor.add_base_point_y(24);
        }

      else
        if(g_modified_input.test_p_button() && g_input.test_p_button())
        {
          g_object_space.remove_all();

          end(m_index);
        }

      else
        if(g_modified_input.test_n_button() && g_input.test_n_button())
        {
          end();
        }
      break;
    }
}




}}




