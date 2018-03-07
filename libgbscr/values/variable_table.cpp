#include"libgbscr/object.hpp"


namespace gbscr{
namespace objects{




variable_table::variable::
variable() noexcept: reference(this->value)
{
}


variable_table::variable::
~variable()
{
    if(reference::get_count() > 1)
    {
      reference::unset_pointer();
    }
}


void
variable_table::variable::
print() const noexcept
{
  printf("%d \n",get_count());
}




reference
variable_table::
operator[](gbstd::string_view  name) const noexcept
{
    for(auto  var: m_variables)
    {
        if(var->name == name)
        {
          return reference(*var);
        }
    }


  return reference();
}


reference
variable_table::
operator[](int  i) const noexcept
{
  return ((i >= 0) && (i < m_variables.size()))? *m_variables[i]:reference();
}




int
variable_table::
find(gbstd::string_view  name) const noexcept
{
  int  i = 0;

    for(auto  var: m_variables)
    {
        if(var->name == name)
        {
          return i;
        }


      ++i;
    }


  return -1;
}


void
variable_table::
clear() noexcept
{
    for(auto  ptr: m_variables)
    {
      delete ptr;
    }


  m_variables.clear();
}


reference
variable_table::
append(const value&  value, gbstd::string_view  name) noexcept
{
  auto  ptr = new variable();

  m_variables.emplace_back(ptr);

  ptr->value = value;
  ptr->name  =  name;

  return *ptr;
}


void
variable_table::
print() const noexcept
{
    for(auto  var: m_variables)
    {
      var->value.print();

      printf("%s  ",var->name.data());
    }
}




}}




