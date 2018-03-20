#ifndef LIBGBscr_process_HPP
#define LIBGBscr_process_HPP


#include"libgbscr/shared_string.hpp"
#include"libgbscr/short_string.hpp"
#include"libgbscr/token.hpp"
#include"libgbscr/list.hpp"
#include"libgbscr/value.hpp"
#include"libgbscr/expr.hpp"


namespace gbscr{


namespace stmts{
class routine;
}


namespace processes{


class
process
{
  struct frame;

  frame*  m_top_frame=nullptr;

  size_t  m_number_of_frames=0;


  struct entry{
    gbstd::string  file_path;
    gbstd::string  file_content;

    tokens::block  block;

  };


  std::vector<std::unique_ptr<entry>>             m_entry_list;
  std::vector<std::unique_ptr<stmts::routine>>  m_routine_list;

  table  m_global_table;

  uint32_t  m_sleeping_time=0;

  enum class state{
    not_ready,
    ready,
    sleeping,
    exited,

  } m_state=state::not_ready;


  static std::unique_ptr<entry>  create_entry(const char*  filepath) noexcept;

  void  finish_stmt() noexcept;

  void  return_(const value&  v) noexcept;

  void  call(const stmts::routine&  routine, const value_list&  argument_list, value*  return_value=nullptr) noexcept;

public:
  process() noexcept;
  process(const process&   rhs) noexcept=delete;
  process(      process&&  rhs) noexcept=delete;
 ~process();

  process&  operator=(const process&   rhs) noexcept=delete;
  process&  operator=(      process&&  rhs) noexcept=delete;

  void  load_file(const char*  filepath) noexcept;

  void  clear() noexcept;

  void  prepare_call(const stmts::routine&  routine, const expr_list&  argument_list, value*  return_value=nullptr) noexcept;
//  void  prepare_call(const method_calling&       mc, const expr_list&  argument_list, value*  return_value=nullptr) noexcept;

  void  call(gbstd::string_view  routine_name, const value_list&  argument_list, value*  return_value=nullptr) noexcept;

  bool  append_variable(const value&  value, gbstd::string_view  name) noexcept;

  const table&  get_global_table() const noexcept{return m_global_table;}

  size_t  get_number_of_frames() const noexcept{return m_number_of_frames;}

  void  resize(size_t  n) noexcept;

  value  get_value(gbstd::string_view  name) const noexcept;

  bool  is_not_ready() const noexcept{return m_state == state::not_ready;}
  bool  is_ready()     const noexcept{return m_state == state::ready;}
  bool  is_sleeping()  const noexcept{return m_state == state::sleeping;}
  bool  is_exited()    const noexcept{return m_state == state::exited;}

  void  step() noexcept;
  void   run() noexcept;

  void  print() const noexcept;

};


}


using processes::process;


}




#endif




