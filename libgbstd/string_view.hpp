#ifndef STRING_STRING_VIEW_HPP
#define STRING_STRING_VIEW_HPP



#include<cstddef>
#include<cstring>
#include<utility>


namespace gbstd{


template<typename  T> class  basic_string;


template<typename  T>
class
basic_string_view
{
  const T*  m_pointer=nullptr;

  size_t  m_length=0;

  static constexpr size_t  strlen_(const T*  p, size_t  l) noexcept
  {
    return *p? strlen_(p+1,l+1):l;
  }

  static constexpr size_t  strlen(const T*  p) noexcept
  {
    return strlen_(p,0);
  }

  static constexpr bool  strcmp(const T*  a, const T*  b, size_t  l) noexcept
  {
    return l? ((*a == *b)? strcmp(a+1,b+1,l-1):false)
          :true;
  }

public:
  constexpr basic_string_view() noexcept{}
  constexpr basic_string_view(const T*  s) noexcept{assign(s);}
  constexpr basic_string_view(const T*  s, size_t  l) noexcept{assign(s,l);}
  basic_string_view(const basic_string<T>&  s) noexcept{assign(s);}


  basic_string_view&  operator=(const basic_string<T>&  s) const noexcept
  {
    assign(s);

    return *this;
  }


  constexpr bool  operator==(const basic_string_view&  rhs) const noexcept
  {
    return (m_length == rhs.m_length) && strcmp(m_pointer,rhs.m_pointer,m_length);
  }


  constexpr bool  operator!=(const basic_string_view&  rhs) const noexcept
  {
    return !(*this == rhs);
  }


  constexpr void  assign(const T*  s) noexcept
  {
    assign(s,strlen(s));
  }

  constexpr void  assign(const T*  s, size_t  l) noexcept
  {
    m_pointer= s;
    m_length = l;
  }

  void  assign(const basic_string<T>&  s) noexcept
  {
    m_pointer= s.data();
    m_length = s.size();
  }

  constexpr size_t  size() const noexcept{return m_length;}

  constexpr const char*  data() const noexcept{return m_pointer;}

  constexpr const char&  operator[](int  i) const noexcept{return m_pointer[i];}

  constexpr bool  is_null_terminated() const noexcept{return !*(m_pointer+m_length);}

  using iterator = const T*;

  constexpr iterator  begin() const noexcept{return m_pointer;}
  constexpr iterator    end() const noexcept{return m_pointer+m_length;}

};


using    string_view = basic_string_view<char>;
using u16string_view = basic_string_view<char16_t>;


}


#endif





