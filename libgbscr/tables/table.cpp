#include"libgbscr/table.hpp"
#include"libgbscr/process.hpp"


namespace gbscr{
namespace tables{




reference
table::
operator[](gbstd::string_view  name) noexcept
{
    for(auto&  ptr: m_variables)
    {
        if(ptr->get_name() == name)
        {
          return reference(*ptr);
        }
    }


  auto  var = variable::create_instance(operand(),name);

  var->set_table(*this);

  m_variables.emplace_back(var);

  return reference(*var);
}




table&
table::
load(const block&  blk, processes::process&  proc)
{
  clear();

  variable*  var_ptr;

  cursor  cur(blk);

    if(read_variable(cur,var_ptr,proc))
    {
      var_ptr->set_table(*this);

      m_variables.emplace_back(var_ptr);

        while(cur)
        {
            if(cur->is_punctuations(","))
            {
              ++cur;
            }


            if(!read_variable(cur,var_ptr,proc))
            {
              break;
            }


          var_ptr->set_table(*this);

          m_variables.emplace_back(var_ptr);
        }
    }


  return *this;
}


reference
table::
find(gbstd::string_view  name) const noexcept
{
    for(auto&  ptr: m_variables)
    {
        if(ptr->get_name() == name)
        {
          return reference(*ptr);
        }
    }


  return reference(nullptr);
}


void
table::
clear() noexcept
{
  m_variables.clear();
}


reference
table::
append(const value&  v, gbstd::string_view  name) noexcept
{
  auto  var = variable::create_instance(v,name);

  var->set_table(*this);

  m_variables.emplace_back(var);

  return reference(*var);
}


void
table::
print() const noexcept
{
    for(auto&  var: m_variables)
    {
      var->print();

      printf("\n");
    }

}


table
table::
clone(const table&  src) noexcept
{
  table  new_table;

    for(auto&  var: src.m_variables)
    {
      auto  new_var = variable::copy_instance(*var);

      new_var->set_table(new_table);

      new_table.m_variables.emplace_back(new_var);
    }


  return std::move(new_table);
}




}}




