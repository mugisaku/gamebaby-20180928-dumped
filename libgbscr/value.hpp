#ifndef LIBGBSND_value_HPP
#define LIBGBSND_value_HPP


#include<cstdint>
#include<cstdio>
#include<memory>
#include<vector>
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


namespace exprs{
class operand;
}


namespace tables{
class variable;
class table;
}


namespace processes{
class process;
}


namespace values{




class value;
class method;


class
reference
{
  tables::variable*  m_pointer;

public:
  constexpr reference(tables::variable*  var) noexcept: m_pointer( var){}
  constexpr reference(tables::variable&  var) noexcept: m_pointer(&var){}

  constexpr bool  operator==(const reference&  rhs) const noexcept{return m_pointer == rhs.m_pointer;}
  constexpr bool  operator!=(const reference&  rhs) const noexcept{return m_pointer != rhs.m_pointer;}

  constexpr  operator bool() const noexcept{return m_pointer;}

  constexpr tables::variable*  operator->() const noexcept{return  m_pointer;}
  constexpr tables::variable&  operator *() const noexcept{return *m_pointer;}

};


struct
value_conversion_error
{
};


class
value
{
  struct private_data;

  private_data*  m_data=nullptr;


  static private_data*  stock;

  static private_data*  pop() noexcept;
  static void           push(private_data*  ptr) noexcept;

  void  unrefer() noexcept;
  void  unrefer_if_not_unique() noexcept;

public:
  value() noexcept;
  value(bool  b) noexcept{*this = b;}
  value(int  i) noexcept{*this = i;}
  value(gbstd::string&&  s) noexcept{*this = std::move(s);}
  value(const reference&  r) noexcept{*this = r;}
  value(stmts::routine&&  rt) noexcept{*this = std::move(rt);}
  value(tables::table&&  tbl) noexcept{*this = std::move(tbl);}
  value(const value&   rhs) noexcept{*this = rhs;}
  value(      value&&  rhs) noexcept{*this = std::move(rhs);}
 ~value(){unrefer();}	

  value&  operator=(bool  b) noexcept;
  value&  operator=(int  i) noexcept;
  value&  operator=(gbstd::string&&  s) noexcept;
  value&  operator=(const reference&  r) noexcept;
  value&  operator=(stmts::routine&&  rt) noexcept;
  value&  operator=(tables::table&&  tbl) noexcept;
  value&  operator=(const value&   rhs) noexcept;
  value&  operator=(      value&&  rhs) noexcept;

  operator bool() const noexcept;

  bool  is_null()      const noexcept;
  bool  is_boolean()   const noexcept;
  bool  is_reference() const noexcept;
  bool  is_integer()   const noexcept;
  bool  is_string()    const noexcept;
  bool  is_routine()   const noexcept;
  bool  is_table()     const noexcept;

  bool                   get_boolean()   const noexcept;
  int                    get_integer()   const noexcept;
  const gbstd::string&   get_string()    const noexcept;
  const reference&       get_reference() const noexcept;
  const stmts::routine&  get_routine()   const noexcept;
  tables::table&         get_table()     const noexcept;

  bool                   convert_to_boolean()   const;
  int                    convert_to_integer()   const;
  shared_string          convert_to_string()    const;
  const stmts::routine&  convert_to_routine()   const;

  void  print() const noexcept;

};


class
value_list: public list<value>
{
public:
  using list::list;

};




}


using values::reference;
using values::value;
using values::value_conversion_error;
using values::value_list;


}




#endif




