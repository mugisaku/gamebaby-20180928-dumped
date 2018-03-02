#ifndef dtree_HPP
#define dtree_HPP


#include<cstddef>
#include<cstdint>
#include"libgbscr/shared_string.hpp"


namespace  gbscr{
namespace dtrees{


class value;



class
list
{
  struct reader;

  value*  m_data=nullptr;

  uint32_t  m_number_of_values=0;

  value   read_number(tok::stream_reader&  r);
  value*  read_value_of_pair(tok::stream_reader&  r, char  cl);
  value   read_value(tok::stream_reader&  r, char  cl);

  list(tok::stream_reader&  reader, char  cl=0){assign(reader,cl);}

  void  assign(tok::stream_reader&  reader, char  cl=0);

public:
  list() noexcept{}
  list(gbstd::string_view  filepath){open(filepath);}
  list(const list&   rhs) noexcept{*this = rhs;}
  list(      list&&  rhs) noexcept{*this = std::move(rhs);}
 ~list(){clear();}


  list&  operator=(const list&   rhs) noexcept;
  list&  operator=(      list&&  rhs) noexcept;

  void  clear() noexcept;

  uint32_t  size() const noexcept{return m_number_of_values;}

  void  open(gbstd::string_view  filepath);

  const value&   get_named_value(gbstd::string_view  name) const;
  const value*  find_named_value(gbstd::string_view  name) const noexcept;
  const value*  access(std::initializer_list<const char*>  ls) const noexcept;

  void  print(int  indent=0) const noexcept;

  const value*  begin() const noexcept;
  const value*    end() const noexcept;

};


class
value
{
  enum class kind{
    null,
    string,
    integer,
    real,
    list,

  } m_kind;


  union data{
    int                i;
    double             r;
    shared_string      s;
    list              ls;

     data(){}
    ~data(){}

  } m_data;

public:
  value() noexcept{}
  value(int        i) noexcept: m_kind(kind_type::integer){m_data.i = i;}
  value(double     r) noexcept: m_kind(kind_type::real   ){m_data.r = r;}
  value(const shared_string&  s) noexcept: m_kind(kind_type::string ){new(&m_data) shared_string(s);}
  value(list&&    ls) noexcept: m_kind(kind_type::list){new(&m_data) list(std::move(ls));}
  value(const value&   rhs) noexcept{*this = rhs;}
  value(      value&&  rhs) noexcept{*this = std::move(rhs);}
 ~value(){clear();}

  value&  operator=(const value&   rhs) noexcept;
  value&  operator=(      value&&  rhs) noexcept;

  operator bool() const noexcept{return m_kind != kind_type::null;}


  bool  is_integer() const noexcept{return(m_kind == kind_type::integer);}
  bool  is_real(   ) const noexcept{return(m_kind == kind_type::real   );}
  bool  is_string( ) const noexcept{return(m_kind == kind_type::string );}
  bool  is_list(   ) const noexcept{return(m_kind == kind_type::list   );}

  void  clear() noexcept;

  int                  get_integer() const noexcept{return m_data.i;}
  double                  get_real() const noexcept{return m_data.r;}
  const shared_string&  get_string() const noexcept{return m_data.s;}
  const list&             get_list() const noexcept{return m_data.ls;}

  void  print(int  indent=0) const noexcept;

};


}


using dtrees::list;
using dtrees::value;


}





#endif




