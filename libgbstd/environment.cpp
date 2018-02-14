#include"environment.hpp"
#include<vector>






namespace gbstd{
namespace environment{


unsigned int
count_of_report;


namespace{


//std::vector<gbstd::string_view,gbstd::string>
//variables;


}


int
fn_keystate;


void
set_value(gbstd::string_view  name, gbstd::string_view  value) noexcept
{
//  variables[name] = std::string(value);
}


const gbstd::string&
get_value(gbstd::string_view  name) noexcept
{
static gbstd::string  s;
//  return variables[0];
  return s;
}


}}




