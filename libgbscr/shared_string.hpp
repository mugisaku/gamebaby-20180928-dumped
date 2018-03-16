#ifndef LIBGBSND_SHARED_STRING_HPP
#define LIBGBSND_SHARED_STRING_HPP


#include<cstddef>
#include"libgbstd/string_view.hpp"
#include"libgbstd/string.hpp"


namespace gbscr{


class
shared_string
{
  struct private_data;

  private_data*  m_data=nullptr;

  size_t  m_length=0;

  void  unrefer() noexcept;

public:
  shared_string() noexcept{}
  shared_string(const char*  s, size_t  l) noexcept{assign(gbstd::string_view(s,l));}
  shared_string(gbstd::string_view  sv) noexcept{assign(gbstd::string(sv));}
  shared_string(gbstd::string&&  s) noexcept{assign(std::move(s));}
  shared_string(const shared_string&   rhs) noexcept{*this = rhs;}
  shared_string(      shared_string&&  rhs) noexcept{*this = std::move(rhs);}
 ~shared_string(){unrefer();}

  shared_string&  operator=(const shared_string&   rhs) noexcept;
  shared_string&  operator=(      shared_string&&  rhs) noexcept;

  shared_string&  operator=(gbstd::string_view  sv) noexcept{return assign(sv);}

  shared_string&  operator+=(const shared_string&  rhs) noexcept{return append(rhs.view());}
  shared_string&  operator+=(char  c) noexcept{return append(c);}
  shared_string&  operator+=(gbstd::string_view  sv) noexcept{return append(sv);}

  const char&  operator[](int  i) const noexcept;

  bool  operator==(const shared_string&  rhs) const noexcept{return view() == rhs.view();}
  bool  operator!=(const shared_string&  rhs) const noexcept{return view() != rhs.view();}

  shared_string&  assign(gbstd::string_view  sv) noexcept{return assign(gbstd::string(sv));}
  shared_string&  assign(gbstd::string&&  s) noexcept;

  const char*  data() const noexcept;
  size_t       size() const noexcept{return m_length;}

  shared_string&  append(char  c) noexcept{return append(gbstd::string_view(&c,1));}
  shared_string&  append(gbstd::string_view  sv) noexcept;

  gbstd::string_view  view() const noexcept{return gbstd::string_view(data(),size());}

  const char*  begin() const noexcept{return data();}
  const char*    end() const noexcept{return data()+m_length;}

  void  print() const noexcept;
  void  print_detail() const noexcept;

};


}




#endif




