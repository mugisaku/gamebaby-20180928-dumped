#ifndef LIBGBSND_value_HPP
#define LIBGBSND_value_HPP


#include<cstdint>
#include<cstdio>
#include<memory>
#include"libgbstd/string.hpp"
#include"libgbscr/shared_string.hpp"
#include"libgbscr/list.hpp"
#include"libgbscr/token.hpp"


namespace gbscr{


namespace stmts{
class stmt;
class stmt_list;
class routine;
}


namespace values{




class table;
class variable;
class value;
class method;


class
reference
{
  variable&  m_variable;

public:
  reference(variable&  var) noexcept: m_variable(var){}

  variable&  operator()() const noexcept{return m_variable;}

};


struct
calling
{
  values::table*  table;

  const stmts::routine*  routine;

};


class
table
{
  std::vector<variable*>  m_variables;

public:
  table() noexcept{}
  table(const table&   rhs) noexcept{*this = rhs;}
  table(      table&&  rhs) noexcept{*this = std::move(rhs);}
 ~table(){clear();}

  table&  operator=(const table&   rhs) noexcept;
  table&  operator=(      table&&  rhs) noexcept;

  variable&  operator[](gbstd::string_view  name) noexcept;

  variable*  find(gbstd::string_view  name) const noexcept;

  void  clear() noexcept;

  void  carry(table&  dst) noexcept;

  void  set_carry_flag() noexcept;

  variable&  append(const value&  v, gbstd::string_view  name) noexcept;

  void  print() const noexcept;

};


struct
user_data
{
  void*  pointer;

};


class
value
{
  enum class kind{
    null,
    integer,
    string,
    reference,
    routine,
    table,
    calling,
    user_data,
    host_function,

  } m_kind=kind::null;

  union data{
    int              i;
    gbstd::string    s;
    reference        r;

    const stmts::routine*  rt;
    table*                tbl;

    data(){}
   ~data(){}

  } m_data;

public:
  value() noexcept{}
  value(bool  b) noexcept{*this = b;}
  value(int  i) noexcept{*this = i;}
  value(const gbstd::string&  s) noexcept{*this = s;}
  value(const reference&  r) noexcept{*this = r;}
  value(const stmts::routine&  rt) noexcept{*this = rt;}
  value(table&  tbl) noexcept{*this = tbl;}
  value(const value&   rhs) noexcept{*this = rhs;}
  value(      value&&  rhs) noexcept{*this = std::move(rhs);}
 ~value(){clear();}

  value&  operator=(bool  b) noexcept;
  value&  operator=(int  i) noexcept;
  value&  operator=(const gbstd::string&  s) noexcept;
  value&  operator=(const reference&  r) noexcept;
  value&  operator=(const stmts::routine&  rt) noexcept;
  value&  operator=(table&  tbl) noexcept;
  value&  operator=(const value&   rhs) noexcept;
  value&  operator=(      value&&  rhs) noexcept;

  operator bool() const noexcept{return m_kind != kind::null;}

  void  clear() noexcept;

  bool  is_null()           const noexcept{return m_kind == kind::null;}
  bool  is_reference()      const noexcept{return m_kind == kind::reference;}
  bool  is_integer()        const noexcept{return m_kind == kind::integer;}
  bool  is_string()         const noexcept{return m_kind == kind::string;}
  bool  is_routine()        const noexcept{return m_kind == kind::routine;}
  bool  is_table()         const noexcept{return m_kind == kind::table;}

  int                    get_integer()        const noexcept{return m_data.i;}
  const gbstd::string&   get_string()         const noexcept{return m_data.s;}
  const reference&       get_reference()      const noexcept{return m_data.r;}
  const stmts::routine&  get_routine()        const noexcept;
  table&                   get_table()        const noexcept{return *m_data.tbl;}

  int  get_integer_safely() const noexcept;

  void  print() const noexcept;

};


class
value_list: public list<value>
{
public:
  using list::list;

};


class
variable
{
  table*  m_table=nullptr;

  values::value  m_value;

  gbstd::string  m_name;

  bool  m_carry_flag=false;

public:
  variable() noexcept{}
  variable(table&  table, const value&  v, gbstd::string_view  name) noexcept;
  variable(const variable&   rhs) noexcept=default;
  variable(      variable&&  rhs) noexcept=delete;

  variable&  operator=(const variable&   rhs) noexcept=default;
  variable&  operator=(      variable&&  rhs) noexcept=delete;

  value&  get_value() noexcept{return m_value;}

  reference  get_reference() noexcept{return reference(*this);}

  bool   test_carry_flag() const noexcept{return m_carry_flag;}
  void    set_carry_flag()       noexcept;
  void  unset_carry_flag()       noexcept{m_carry_flag = false;}

  const gbstd::string&  get_name() const noexcept{return m_name;}

  void  print() const noexcept;

};


}


using values::reference;
using values::value;
using values::value_list;
using values::variable;
using values::table;
using values::calling;


}




#endif




