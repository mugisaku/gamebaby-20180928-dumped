#ifndef LIBGBSND_list_HPP
#define LIBGBSND_list_HPP


#include<cstddef>
#include<initializer_list>
#include"libgbstd/utility.hpp"


namespace gbsnd{


template<typename  T>
class
list
{
  T*  m_data=nullptr;

  size_t  m_number_of_elements=0;

public:
  list() noexcept{}
  list(std::initializer_list<T>  ls) noexcept{assign(ls.begin(),ls.size());}
  list(const T*  src, size_t  n) noexcept{assign(src,n);}
  list(const list&   rhs) noexcept{*this = rhs;}
  list(      list&&  rhs) noexcept{*this = std::move(rhs);}
 ~list(){clear();}

  list&  operator=(const list&  rhs) noexcept
  {
      if(this != &rhs)
      {
        assign(rhs.m_data,rhs.m_number_of_elements);
      }


    return *this;
  }


  list&  operator=(list&&  rhs) noexcept
  {
      if(this != &rhs)
      {
        clear();

        std::swap(m_data              ,rhs.m_data              );
        std::swap(m_number_of_elements,rhs.m_number_of_elements);
      }


    return *this;
  }


  void  clear() noexcept
  {
    delete[] m_data          ;
             m_data = nullptr;

    m_number_of_elements = 0;
  }


  void  assign(const T*  src, size_t  n) noexcept
  {
    clear();

    m_data = gbstd::duplicate(src,n);

    m_number_of_elements = n;
  }


  const T*  data() const noexcept{return m_data;}

  const T&  front() const noexcept{return m_data[0];}
  const T&   back() const noexcept{return m_data[m_number_of_elements-1];}

  const T*  begin() const noexcept{return m_data;}
  const T*    end() const noexcept{return m_data+m_number_of_elements;}

  size_t  size() const noexcept{return m_number_of_elements;}

};


}




#endif




