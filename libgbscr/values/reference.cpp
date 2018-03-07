#include"libgbscr/value.hpp"


namespace gbscr{
namespace values{




struct
reference::
private_data
{
  size_t  count;

  value*  pointer;

};




reference::
reference() noexcept:
m_data(new private_data)
{
  static value  null;

  m_data->count = 1;

  m_data->pointer = &null;
}


reference::
reference(value&  v) noexcept:
m_data(new private_data)
{
  m_data->count = 1;

  m_data->pointer = &v;
}




reference&
reference::
operator=(const reference&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      m_data = rhs.m_data;

        if(m_data)
        {
//printf("%3d\n",m_data->count+1);
          ++m_data->count;
        }
    }


  return *this;
}


reference&
reference::
operator=(reference&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      std::swap(m_data,rhs.m_data);
    }


  return *this;
}


reference::
operator bool() const noexcept
{
  return m_data->pointer;
}


value&
reference::
operator()() const noexcept
{
  return *m_data->pointer;
}




void
reference::
unrefer() noexcept
{
    if(m_data)
    {
//printf("%3d\n",m_data->count-1);
        if(!--m_data->count)
        {
          delete m_data;
        }


      m_data = nullptr;
    }
}


size_t
reference::
get_count() const noexcept
{
  return m_data->count;
}


void
reference::
unset_pointer() noexcept
{
  m_data->pointer = nullptr;
}




}}




