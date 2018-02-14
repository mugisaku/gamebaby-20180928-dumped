#ifndef GMBB_Figures_HPP
#define GMBB_Figures_HPP


#include<cstdio>


#ifndef report
#define report printf("[report] %s, %s, %d\n",__FILE__,__func__,__LINE__);
#endif


namespace gbstd{
namespace images{


struct
point
{
  int  x;
  int  y;

  constexpr point(int  x_=0, int  y_=0) noexcept:
  x(x_),
  y(y_){}


  constexpr point  operator+(point const&  rhs) const noexcept
  {
    return point(x+rhs.x,y+rhs.y);
  }

  constexpr point  operator-(point const&  rhs) const noexcept
  {
    return point(x-rhs.x,y-rhs.y);
  }

  constexpr point  operator+(int  n) const noexcept
  {
    return point(x+n,y+n);
  }

  constexpr point  operator*(int  n) const noexcept
  {
    return point(x*n,y*n);
  }

  constexpr point  operator/(int  n) const noexcept
  {
    return point(x/n,y/n);
  }

  constexpr point  operator%(int  n) const noexcept
  {
    return point(x%n,y%n);
  }

  constexpr point  operator-() const noexcept
  {
    return point(-x,-y);
  }

  constexpr point&  operator+=(point const&  rhs) noexcept
  {
    x += rhs.x;
    y += rhs.y;

    return *this;
  }

  constexpr point&  operator*=(int  n) noexcept
  {
    x *= n;
    y *= n;

    return *this;
  }

  constexpr point&  operator/=(int  n) noexcept
  {
    x /= n;
    y /= n;

    return *this;
  }

  constexpr point&  operator%=(int  n) noexcept
  {
    x %= n;
    y %= n;

    return *this;
  }

  void  print(char const*  prefix="") const
  {
    printf("[print point %s] %4d %4d\n",prefix,x,y);
  }


  point&  move_x(int  n) noexcept
  {
    x += n;

    return *this;
  }


  point&  move_y(int  n) noexcept
  {
    y += n;

    return *this;
  }


  void  transform(int  w, int  h) noexcept
  {
      if(x < 0)
      {
          while(x < 0)
          {
            x += w;
          }
      }

    else
      if(x >= w)
      {
          while(x >= w)
          {
            x -= w;
          }
      }


      if(y < 0)
      {
          while(y < 0)
          {
            y += h;
          }
      }

    else
      if(y >= h)
      {
          while(y >= h)
          {
            y -= h;
          }
      }
  }


  void  transform(int  w, int  h, int  field_w, int  field_h) noexcept
  {
      while((x+w) >= field_w)
      {
        x -= field_w;
      }


      while((y+h) >= field_h)
      {
        y -= field_h;
      }
  }

};


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

};


}


using images::point;
using images::rectangle;
using images::triangle;
using images::line;


}


#endif




