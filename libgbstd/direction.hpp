#ifndef gmbb_direction_HPP
#define gmbb_direction_HPP


namespace gbstd{
namespace directions{


enum class
direction
{
        front,
   front_left,
         left,
    back_left,
         back,
   back_right,
        right,
  front_right,

  last = front_right,

  unknown,
  error,
};


constexpr direction
get_opposite(direction  d) noexcept
{
  return (d == direction::front      )? direction::back
        :(d == direction::front_left )? direction::back_right
        :(d == direction::left       )? direction::right
        :(d == direction::back_left  )? direction::front_right
        :(d == direction::back       )? direction::front
        :(d == direction::back_right )? direction::front_left
        :(d == direction::right      )? direction::left
        :(d == direction::front_right)? direction::back_left
        :direction::unknown;
         
}


constexpr direction
get_left(direction  d) noexcept
{
  return (d == direction::front      )? direction::front_left
        :(d == direction::front_left )? direction::left
        :(d == direction::left       )? direction::back_left
        :(d == direction::back_left  )? direction::back
        :(d == direction::back       )? direction::back_right
        :(d == direction::back_right )? direction::right
        :(d == direction::right      )? direction::front_right
        :(d == direction::front_right)? direction::front
        :direction::unknown;
         
}


constexpr direction
get_right(direction  d) noexcept
{
  return (d == direction::front      )? direction::front_right
        :(d == direction::front_left )? direction::front
        :(d == direction::left       )? direction::front_left
        :(d == direction::back_left  )? direction::left
        :(d == direction::back       )? direction::back_left
        :(d == direction::back_right )? direction::back
        :(d == direction::right      )? direction::back_right
        :(d == direction::front_right)? direction::right
        :direction::unknown;
         
}


constexpr int
abs(int  n) noexcept
{
  return (n < 0)? -n:n;
}


constexpr int
get_distance(direction  a, direction  b) noexcept
{
  auto  diff = abs(static_cast<int>(a)-static_cast<int>(b));

  return (diff <= 4)? diff:8-diff;
}


constexpr direction
get_farer(direction  base, direction  a, direction  b) noexcept
{
  return get_distance(base,a) < get_distance(base,b)? b:a;
}


constexpr direction
get_nearer(direction  base, direction  a, direction  b) noexcept
{
  return get_distance(base,a) < get_distance(base,b)? a:b;
}


}


using directions::direction;
using directions::get_left;
using directions::get_right;
using directions::get_opposite;
using directions::get_nearer;
using directions::get_farer;
using directions::get_distance;


}




#endif




