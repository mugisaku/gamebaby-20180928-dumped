#ifndef gbwar_process_HPP
#define gbwar_process_HPP


#include"libgbwar/board.hpp"
#include"libgbwar/piece.hpp"


using namespace gbstd;


piece*  find_dead_piece() noexcept;


class
team
{
};


namespace processes{


class
battle: public programs::process
{

public:
  void  step() noexcept override;

};


}




#endif




