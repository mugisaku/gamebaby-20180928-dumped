#ifndef LIBGBscr_process_HPP
#define LIBGBscr_process_HPP


#include"libgbscr/shared_string.hpp"
#include"libgbscr/short_string.hpp"
#include"libgbscr/token.hpp"
#include"libgbscr/list.hpp"
#include"libgbscr/value.hpp"
#include"libgbscr/expr.hpp"
#include"libgbscr/stmt.hpp"


namespace gbscr{
namespace processes{




class
constant
{
  enum class kind{
    null,
    string,
    routine,
    table,
  } m_kind=kind::null;


  union data{
    gbstd::string  s;
    routine        r;
    table          t;

    data(){}
   ~data(){}

  } m_data;

public:
  constant(gbstd::string&&  s) noexcept: m_kind(kind::string){new(&m_data) gbstd::string(std::move(s));}
  constant(routine&&  r) noexcept: m_kind(kind::routine){new(&m_data) routine(std::move(r));}
  constant(const table&  t) noexcept: m_kind(kind::table){new(&m_data) table(t);}
 ~constant(){}

  bool  is_string()  const noexcept{return m_kind == kind::string;}
  bool  is_routine() const noexcept{return m_kind == kind::routine;}
  bool  is_table()   const noexcept{return m_kind == kind::table;}

  const gbstd::string&  get_string()  const noexcept{return m_data.s;}
  const routine&        get_routine() const noexcept{return m_data.r;}
  const table&          get_table()   const noexcept{return m_data.t;}

};


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


  std::vector<std::unique_ptr<entry>>        m_entry_list;
  std::vector<std::unique_ptr<constant>>  m_constant_list;

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

  void  append_constant(constant*  c) noexcept{m_constant_list.emplace_back(c);}

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


using processes::constant;
using processes::process;


}




#endif




