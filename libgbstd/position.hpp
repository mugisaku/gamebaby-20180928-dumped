#ifndef gmbb_position_HPP
#define gmbb_position_HPP


namespace gbstd{
namespace positions{


enum class
position
{
     top,
    left,
   right,
  bottom,
    none,

};




constexpr position
get_opposite(position  pos) noexcept
{
  return( (pos == position::top   )? position::bottom
         :(pos == position::left  )? position::right
         :(pos == position::right )? position::left
         :(pos == position::bottom)? position::top
         :                           position::none);
}


}


using positions::position;
using positions::get_opposite;


}




#endif




