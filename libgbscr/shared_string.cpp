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
          free(m_data);
        }


      m_data = nullptr;
    }
}




shared_string&
shared_string::
assign(const char*  s, size_t  l) noexcept
{
  unrefer();

  m_data = static_cast<private_data*>(malloc(sizeof(private_data)+l+1));

  m_data->reference_count = 1;

  m_data->length = l;

  std::memcpy(m_data->data,s,l);

  m_data->data[l] = 0;


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
operator==(const shared_string&  rhs) const noexcept
{
  return (size() != rhs.size())? false:(std::strncmp(data(),rhs.data(),size()) == 0);
}


const char&
shared_string::
operator[](int  i) const noexcept
{
  return (i < m_data->length)? m_data->data[i]:null;
}




const char*
shared_string::
data() const noexcept
{
  return m_data? m_data->data:&null;
}


size_t
shared_string::
size() const noexcept
{
  return m_data? m_data->length:0;
}




shared_string
make_string_from_file(const char*  filepath) noexcept
{
  shared_string  s;

  auto  f = fopen(filepath,"rb");

    if(f)
    {
      std::string  buf;

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

      s.assign(buf.data(),buf.size());
    }


  return std::move(s);
}




}




