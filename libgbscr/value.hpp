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




class object;
class class_info;
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
method_calling
{
  void*  data;

  const values::method*  method;

};


class
object
{
  const class_info&  m_info;

  void*  m_data;

public:
  object(const class_info&  info, void*  data) noexcept:
  m_info(info), m_data(data){}

  const class_info&  get_class_info() const noexcept{return m_info;}

  method_calling  operator[](gbstd::string_view  name) const noexcept;

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
    object,
    method_calling,

  } m_kind=kind::null;

  union data{
    int              i;
    gbstd::string    s;
    reference        r;

    const stmts::routine*  rt;
    object                obj;
    method_calling         mc;

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
  value(const method_calling&  mc) noexcept{*this = mc;}
  value(const object&  obj) noexcept{*this = obj;}
  value(const value&   rhs) noexcept{*this = rhs;}
  value(      value&&  rhs) noexcept{*this = std::move(rhs);}
 ~value(){clear();}

  value&  operator=(bool  b) noexcept;
  value&  operator=(int  i) noexcept;
  value&  operator=(const gbstd::string&  s) noexcept;
  value&  operator=(const reference&  r) noexcept;
  value&  operator=(const stmts::routine&  rt) noexcept;
  value&  operator=(const method_calling&  mc) noexcept;
  value&  operator=(const object&  obj) noexcept;
  value&  operator=(const value&   rhs) noexcept;
  value&  operator=(      value&&  rhs) noexcept;

  operator bool() const noexcept{return m_kind != kind::null;}

  void  clear() noexcept;

  bool  is_null()           const noexcept{return m_kind == kind::null;}
  bool  is_reference()      const noexcept{return m_kind == kind::reference;}
  bool  is_integer()        const noexcept{return m_kind == kind::integer;}
  bool  is_string()         const noexcept{return m_kind == kind::string;}
  bool  is_routine()        const noexcept{return m_kind == kind::routine;}
  bool  is_object()         const noexcept{return m_kind == kind::object;}
  bool  is_method_calling() const noexcept{return m_kind == kind::method_calling;}

  int                    get_integer()        const noexcept{return m_data.i;}
  const gbstd::string&   get_string()         const noexcept{return m_data.s;}
  const reference&       get_reference()      const noexcept{return m_data.r;}
  const stmts::routine&  get_routine()        const noexcept;
  const object&          get_object()         const noexcept{return m_data.obj;}
  const method_calling&  get_method_calling() const noexcept{return m_data.mc;}

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
  values::value  m_value;

  gbstd::string  m_name;

  bool  m_carry_flag=false;

public:
  variable() noexcept{}
  variable(const value&  v, gbstd::string_view  name) noexcept;
  variable(const variable&   rhs) noexcept=delete;
  variable(      variable&&  rhs) noexcept=delete;

  variable&  operator=(const variable&   rhs) noexcept=delete;
  variable&  operator=(      variable&&  rhs) noexcept=delete;

  value&  get_value() noexcept{return m_value;}

  reference  get_reference() noexcept{return reference(*this);}

  bool   test_carry_flag() const noexcept{return m_carry_flag;}
  void    set_carry_flag()       noexcept;
  void  unset_carry_flag()       noexcept{m_carry_flag = false;}

  const gbstd::string&  get_name() const noexcept{return m_name;}

  void  print() const noexcept;

};


using callback = value  (*)(void*  ptr, const value_list&  argls);


class
method
{
  gbstd::string_view  m_name;

  callback  m_callback=nullptr;

public:
  constexpr method() noexcept{}
  constexpr method(gbstd::string_view  name, callback  cb) noexcept:
  m_name(name),
  m_callback(cb){}

  template<typename  T>
  constexpr method(gbstd::string_view  name, value  (*cb)(T*  data, const value_list&  argls)) noexcept:
  m_name(name),
  m_callback(reinterpret_cast<callback>(cb)){}

  constexpr const gbstd::string_view&  get_name() const noexcept{return m_name;}

  constexpr callback  operator*() const noexcept{return m_callback;}

};


class
class_info
{
  gbstd::string  m_name;

  std::vector<method>  m_method_list;

public:
  class_info(gbstd::string_view  name, std::initializer_list<method>  ls) noexcept:
  m_name(name), m_method_list(ls){}

  class_info(const class_info&   rhs) noexcept=delete;
  class_info(      class_info&&  rhs) noexcept=delete;

  class_info&  operator=(const class_info&   rhs) noexcept=delete;
  class_info&  operator=(      class_info&&  rhs) noexcept=delete;

  const gbstd::string&  get_name() const noexcept{return m_name;}

  const method*  find_method(gbstd::string_view  name) const noexcept;

};


}


using values::reference;
using values::value;
using values::value_list;
using values::variable;
using values::class_info;
using values::object;
using values::method;
using values::method_calling;


}




#endif




