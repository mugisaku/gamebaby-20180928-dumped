#ifndef GMBB_Figures_HPP
#define GMBB_Figures_HPP


#include<cstdio>
#include<cstdlib>
#include<algorithm>


namespace gbstd{
namespace images{


template<typename  T>
struct
basic_point
{
  using value_type = T;

  value_type  x;
  value_type  y;

  constexpr basic_point(value_type  x_=0, value_type  y_=0) noexcept:
  x(x_),
  y(y_){}

  template<typename  U>
  constexpr basic_point(basic_point<U>  rhs) noexcept:
  x(static_cast<T>(rhs.x)),
  y(static_cast<T>(rhs.y)){}


  constexpr bool  operator==(basic_point const&  rhs) const noexcept
  {
    return(x == rhs.x) &&
          (y == rhs.y);
  }

  constexpr bool  operator!=(basic_point const&  rhs) const noexcept
  {
    return(x != rhs.x) ||
          (y != rhs.y);
  }

  constexpr basic_point  operator+(basic_point const&  rhs) const noexcept
  {
    return basic_point(x+rhs.x,y+rhs.y);
  }

  constexpr basic_point  operator-(basic_point const&  rhs) const noexcept
  {
    return basic_point(x-rhs.x,y-rhs.y);
  }

  constexpr basic_point  operator+(value_type  n) const noexcept
  {
    return basic_point(x+n,y+n);
  }

  constexpr basic_point  operator*(value_type  n) const noexcept
  {
    return basic_point(x*n,y*n);
  }

  constexpr basic_point  operator/(value_type  n) const noexcept
  {
    return basic_point(x/n,y/n);
  }

  constexpr basic_point  operator%(value_type  n) const noexcept
  {
    return basic_point(x%n,y%n);
  }

  constexpr basic_point  operator-() const noexcept
  {
    return basic_point(-x,-y);
  }

  constexpr basic_point&  operator+=(basic_point const&  rhs) noexcept
  {
    x += rhs.x;
    y += rhs.y;

    return *this;
  }

  constexpr basic_point&  operator-=(basic_point const&  rhs) noexcept
  {
    x -= rhs.x;
    y -= rhs.y;

    return *this;
  }

  constexpr basic_point&  operator*=(value_type  n) noexcept
  {
    x *= n;
    y *= n;

    return *this;
  }

  constexpr basic_point&  operator/=(value_type  n) noexcept
  {
    x /= n;
    y /= n;

    return *this;
  }

  constexpr basic_point&  operator%=(value_type  n) noexcept
  {
    x %= n;
    y %= n;

    return *this;
  }


  basic_point&  move_x(value_type  n) noexcept
  {
    x += n;

    return *this;
  }


  basic_point&  move_y(value_type  n) noexcept
  {
    y += n;

    return *this;
  }

};


using      point = basic_point<int>;
using real_point = basic_point<double>;


struct
rectangle: public point
{
  int  w;
  int  h;

  constexpr rectangle(int  x_=0, int  y_=0, int  w_=0, int  h_=0) noexcept:
  point(x_,y_),
  w(w_),
  h(h_){}

};


constexpr rectangle
make_rectangle(point  a, point  b) noexcept
{
  int  x =   std::min(a.x,b.x);
  int  y =   std::min(a.y,b.y);
  int  w = 1+std::abs(a.x-b.x);
  int  h = 1+std::abs(a.y-b.y);

  return rectangle(x,y,w,h);
}


struct
triangle
{
  point  a_point;
  point  b_point;
  point  c_point;

  constexpr triangle(point  a=point(), point  b=point(), point  c=point()) noexcept:
  a_point(a),
  b_point(b),
  c_point(c){}

};


struct
line
{
  point  a_point;
  point  b_point;

  constexpr line(point  a=point(), point  b=point()) noexcept:
  a_point(a),
  b_point(b){}

  constexpr line(int  x0, int  y0, int  x1, int  y1) noexcept:
  a_point(x0,y0),
  b_point(x1,y1){}

};


}


using images::point;
using images::real_point;
using images::rectangle;
using images::triangle;
using images::line;


}


#endif




