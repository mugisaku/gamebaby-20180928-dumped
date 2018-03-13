#include"libgbscr/value.hpp"


namespace gbscr{
namespace values{




table&
table::
operator=(const table&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

        for(auto  ptr: rhs.m_variables)
        {
          m_variables.emplace_back(new variable(*ptr));
        }
    }


  return *this;
}


table&
table::
operator=(table&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_variables,rhs.m_variables);
    }


  return *this;
}


variable&
table::
operator[](gbstd::string_view  name) noexcept
{
    for(auto  ptr: m_variables)
    {
        if(ptr->get_name() == name)
        {
          return *ptr;
        }
    }


  auto  var = new variable(value(),name);

  m_variables.emplace_back(var);

  return *var;
}




void
table::
clear() noexcept
{
    for(auto  ptr: m_variables)
    {
      delete ptr;
    }


  m_variables.clear();
}


void
table::
append(const value&  v, gbstd::string_view  name) noexcept
{
  m_variables.emplace_back(new variable(v,name));
}


}}




