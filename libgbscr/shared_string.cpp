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




void
shared_string::
unrefer() noexcept
{
    if(m_data)
    {
        if(!--m_data->reference_count)
        {
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
        }
    }

  else
    {
      m_data = new private_data;

      m_data->reference_count = 1;
    }


  m_data->string.assign(std::move(s));

  m_length = s.size();

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




void
shared_string::
append(gbstd::string_view  sv) noexcept
{
    if(m_data && (m_data->string.size() == m_length))
    {
      m_data->string.append(sv);

      m_length += sv.size();
    }

  else
    {
      assign(sv);
    }
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




shared_string
make_string_from_file(const char*  filepath) noexcept
{
  shared_string  s;

  auto  f = fopen(filepath,"rb");

    if(f)
    {
      gbstd::string  buf;

        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f) || ferror(f))
            {
              break;
            }


          buf += c;
        }


      fclose(f);

      s.assign(std::move(buf));
    }


  return std::move(s);
}




}




