#ifndef LIBGBscr_process_HPP
#define LIBGBscr_process_HPP


#include"libgbscr/shared_string.hpp"
#include"libgbscr/short_string.hpp"
#include"libgbscr/token.hpp"
#include"libgbscr/list.hpp"
#include"libgbscr/stmt.hpp"
#include"libgbscr/object.hpp"


namespace     gbscr{
namespace processes{


class
process
{
  struct private_data;

  private_data*  m_data=nullptr;

  void  unrefer() noexcept;

public:
  process() noexcept;
  process(const process&   rhs) noexcept{*this = rhs;}
  process(      process&&  rhs) noexcept{*this = std::move(rhs);}
 ~process(){unrefer();}

  process&  operator=(const process&   rhs) noexcept;
  process&  operator=(      process&&  rhs) noexcept;

  void  load_file(const char*  filepath) noexcept;

  const std::vector<std::unique_ptr<routine>>&          get_routine_list() const noexcept;

  const routine*  find_routine(gbstd::string_view  name) const noexcept;

  void  print() const noexcept;

};


}


using processes::process;


}




#endif




