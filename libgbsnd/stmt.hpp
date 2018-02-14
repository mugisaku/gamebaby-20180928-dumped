#ifndef LIBGBSND_STMT_HPP
#define LIBGBSND_STMT_HPP


#include"libgbsnd/expr.hpp"


namespace gbsnd{


namespace scripts{
class script_token_string;
class script_token_cursor;
}


namespace stmts{




enum class
stmt_kind
{
  null,
  evaluate_and_dump,
  evaluate_and_save,
  evaluate_and_zero,
  evaluate_and_not_zero,
  evaluate_and_equal,
  evaluate_and_not_equal,
  label,
  sleep,
  print,
  return_,
  exit,
  jump,
  jump_by_condition,

};


class
stmt
{
  stmt_kind  m_kind=stmt_kind::null;

  expr  m_expr;

  gbstd::string  m_label;

public:
  stmt() noexcept{}
  stmt(stmt_kind  k, expr&&  expr)        noexcept: m_kind(k), m_expr(std::move(expr)){}
  stmt(stmt_kind  k, gbstd::string_view  label) noexcept: m_kind(k), m_label(label){}

  operator bool() const noexcept{return m_kind != stmt_kind::null;}

  bool  is_evaluate_and_dump()      const noexcept{return m_kind == stmt_kind::evaluate_and_dump;}
  bool  is_evaluate_and_save()      const noexcept{return m_kind == stmt_kind::evaluate_and_save;}
  bool  is_evaluate_and_zero()      const noexcept{return m_kind == stmt_kind::evaluate_and_zero;}
  bool  is_evaluate_and_not_zero()  const noexcept{return m_kind == stmt_kind::evaluate_and_not_zero;}
  bool  is_evaluate_and_equal()     const noexcept{return m_kind == stmt_kind::evaluate_and_equal;}
  bool  is_evaluate_and_not_equal() const noexcept{return m_kind == stmt_kind::evaluate_and_not_equal;}
  bool  is_return()                 const noexcept{return m_kind == stmt_kind::return_;}
  bool  is_label()                  const noexcept{return m_kind == stmt_kind::label;}
  bool  is_sleep()                  const noexcept{return m_kind == stmt_kind::sleep;}
  bool  is_print()                  const noexcept{return m_kind == stmt_kind::print;}
  bool  is_exit()                   const noexcept{return m_kind == stmt_kind::exit;}
  bool  is_jump()                   const noexcept{return m_kind == stmt_kind::jump;}
  bool  is_jump_by_condition()      const noexcept{return m_kind == stmt_kind::jump_by_condition;}

  const expr&     get_expr()  const noexcept{return m_expr;}
  const gbstd::string&  get_label() const noexcept{return m_label;}

  void  print() const noexcept;

};


class
stmt_list: public list<stmt>
{
public:
  using list::list;

  stmt_list(const scripts::script_token_string&  toks) noexcept;

};


class
parameter_list: public list<gbstd::string>
{
public:
  using list::list;
};


class
routine
{
  parameter_list  m_parameter_list;
  stmt_list            m_stmt_list;

public:
  routine() noexcept{}
  routine(const scripts::script_token_string&  parals_src,
          const scripts::script_token_string&  blk_src) noexcept;

  const parameter_list&  get_parameter_list() const noexcept{return m_parameter_list;}
  const stmt_list&            get_stmt_list() const noexcept{return m_stmt_list;}

  void  print() const noexcept;

};


}


using stmts::stmt;
using stmts::stmt_list;
using stmts::parameter_list;
using stmts::routine;


}




#endif




