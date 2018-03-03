#include"libgbscr/process.hpp"
#include"libgbscr/stream.hpp"
#include<memory>


namespace     gbscr{
namespace processes{




struct
process::
private_data
{
  size_t  reference_count=1;

  std::vector<std::unique_ptr<routine>>  routine_list;

  std::list<object>  object_list;

};


process::
process() noexcept:
m_data(new private_data)
{
}




process&
process::
operator=(const process&   rhs) noexcept
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


process&
process::
operator=(process&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      std::swap(m_data,rhs.m_data);
    }


  return *this;
}




void
process::
unrefer() noexcept
{
    if(m_data)
    {
        if(!--m_data->reference_count)
        {
          delete m_data;
        }


      m_data = nullptr;
    }
}




void
process::
load_file(const char*  filepath) noexcept
{
  auto  ss = make_string_from_file(filepath);

  stream  s(ss.data());

  token_string  toks(s,0,0);

  token_cursor  cur(toks);

    while(cur)
    {
      using sv = gbstd::string_view;

        if(cur[0].is_identifier() &&
           cur[1].is_identifier())
        {
          sv  type_name(cur[0].get_string().view());
          sv   var_name(cur[1].get_string().view());

          cur += 2;

            if(type_name == sv("routine"))
            {
                if(cur[0].is_token_string('(',')') &&
                   cur[1].is_token_string('{','}'))
                {
                  auto&  parals = cur[0].get_token_string();
                  auto&   block = cur[1].get_token_string();

                  
                  auto  rt = new routine(parals,block);

                  m_data->object_list.emplace_back(value(*rt),var_name);

                  cur += 2;
                }
            }
        }

      else
        if(cur[0].is_semicolon())
        {
          cur += 1;
        }

      else
        {
          printf("不明な構文\n");

          break;
        }
    }
}




const std::vector<std::unique_ptr<routine>>&
process::
get_routine_list() const noexcept
{
  return m_data->routine_list;
}




const routine*
process::
find_routine(gbstd::string_view  name) const noexcept
{
    for(auto&  obj: m_data->object_list)
    {
        if((obj.get_name() == name) && obj.is_routine())
        {
          return &obj.get_routine();
        }
    }


  return nullptr;
}


void
process::
print() const noexcept
{
  printf("reference_count: %ld\n",m_data->reference_count);

  printf("objects: %d{\n",m_data->object_list.size());

    for(auto&  obj: m_data->object_list)
    {
      obj.print();

      printf("\n");
    }


  printf("}\n\n");
}




}}




