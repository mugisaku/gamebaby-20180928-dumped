#ifndef LIBGBSND_table_HPP
#define LIBGBSND_table_HPP


#include<cstdint>
#include<cstdio>
#include<memory>
#include<vector>
#include"libgbscr/expr.hpp"
#include"libgbscr/stmt.hpp"


namespace gbscr{
namespace tables{




class table;


class
variable
{
  table*  m_table=nullptr;

  operand  m_operand;

  gbstd::string  m_name;

  variable(const operand&  o, gbstd::string_view  name) noexcept:
  m_operand(o),
  m_name(name){}

  variable(operand&&  o, gbstd::string_view  name) noexcept:
  m_operand(std::move(o)),
  m_name(name){}

public:
  variable(const variable&   rhs) noexcept=delete;
  variable(      variable&&  rhs) noexcept=delete;

  variable&  operator=(const variable&   rhs) noexcept=delete;
  variable&  operator=(      variable&&  rhs) noexcept=delete;

  void    set_table(table&  tbl)       noexcept{        m_table = &tbl;}
  table&  get_table(           ) const noexcept{return *m_table       ;}

  void            set_operand(const operand&  o)       noexcept{       m_operand = o;}
  const operand&  get_operand(                 ) const noexcept{return m_operand    ;}

  reference  get_reference() noexcept{return reference(*this);}

  void                  set_name(gbstd::string_view  name)       noexcept{       m_name = name;}
  const gbstd::string&  get_name(                        ) const noexcept{return m_name       ;}

  void  print() const noexcept;

  static variable*  create_instance(operand&&  o, gbstd::string_view  name) noexcept{return new variable(std::move(o),name);}
  static variable*    copy_instance(const variable&  src) noexcept{return new variable(src.m_operand,src.m_name);}

};


bool  read_variable(cursor&  cur, variable*&  var, processes::process&  proc);


class
table
{
  std::vector<std::unique_ptr<variable>>  m_variables;

public:
  table() noexcept{}
  table(const table&   rhs)         =default;
  table(      table&&  rhs) noexcept=default;
  table(const block&  blk, processes::process&  proc){load(blk,proc);}
 ~table(){clear();}

  reference  operator[](gbstd::string_view  name) noexcept;

  reference  find(gbstd::string_view  name) const noexcept;

  void  clear() noexcept;

  table&  load(const block&  blk, processes::process&  proc);

  reference  append(const value&  v, gbstd::string_view  name) noexcept;

  void  print() const noexcept;

  static table  clone(const table&  src) noexcept;

};


}


using tables::variable;
using tables::table;


}




#endif




