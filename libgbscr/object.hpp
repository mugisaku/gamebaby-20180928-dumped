#ifndef LIBGBSND_OBJECT_HPP
#define LIBGBSND_OBJECT_HPP


#include<cstdint>
#include<cstdio>
#include<memory>
#include"libgbstd/string.hpp"
#include"libgbscr/shared_string.hpp"
#include"libgbscr/list.hpp"
#include"libgbscr/token.hpp"


namespace gbscr{


class execution_context;


class argument_list;


namespace stmts{
class stmt;
class stmt_list;
class routine;
}


namespace objects{




class object;
class variable_table;
class value;


class
reference
{
protected:
  struct private_data;

  private_data*  m_data=nullptr;

  void  unrefer() noexcept;

  void  unset_pointer() noexcept;

  size_t  get_count() const noexcept;

  reference(value&  v) noexcept;

public:
  reference() noexcept;
  reference(const reference&   rhs) noexcept{*this = rhs;}
  reference(      reference&&  rhs) noexcept{*this = std::move(rhs);}
 ~reference(){unrefer();}

  reference&  operator=(const reference&   rhs) noexcept;
  reference&  operator=(      reference&&  rhs) noexcept;

  operator bool() const noexcept;

  value&  operator()() const noexcept;

};


struct system{};


class
method_calling
{
  void*  m_object;
  void*  m_method;

};


class
value
{
  enum class kind{
    null,
    integer,
    reference,
    routine,
    object,
    
    system,

  } m_kind=kind::null;

  union data{
    int              i;
    reference        r;
    const stmts::routine*  rt;

    data(){}
   ~data(){}

  } m_data;

public:
  value() noexcept{}
  value(bool  b) noexcept{*this = b;}
  value(int  i) noexcept{*this = i;}
  value(reference  r) noexcept{*this = r;}
  value(const stmts::routine&  rt) noexcept{*this = rt;}
  value(system  sys) noexcept{*this = sys;}
  value(const value&   rhs) noexcept{*this = rhs;}
  value(      value&&  rhs) noexcept{*this = std::move(rhs);}
 ~value(){clear();}

  value&  operator=(bool  b) noexcept;
  value&  operator=(int  i) noexcept;
  value&  operator=(reference  r) noexcept;
  value&  operator=(const stmts::routine&  rt) noexcept;
  value&  operator=(system  sys) noexcept;
  value&  operator=(const value&   rhs) noexcept;
  value&  operator=(      value&&  rhs) noexcept;

  operator bool() const noexcept{return m_kind != kind::null;}

  void  clear() noexcept;

  bool  is_reference()   const noexcept{return m_kind == kind::reference;}
  bool  is_integer()     const noexcept{return m_kind == kind::integer;}
  bool  is_routine()     const noexcept{return m_kind == kind::routine;}
  bool  is_system()      const noexcept{return m_kind == kind::system;}

  int                get_integer()     const noexcept{return m_data.i;}
  reference          get_reference()   const noexcept{return m_data.r;}
  const stmts::routine&  get_routine()     const noexcept;

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
variable_table
{
  struct variable: public reference{
    objects::value  value;
    gbstd::string    name;

     variable() noexcept;
    ~variable();

    void  print() const noexcept;

  };


  std::vector<variable*>  m_variables;

public:
  variable_table() noexcept{}
 ~variable_table(){clear();}

  reference  operator[](gbstd::string_view  name) const noexcept;
  reference  operator[](int  i) const noexcept;

  void  clear() noexcept;

  reference  append(const value&  value, gbstd::string_view  name) noexcept;

  int  find(gbstd::string_view  name) const noexcept;

  void  print() const noexcept;

};


using method = value  (*)(void*  ptr, const argument_list&  argls);


class
class_info
{
  gbstd::string  m_name;

  size_t  m_size;

  void  (*m_constructor)(void*  ptr, const argument_list&  ls)=nullptr;
  void  (*m_destructor )(void*  ptr                          )=nullptr;

  std::vector<method>  m_method_list;

public:


};


class
object
{
  class_info  m_info;

  void*  m_data=nullptr;

  bool  m_is_allocated_by_host;

public:
};


}


using objects::reference;
using objects::value;
using objects::value_list;
using objects::variable_table;
using objects::class_info;
using objects::object;
using objects::method;


}




#endif




