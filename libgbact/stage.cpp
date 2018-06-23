#include"libgbact/stage.hpp"




namespace gbact{
namespace stages{




std::vector<square_data>
g_square_data_set ={
  square_data::null(),
  square_data::block0(),
  square_data::block1(),
  square_data::ladder0(),
  square_data::ladder1(),
};




void
stage::
resize(int  w, int  h) noexcept
{
  m_width  = w;
  m_height = h;

  m_table.resize(w*h);
}


void
stage::
set_prop(const prop&  pr, int  x, int  y) noexcept
{
  auto&  dst = get_prop(x,y);

  dst = pr;

    if(dst.is_player())
    {
        if(dst.get_index() == 0)
        {
          m_lady_prop = &dst;
        }

      else
        if(dst.get_index() == 1)
        {
          m_boy_prop = &dst;
        }
    }
}



}}




