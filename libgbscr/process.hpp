#ifndef LIBGBscr_process_HPP
#define LIBGBscr_process_HPP


#include"libgbscr/shared_string.hpp"
#include"libgbscr/short_string.hpp"
#include"libgbscr/token.hpp"
#include"libgbscr/list.hpp"
#include"libgbscr/value.hpp"
#include"libgbscr/expr.hpp"


namespace     gbscr{
namespace processes{


class
process
{
  struct private_data;
  struct        frame;

  std::vector<std::unique_ptr<variable>>  m_carried_variables;

  private_data*  m_data=nullptr;

  frame*  m_top_frame=nullptr;

  size_t  m_number_of_frames=0;

  uint32_t  m_sleeping_time=0;

  enum class state{
    not_ready,
    ready,
    sleeping,
    exited,

  } m_state=state::not_ready;


  void  finish_stmt() noexcept;

  void  return_(const value&  v) noexcept;

  void  unrefer() noexcept;

  void  call(const stmts::routine&  routine, const value_list&  argument_list, value*  return_value=nullptr) noexcept;

public:
  process() noexcept;
  process(const process&   rhs) noexcept{*this = rhs;}
  process(      process&&  rhs) noexcept{*this = std::move(rhs);}
 ~process(){unrefer();}

  process&  operator=(const process&   rhs) noexcept;
  process&  operator=(      process&&  rhs) noexcept;

  void  load_file(const char*  filepath) noexcept;

  void  clear() noexcept;

  void  prepare_call(const stmts::routine&  routine, const expr_list&  argument_list, value*  return_value=nullptr) noexcept;
  void  prepare_call(const method_calling&       mc, const expr_list&  argument_list, value*  return_value=nullptr) noexcept;

  void  call(gbstd::string_view  routine_name, const value_list&  argument_list, value*  return_value=nullptr) noexcept;

  void  append_class_info(const class_info&  ci) noexcept;

  bool  append_object(gbstd::string_view  class_name, gbstd::string_view  name, void*  data) noexcept;

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




