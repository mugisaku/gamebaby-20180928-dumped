#ifndef STRING_STRING_COPY_HPP
#define STRING_STRING_COPY_HPP



#include"libgbstd/string_view.hpp"


namespace gbstd{


template<typename  T>
class
basic_string_copy
{
  const T*  m_data;

  size_t  m_length=0;

  bool  m_duplicated=false;

public:
  basic_string_copy() noexcept{}
  basic_string_copy(basic_string_view<T>  sv) noexcept{*this = sv;}
  basic_string_copy(const basic_string_copy&   rhs) noexcept{*this = rhs;}
  basic_string_copy(      basic_string_copy&&  rhs) noexcept{*this = std::move(rhs);}
 ~basic_string_copy(){clear();}

  basic_string_copy&  operator=(const basic_string_copy&   rhs) noexcept
  {
    return *this = gbstd::string_view(rhs.data(),rhs.size());
  }

  basic_string_copy&  operator=(basic_string_copy&&  rhs) noexcept
  {
    clear();

    std::swap(m_data      ,rhs.m_data);
    std::swap(m_length    ,rhs.m_length);
    std::swap(m_duplicated,rhs.m_duplicated);

    return *this;
  }

  basic_string_copy&  operator=(basic_string_view<T>  sv) noexcept
  {
    clear();

    m_length = sv.size();

      if(sv.is_null_terminated())
      {
        m_data = sv.data();
      }

    else
      {
        auto  ptr = new T[m_length+1];

        std::memcpy(ptr,sv.data(),m_length);

                 ptr[m_length] = 0;
        m_data = ptr              ;

        m_duplicated = true;
      }


    return *this;
  }

  void  clear() noexcept
  {
      if(m_duplicated)
      {
        delete[] const_cast<T*>(m_data);
      }


    m_data = nullptr;

    m_length     = 0;
    m_duplicated = false;
  }

  size_t  size() const noexcept{return m_length;}

  const char*  data() const noexcept{return m_data;}

  using iterator = const T*;

  iterator  begin() const noexcept{return m_data;}
  iterator    end() const noexcept{return m_data+m_length;}

};


using    string_copy = basic_string_copy<char>;
using u16string_copy = basic_string_copy<char16_t>;


}


#endif





