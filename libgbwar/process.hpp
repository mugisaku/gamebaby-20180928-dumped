#ifndef gbwar_process_HPP
#define gbwar_process_HPP


#include"libgbwar/board.hpp"
#include"libgbwar/piece.hpp"


using namespace gbstd;


piece*  find_dead_piece() noexcept;


class
team
{
  std::vector<piece*>  m_piece_list;
  std::vector<piece*>  m_buffer;

public:


};


namespace processes{


class
battle: public programs::process
{
  team*  m_first_team;
  team*  m_second_team;

public:
  void  step() noexcept override;

};


}




#endif




