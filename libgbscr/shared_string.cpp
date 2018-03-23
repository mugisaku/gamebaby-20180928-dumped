#include"libgbscr/shared_string.hpp"
#include<cstdlib>
#include<cstring>
#include<string>
#include<cstdio>


namespace gbscr{


namespace{
const char  null = 0;
}


struct
shared_string::
private_data
{
  size_t  reference_count;

  gbstd::string  string;

};




shared_string::
shared_string() noexcept:
m_data(new private_data)
{
  m_data->reference_count = 1;
}




void
shared_string::
unrefer() noexcept
{
    if(m_data)
    {
        if(!--m_data->reference_count)
        {
//printf("%p is deleted.\n",m_data);
          delete m_data;
        }


      m_data = nullptr;
      m_length = 0;
    }
}




shared_string&
shared_string::
assign(gbstd::string&&  s) noexcept
{
    if(m_data)
    {
        if(m_data->reference_count > 1)
        {
          unrefer();

          m_data = new private_data;

          m_data->reference_count = 1;
        }
    }

  else
    {
      m_data = new private_data;

      m_data->reference_count = 1;
    }


  m_data->string.assign(std::move(s));

  m_length = m_data->string.size();

  return *this;
}


shared_string&
shared_string::
operator=(const shared_string&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      m_data   = rhs.m_data;
      m_length = rhs.m_length;

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
      std::swap(m_length,rhs.m_length);
    }


  return *this;
}




const char&
shared_string::
operator[](int  i) const noexcept
{
  return (i < m_length)? m_data->string[i]:null;
}




const char*
shared_string::
data() const noexcept
{
  return m_data? m_data->string.data():&null;
}




shared_string&
shared_string::
append(gbstd::string_view  sv) noexcept
{
    if(m_data)
    {
        if(m_data->string.size() == m_length)
        {
          m_data->string.append(sv);

          m_length += sv.size();
        }

      else
        {
          gbstd::string_view  base_sv(m_data->string.data(),m_length);

          assign(base_sv);

          append(sv);
        }
    }

  else
    {
      assign(sv);
    }


  return *this;
}


void
shared_string::
print() const noexcept
{
    for(auto  c: *this)
    {
      printf("%c",c);
    }
}


void
shared_string::
print_detail() const noexcept
{
    if(m_data)
    {
      printf("%p(%4d)[%4d]",m_data,m_data->reference_count,m_length);
    }

  else
    {
      printf("0x00000000(   0) ");
    }
}




}




