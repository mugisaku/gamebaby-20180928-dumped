#ifndef LIBGBSND_SHARED_STRING_HPP
#define LIBGBSND_SHARED_STRING_HPP


#include"libgbstd/string.hpp"


namespace gbsnd{


class
shared_string
{
  struct private_data;

  private_data*  m_data=nullptr;

  void  unrefer() noexcept;

public:
  shared_string() noexcept{}
  shared_string(gbstd::string_view  sv) noexcept{*this = sv;}
  shared_string(const shared_string&   rhs) noexcept{*this = rhs;}
  shared_string(      shared_string&&  rhs) noexcept{*this = std::move(rhs);}
 ~shared_string(){unrefer();}

  shared_string&  operator=(gbstd::string_view  sv) noexcept;
  shared_string&  operator=(const shared_string&   rhs) noexcept;
  shared_string&  operator=(      shared_string&&  rhs) noexcept;

  bool  operator==(gbstd::string_view  sv) const noexcept;

  const char*  data() const noexcept;
  size_t       size() const noexcept;

  gbstd::string_view  view() const noexcept;

};


using identifier = shared_string;


}




#endif




