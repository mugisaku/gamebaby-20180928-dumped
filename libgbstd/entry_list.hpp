#ifndef GMBB_entry_list_HPP
#define GMBB_entry_list_HPP


#include"libgbstd/string.hpp"
#include<cstddef>


namespace gbstd{
namespace entry_lists{


template<typename  T, size_t  N>
class
entry_list
{
  T  m_dummy;

  T  m_elements[N];

  T*  m_end;

public:
  entry_list() noexcept:
  m_end(m_elements){}

  entry_list(const entry_list&   rhs) noexcept=delete;
  entry_list(      entry_list&&  rhs) noexcept=delete;

  entry_list&  operator=(const entry_list&   rhs) noexcept=delete;
  entry_list&  operator=(      entry_list&&  rhs) noexcept=delete;

  T&  operator[](gbstd::string_view  name) noexcept
  {
    auto  p = &m_elements[0];

      while(p != m_end)
      {
          if(p->get_name() == name)
          {
            return *p;
          }


        ++p;
      }


    p->set_name(name);

    ++m_end;

    return *p;
  }


  const T&  operator[](gbstd::string_view  name) const noexcept
  {
    auto  p = &m_elements[0];

      while(p != m_end)
      {
          if(p->get_name() == name)
          {
            return *p;
          }


        ++p;
      }


    return m_dummy;
  }


  size_t  size() const noexcept{return m_end-&m_elements[0];}

  const T*  begin() const noexcept{return &m_elements[0];}
  const T*    end() const noexcept{return m_end;}

};


}


using entry_lists::entry_list;


}




#endif




