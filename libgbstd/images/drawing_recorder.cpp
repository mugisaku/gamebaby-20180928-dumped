#include"libgbstd/image.hpp"
#include"libgbstd/utility.hpp"



namespace gbstd{
namespace images{


struct
drawing_recorder::
record
{
  record*  next;

  bool  solid;

  size_t  number_of_dots;

  dot  dots[];

};




void
drawing_recorder::
rollback(image&  img) noexcept
{
START:
    if(m_dot_buffer.size())
    {
      auto  dot = m_dot_buffer.back();

      m_dot_buffer.pop_back();

      img.set_pixel(dot.color,dot.x,dot.y);
    }

  else
    if(m_record_list)
    {
      auto  rec = m_record_list            ;
                  m_record_list = rec->next;

        if(rec->solid)
        {
            for(int  i = 0;  i < rec->number_of_dots;  ++i)
            {
              auto&  dot = rec->dots[i];

              img.set_pixel(dot.color,dot.x,dot.y);
            }


          free(rec);
        }

      else
        {
            for(int  i = 0;  i < rec->number_of_dots;  ++i)
            {
              m_dot_buffer.emplace_back(rec->dots[i]);
            }


          free(rec);

          goto START;
        }
    }
}


void
drawing_recorder::
commit(bool  solid) noexcept
{
  int  n = m_dot_buffer.size();

    if(n)
    {
      auto  rec_size = sizeof(record)+(sizeof(dot)*n);

      auto  rec = reinterpret_cast<record*>(malloc(rec_size));

      rec->solid = solid;

      rec->number_of_dots = n;

      std::copy(m_dot_buffer.cbegin(),m_dot_buffer.cend(),rec->dots);

      m_dot_buffer.resize(0);

      rec->next = m_record_list      ;
                  m_record_list = rec;
    }
}


void
drawing_recorder::
clear() noexcept
{
    while(m_record_list)
    {
      auto  next = m_record_list->next;

      free(m_record_list)      ;
           m_record_list = next;
    }


  m_record_list = nullptr;

  m_dot_buffer.resize(0);
}




}}




