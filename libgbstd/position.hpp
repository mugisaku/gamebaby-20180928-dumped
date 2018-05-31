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

};




constexpr position
get_opposite(position  pos) noexcept
{
  return( (pos == position::top  )? position::bottom
         :(pos == position::left )? position::right
         :(pos == position::right)? position::left
         :                          position::top);
}


}


using positions::position;
using positions::get_opposite;


}




#endif




