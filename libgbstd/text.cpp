#include"libgbstd/text.hpp"




namespace gbstd{
namespace texts{


namespace{


constexpr size_t  buffer_size = 1024;

char  buffer[buffer_size];


}


gbstd::string_view
make_text(const char*  fmt, ...) noexcept
{
  va_list  ap;
  va_start(ap,fmt);

  auto  sv = make_text_with_va_list(fmt,ap);

  va_end(ap);

  return sv;
}


gbstd::string_view
make_text_with_va_list(const char*  fmt, va_list  ap) noexcept
{
  auto  n = vsnprintf(buffer,sizeof(buffer),fmt,ap);

    if(n >= buffer_size)
    {
      return gbstd::string_view("バッファが足りない");
    }


  return gbstd::string_view(buffer,n);
}


}}




