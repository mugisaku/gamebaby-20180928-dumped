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




constexpr int  index_of_lady = 1;
constexpr int  index_of_boy  = 2;
constexpr int  index_of_meat = 3;
constexpr int  index_of_wall = 4;


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


  put_prop(prop(0,index_of_lady),4,4);
  put_prop(prop(0,index_of_boy),8,4);


  put_prop(prop(0,index_of_wall),2,4);
  put_prop(prop(0,index_of_wall),3,4);
  put_prop(prop(0,index_of_wall),6,4);
  put_prop(prop(0,index_of_wall),7,4);
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
put_prop(const prop&  pr, int  x, int  y) noexcept
{
  auto&  dst = get_prop(x,y);

  auto  dst_obji = dst.get_object_index();
  auto  src_obji =  pr.get_object_index();

    if(src_obji == 0)
    {
      dst.set_block_index(pr.get_block_index());
    }

  else
    if(src_obji == index_of_lady)
    {
        if(m_lady_prop)
        {
          m_lady_prop->set_object_index(0);
        }


      dst.set_object_index(index_of_lady);

      m_lady_prop = &dst;

      m_lady_point = point(x,y);
    }

  else
    if(src_obji == index_of_boy)
    {
        if(m_boy_prop)
        {
          m_boy_prop->set_object_index(0);
        }


      dst.set_object_index(src_obji);

      m_boy_prop = &dst;

      m_boy_point = point(x,y);
    }

  else
    {
      dst.set_object_index((dst_obji == src_obji)? 0:src_obji);
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

      real_point  pt(g_square_size*x+g_square_size/2,g_square_size*y);

      spaces::object*  obj = nullptr;

        switch(pr.get_object_index())
        {
      case(index_of_lady):
          pt.y += 24;

          obj = &chset.m_lady;
          break;
      case(index_of_boy):
          pt.y += 24;

          obj = &chset.m_boy;
          break;
      case(index_of_meat):
          chset.m_meats.emplace_back(characters::meat());

          obj = &chset.m_meats.back();
          break;
      case(index_of_wall):
          pt.y += 24;

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




