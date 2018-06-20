#include"libgbact/stage.hpp"




namespace gbact{
namespace stages{




namespace{
square_data
g_data[] = {
  square_data::null(),
  square_data::block(),
  square_data::ladder0(),
  square_data::ladder1(),
};
}




void
stage::
resize(int  w, int  h) noexcept
{
  m_width  = w;
  m_height = h;

  m_table.resize(w*h);
}


square_data&
stage::
get_square_data(int  i) noexcept
{
  return g_data[i];
}




}}




