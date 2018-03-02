#ifndef LIBGBSND_SHARED_STRING_HPP
#define LIBGBSND_SHARED_STRING_HPP


#include<cstddef>


namespace gbscr{


class
shared_string
{
  struct private_data;

  private_data*  m_data=nullptr;

  void  unrefer() noexcept;

public:
  shared_string() noexcept{}
  shared_string(const char*  s, size_t  l) noexcept{assign(s,l);}
  shared_string(const shared_string&   rhs) noexcept{*this = rhs;}
  shared_string(      shared_string&&  rhs) noexcept{*this = std::move(rhs);}
 ~shared_string(){unrefer();}

  shared_string&  operator=(const shared_string&   rhs) noexcept;
  shared_string&  operator=(      shared_string&&  rhs) noexcept;

  const char&  operator[](int  i) const noexcept;

  bool  operator==(const shared_string&  s) const noexcept;

  shared_string&  assign(const char*  s, size_t  l) noexcept;

  const char*  data() const noexcept;
  size_t       size() const noexcept;

};


}




#endif




