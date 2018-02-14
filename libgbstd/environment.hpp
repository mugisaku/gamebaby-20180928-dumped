#ifndef GMBB_ENVIRONMENT_HPP_INCLUDED
#define GMBB_ENVIRONMENT_HPP_INCLUDED


#include"libgbstd/string.hpp"




namespace gbstd{
namespace environment{


constexpr int  fn1_flag = 0x80;
constexpr int  fn2_flag = 0x40;
constexpr int  fn3_flag = 0x20;
constexpr int  fn4_flag = 0x10;
constexpr int  fn5_flag = 0x08;
constexpr int  fn6_flag = 0x04;
constexpr int  fn7_flag = 0x02;
constexpr int  fn8_flag = 0x01;

extern int  fn_keystate;


void                  set_value(gbstd::string_view  name, gbstd::string_view  value) noexcept;
const gbstd::string&  get_value(gbstd::string_view  name                           ) noexcept;


}}



#endif




