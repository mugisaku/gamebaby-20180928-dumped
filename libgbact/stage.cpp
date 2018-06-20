#include"libgbact/stage.hpp"




namespace gbact{
namespace stages{




std::vector<square_data>
g_square_data_set ={
  square_data::null(),
  square_data::block(),
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




}}




