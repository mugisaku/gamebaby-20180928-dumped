#include"libgbscr/value.hpp"
#include"libgbscr/process.hpp"


namespace gbscr{
namespace values{




struct
table::
private_data
{
  size_t  count;

  std::vector<variable*>  variables;

};




void
table::
unrefer() noexcept
{
    if(m_data)
    {
        if(!--m_data->count)
        {
          delete m_data;
        }


      m_data = nullptr;
    }
}




table&
table::
operator=(const table&   rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      m_data = rhs.m_data;

        if(m_data)
        {
          ++m_data->count;
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
      unrefer();

      std::swap(m_data,rhs.m_data);
    }


  return *this;
}


reference
table::
operator[](gbstd::string_view  name) const noexcept
{
    for(auto  ptr: m_data->variables)
    {
        if(ptr->get_name() == name)
        {
          return reference(*ptr);
        }
    }


  auto  var = variable::create_instance();

  var->set_table(this);
  var->set_name(name);

  m_data->variables.emplace_back(var);

  return reference(*var);
}




table&
table::
assign(const block&  blk, processes::process&  proc)
{
  unrefer();

  m_data = new private_data;

  m_data->count = 1;

  variable*  var_ptr;

  cursor  cur(blk);

    if(read_variable(cur,var_ptr,proc))
    {
      m_data->variables.emplace_back(var_ptr);

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


          m_data->variables.emplace_back(var_ptr);
        }
    }


  return *this;
}




reference
table::
find(gbstd::string_view  name) const noexcept
{
    for(auto  ptr: m_data->variables)
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
clear() const noexcept
{
    for(auto  ptr: m_data->variables)
    {
      delete ptr;
    }


  m_data->variables.clear();
}


reference
table::
append(const value&  v, gbstd::string_view  name) const noexcept
{
  auto  var = variable::create_instance();

  var->set_table(this);
  var->set_value(v);
  var->set_name(name);

  m_data->variables.emplace_back(var);

  return reference(*var);
}


void
table::
print() const noexcept
{
    for(auto  var: m_data->variables)
    {
      var->print();

      printf("\n");
    }

}



}}




