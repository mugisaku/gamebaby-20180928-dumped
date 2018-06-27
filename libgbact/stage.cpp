#include"libgbact/stage.hpp"
#include"libgbact/routine.hpp"




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




stage::
stage() noexcept
{
  resize(g_board_width,g_board_height);

    for(int  x = 0;  x < m_width;  ++x)
    {
      get_prop(x,         0).set_block_index(1);
      get_prop(x,m_height-1).set_block_index(1);
    }


    for(int  y = 1;  y < (m_height-1);  ++y)
    {
      get_prop(        0,y).set_block_index(1);
      get_prop(m_width-1,y).set_block_index(1);
    }


  set_prop(prop(0,1),4,4);
  set_prop(prop(0,2),8,4);
}




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

    if(dst.get_object_index() == 1)
    {
      m_lady_prop = &dst;

      m_lady_point = point(x,y);
    }

  else
    if(dst.get_object_index() == 2)
    {
      m_boy_prop = &dst;

      m_boy_point = point(x,y);
    }
}


void
stage::
set_block_index(int  i, int  x, int  y) noexcept
{
  get_prop(x,y).set_block_index(i);
}


void
stage::
set_object(int  i, int  x, int  y) noexcept
{
  auto&  dst = get_prop(x,y);

  dst.set_object_index(i);

    if(dst.get_object_index() == 1)
    {
      m_lady_prop = &dst;

      m_lady_point = point(x,y);
    }

  else
    if(dst.get_object_index() == 2)
    {
      m_boy_prop = &dst;

      m_boy_point = point(x,y);
    }
}




void
stage::
restore(character_set&  chset) const noexcept
{
    for(int  y = 0;  y < m_height;  ++y){
    for(int  x = 0;  x < m_width ;  ++x){
      auto&  pr = get_prop(x,y);

      auto&  sq = g_board.get_square(x,y);

      sq.set_data(g_square_data_set[pr.get_block_index()]);

      real_point  pt(g_square_size*x,g_square_size*y);

      spaces::object*  obj = nullptr;

        switch(pr.get_object_index())
        {
      case(1):
          obj = &chset.m_lady;
          break;
      case(2):
          obj = &chset.m_boy;
          break;
      case(3):
          chset.m_meats.emplace_back(characters::meat());

          obj = &chset.m_meats.back();
          break;
      case(4):
          chset.m_walls.emplace_back(characters::wall());

          obj = &chset.m_walls.back();
          break;
        }


        if(obj)
        {
          obj->set_base_point(pt);
          obj->set_kinetic_energy(0,0);
        }
    }}
}




}}




