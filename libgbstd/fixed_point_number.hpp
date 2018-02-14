#ifndef gmbb_FixedPointNumber_HPP
#define gmbb_FixedPointNumber_HPP


#include<cstdint>


namespace gbstd{


class
fixed_point_number
{
public:
  using type = fixed_point_number;

  static constexpr int  shift_amount = 16;

  static constexpr int  unit = 1<<shift_amount;
  static constexpr int  half = unit/2;

  struct internal_value{int32_t  data;};

private:
  internal_value  m_value;

  static constexpr int32_t  add_char(char  c, int32_t  n) noexcept
  {
    return ((c >= '0') && (c <= '9'))? (n*10)+(c-'0')
          :n;
  }

  static constexpr int32_t  add_char_fract(char  c, int32_t  n, int  base) noexcept
  {
    return ((c >= '0') && (c <= '9'))? n+(base*(c-'0'))
          :n;
  }

  static constexpr int32_t  to_fract_int(char const*  s, int32_t  n, int  base=(1<<shift_amount)/10) noexcept
  {
    return *s? to_fract_int(s+1,add_char_fract(*s,n,base),base/10)
          :n;
  }

  static constexpr int32_t  to_int(char const*  s, int32_t  n) noexcept
  {
    return *s? (*s == '.')? (n<<shift_amount)|to_fract_int(s+1,             0)
              :                                     to_int(s+1,add_char(*s,n))
          :(n<<shift_amount);
  }

  static constexpr int32_t  to_int(char const*  s) noexcept
  {
    return (*s == '-')? -to_int(s+1,0)
                      :  to_int(s  ,0);
  }


  constexpr explicit fixed_point_number(internal_value  v) noexcept: m_value{v.data}{}

public:
  constexpr explicit fixed_point_number() noexcept: m_value{0}{}
  constexpr explicit fixed_point_number(int  n) noexcept: m_value{n<<shift_amount}{}
  constexpr explicit fixed_point_number(char const*  s) noexcept: m_value{to_int(s)}{}

  constexpr  operator bool() const noexcept{return m_value.data;}

  constexpr bool  operator==(type const&  rhs) const noexcept{return m_value.data == rhs.m_value.data;}
  constexpr bool  operator!=(type const&  rhs) const noexcept{return m_value.data != rhs.m_value.data;}
  constexpr bool  operator< (type const&  rhs) const noexcept{return m_value.data <  rhs.m_value.data;}
  constexpr bool  operator<=(type const&  rhs) const noexcept{return m_value.data <= rhs.m_value.data;}
  constexpr bool  operator> (type const&  rhs) const noexcept{return m_value.data >  rhs.m_value.data;}
  constexpr bool  operator>=(type const&  rhs) const noexcept{return m_value.data >= rhs.m_value.data;}

  constexpr type  operator+(type const&  rhs) const noexcept{return type(internal_value{m_value.data+rhs.m_value.data});}
  constexpr type  operator-(type const&  rhs) const noexcept{return type(internal_value{m_value.data-rhs.m_value.data});}
  constexpr type  operator-() const noexcept{return type(internal_value{-m_value.data});}

  constexpr type  operator+(int  n) const noexcept{return type(internal_value{m_value.data+(n<<shift_amount)});}
  constexpr type  operator-(int  n) const noexcept{return type(internal_value{m_value.data-(n<<shift_amount)});}
  constexpr type  operator*(int  n) const noexcept{return type(internal_value{m_value.data*n});}
  constexpr type  operator/(int  n) const noexcept{return type(internal_value{m_value.data/n});}
  constexpr type  operator%(int  n) const noexcept{return type(internal_value{m_value.data%n});}
  constexpr type  operator<<(int  n) const noexcept{return type(internal_value{m_value.data<<n});}
  constexpr type  operator>>(int  n) const noexcept{return type(internal_value{m_value.data>>n});}

  constexpr int  to_int() const noexcept
  {
    return ((m_value.data >= 0)? (m_value.data+half)
                               : (m_value.data-half))>>shift_amount;}

  type&  operator=(int  n) noexcept{return *this = type(n);}

  type&  operator+=(type const&  rhs) noexcept{return *this = (*this)+rhs;}
  type&  operator-=(type const&  rhs) noexcept{return *this = (*this)-rhs;}

  type&  operator+=(int  n) noexcept{m_value.data += (n<<shift_amount);  return *this;}
  type&  operator-=(int  n) noexcept{m_value.data -= (n<<shift_amount);  return *this;}
  type&  operator*=(int  n) noexcept{m_value.data *= n;  return *this;}
  type&  operator/=(int  n) noexcept{m_value.data /= n;  return *this;}
  type&  operator%=(int  n) noexcept{m_value.data %= n;  return *this;}
  type&  operator<<=(int  n) noexcept{m_value.data <<= n;  return *this;}
  type&  operator>>=(int  n) noexcept{m_value.data >>= n;  return *this;}

};




}




#endif




