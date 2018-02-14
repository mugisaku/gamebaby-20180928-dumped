#include"libgbsnd/shared_string.hpp"
#include<cstdlib>
#include<cstring>


namespace gbsnd{


struct
shared_string::
private_data
{
  size_t  reference_count;

  size_t  length;

  char  data[0];

};




void
shared_string::
unrefer() noexcept
{
    if(m_data)
    {
        if(!--m_data->reference_count)
        {
          free(m_data)         ;
               m_data = nullptr;
        }
    }
}




shared_string&
shared_string::
operator=(gbstd::string_view  sv) noexcept
{
  unrefer();

  m_data = static_cast<private_data*>(malloc(sizeof(private_data)+sv.size()+1));

  m_data->reference_count = 1;

  m_data->length = sv.size();

  std::memcpy(m_data->data,sv.data(),sv.size());

  m_data->data[sv.size()] = 0;


  return *this;
}


shared_string&
shared_string::
operator=(const shared_string&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      m_data = rhs.m_data;

        if(m_data)
        {
          ++m_data->reference_count;
        }
    }


  return *this;
}


shared_string&
shared_string::
operator=(shared_string&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      std::swap(m_data,rhs.m_data);
    }


  return *this;
}




bool
shared_string::
operator==(gbstd::string_view  sv) const noexcept
{
  return gbstd::string_view(m_data->data,m_data->length) == sv;
}


const char*
shared_string::
data() const noexcept
{
  return m_data? m_data->data:"";
}


size_t
shared_string::
size() const noexcept
{
  return m_data? m_data->length:0;
}


gbstd::string_view
shared_string::
view() const noexcept
{
  static gbstd::string_view  null("");

  return m_data? gbstd::string_view(m_data->data,m_data->length):null;
}




}




