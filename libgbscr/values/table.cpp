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


reference
table::
operator[](gbstd::string_view  name) noexcept
{
    for(auto  ptr: m_variables)
    {
        if(ptr->get_name() == name)
        {
          return reference(*ptr);
        }
    }


  auto  var = variable::create_instance();

  var->set_table(this);
  var->set_name(name);

  m_variables.emplace_back(var);

  return reference(*var);
}




table&
table::
assign(const block&  blk, table&  tbl)
{
  clear();

  variable*  var_ptr;

  cursor  cur(blk);

    if(read_variable(cur,var_ptr,*this))
    {
      m_variables.emplace_back(var_ptr);

        while(cur)
        {
            if(cur->is_punctuations(","))
            {
              ++cur;
            }


            if(!read_variable(cur,var_ptr,*this))
            {
              break;
            }


          m_variables.emplace_back(var_ptr);
        }
    }


  return *this;
}




reference
table::
find(gbstd::string_view  name) const noexcept
{
    for(auto  ptr: m_variables)
    {
        if(ptr->get_name() == name)
        {
          return reference(ptr);
        }
    }


  return reference(nullptr);
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
    for(auto  ptr: m_variables)
    {
        if(ptr->test_carry_flag())
        {
          ptr->unset_carry_flag();

          dst.m_variables.emplace_back(ptr);
        }

      else
        {
          delete ptr;
        }
    }


  m_variables.clear();
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


reference
table::
append(const value&  v, gbstd::string_view  name) noexcept
{
  auto  var = variable::create_instance();

  var->set_table(this);
  var->set_value(v);
  var->set_name(name);

  m_variables.emplace_back(var);

  return reference(*var);
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




