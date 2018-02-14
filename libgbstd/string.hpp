#ifndef STRING_STRING_HPP
#define STRING_STRING_HPP



#include"libgbstd/string_view.hpp"
#include"libgbstd/string_copy.hpp"
#include"libgbstd/utility.hpp"


namespace gbstd{


template<typename  T>
class
basic_string
{
  T*  m_data=nullptr;

  size_t  m_length=0;
  size_t  m_capacity=0;

  void  extend() noexcept
  {
    auto  new_pointer = new T[((m_capacity+8)*2)+1];

    std::memcpy(new_pointer,m_data,m_length);

    m_capacity += 8;
    m_capacity *= 2;

    new_pointer[m_length] = 0;

    delete[] m_data              ;
             m_data = new_pointer;
  }

public:
  basic_string() noexcept{}
  basic_string(const T*  s) noexcept{assign(s);}
  basic_string(const T*  s, size_t  l) noexcept{assign(s,l);}
  basic_string(basic_string_view<T>  sv) noexcept{assign(sv);}
  basic_string(const basic_string&   rhs) noexcept{*this = rhs;}
  basic_string(      basic_string&&  rhs) noexcept{*this = std::move(rhs);}
 ~basic_string(){clear();}

  basic_string&  operator=(basic_string_view<T>  sv) noexcept
  {
    assign(sv);

    return *this;
  }

  basic_string&  operator=(const char*  s) noexcept
  {
    assign(s);

    return *this;
  }

  basic_string&  operator=(const basic_string&   rhs) noexcept
  {
    assign(rhs.data(),rhs.size());

    return *this;
  }

  basic_string&  operator=(basic_string&&  rhs) noexcept
  {
    clear();

    std::swap(m_data    ,rhs.m_data  );
    std::swap(m_length  ,rhs.m_length);
    std::swap(m_capacity,rhs.m_capacity);

    return *this;
  }

  basic_string&  operator+=(T  c) noexcept
  {
    append(c);

    return *this;
  }

  basic_string&  operator+=(const T*  s) noexcept
  {
    append(s);

    return *this;
  }

  basic_string&  operator+=(const basic_string&  s) noexcept
  {
    append(s);

    return *this;
  }


  bool  operator==(const basic_string&  rhs) const noexcept
  {
    return (m_length == rhs.m_length) && (std::memcmp(m_data,rhs.m_data,m_length) == 0);
  }


  bool  operator!=(const basic_string&  rhs) const noexcept
  {
    return !(*this == rhs);
  }


  bool  operator==(const basic_string_view<T>&  sv) const noexcept
  {
    return (m_length == sv.size()) && (std::memcmp(m_data,sv.data(),m_length) == 0);
  }


  bool  operator!=(const basic_string_view<T>&  sv) const noexcept
  {
    return !(*this == sv);
  }


  void  clear() noexcept
  {
    delete[] m_data          ;
             m_data = nullptr;

    m_length   = 0;
    m_capacity = 0;
  }

  void  assign(const T*  s) noexcept
  {
    assign(s,std::strlen(s));
  }

  void  assign(const T*  s, size_t  l) noexcept
  {
    clear();

    m_data     = new T[l+1];
    m_length   = l;
    m_capacity = l;

    std::memcpy(m_data,s,l);

    m_data[l] = 0;
  }

  void  assign(basic_string_view<T>  sv) noexcept
  {
    assign(sv.data(),sv.size());
  }





  void  append(T  c) noexcept
  {
      while(m_length >= m_capacity)
      {
        extend();
      }


    char*  p = &m_data[m_length];

    *p++ = c;
    *p   = 0;

    ++m_length;
  }

  void  append(const T*  s) noexcept
  {
      while(*s)
      {
        append(*s++);
      }
  }

  void  append(const T*  s, size_t  l) noexcept
  {
      while(l--)
      {
        append(*s++);
      }
  }

  void  append(const basic_string&  s) noexcept
  {
    append(s.data(),s.size());
  }




  size_t  size() const noexcept{return m_length;}
  size_t  capacity() const noexcept{return m_capacity;}

  const char*  data() const noexcept{return m_data;}

  using iterator = const T*;

  iterator  begin() const noexcept{return m_data;}
  iterator    end() const noexcept{return m_data+m_length;}

};


using    string = basic_string<char>;
using u16string = basic_string<char16_t>;


}


#endif





