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


  auto  var = new variable(*this,value(),name);

  m_variables.emplace_back(var);

  return *var;
}




variable*
table::
find(gbstd::string_view  name) const noexcept
{
    for(auto  ptr: m_variables)
    {
        if(ptr->get_name() == name)
        {
          return ptr;
        }
    }


  return nullptr;
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
carry(table&  dst) noexcept
{
  std::vector<variable*>  new_buf;

    for(auto  ptr: m_variables)
    {
      auto&  buf = ptr->test_carry_flag()? dst.m_variables:new_buf;

      buf.emplace_back(ptr);
    }


  m_variables = std::move(new_buf);
}


void
table::
set_carry_flag() noexcept
{
    for(auto  ptr: m_variables)
    {
      ptr->set_carry_flag();
    }
}


variable&
table::
append(const value&  v, gbstd::string_view  name) noexcept
{
  auto  var = new variable(*this,v,name);

  m_variables.emplace_back(var);

  return *var;
}


void
table::
print() const noexcept
{
    for(auto  var: m_variables)
    {
      var->print();

      printf("\n");
    }

}



}}



