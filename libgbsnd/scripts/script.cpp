#include"libgbsnd/script.hpp"
#include"libtok/stream_reader.hpp"
#include"libgbsnd/stmt.hpp"
#include<memory>


namespace gbsnd{
namespace scripts{




struct
script::
data
{
  size_t  reference_count=1;

  gbstd::string  source_path;

  std::vector<std::unique_ptr<square_wave>>  square_wave_list;
  std::vector<std::unique_ptr<noise>>              noise_list;
  std::vector<std::unique_ptr<routine>>          routine_list;

  std::list<object>  object_list;

};


script::
script(const script_token_string&  toks, gbstd::string_view  source_path) noexcept:
m_data(new data)
{
  m_data->source_path = source_path;

  script_token_cursor  cur(toks.begin(),toks.end());

//  m_data->object_list.emplace_back(value(system{}),"system");

    while(cur)
    {
      using sv = gbstd::string_view;

        if(cur[0].is_identifier() &&
           cur[1].is_identifier())
        {
          sv  type_name(cur[0].get_identifier().view());
          sv   var_name(cur[1].get_identifier().view());

          cur += 2;

            if(type_name == sv("square_wave"))
            {
              auto  sq = new square_wave;

              sq->set_name(var_name);

              m_data->square_wave_list.emplace_back(sq);

              m_data->object_list.emplace_back(value(*sq),var_name);
            }

          else
            if(type_name == sv("noise"))
            {
              auto  noi = new noise;

              noi->set_name(var_name);

              m_data->noise_list.emplace_back(noi);

              m_data->object_list.emplace_back(value(*noi),var_name);
            }

          else
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




script&
script::
operator=(const script&   rhs) noexcept
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


script&
script::
operator=(script&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      std::swap(m_data,rhs.m_data);
    }


  return *this;
}




void
script::
unrefer() noexcept
{
    if(m_data)
    {
        if(!--m_data->reference_count)
        {
          delete m_data          ;
                 m_data = nullptr;
        }
    }
}




const gbstd::string&
script::
get_source_path() const noexcept
{
  return m_data->source_path;
}


std::list<object>&
script::
get_object_list() const noexcept
{
  return m_data->object_list;
}


const std::vector<std::unique_ptr<routine>>&
script::
get_routine_list() const noexcept
{
  return m_data->routine_list;
}


const std::vector<std::unique_ptr<square_wave>>&
script::
get_square_wave_list() const noexcept
{
  return m_data->square_wave_list;
}


const std::vector<std::unique_ptr<noise>>&
script::
get_noise_list() const noexcept
{
  return m_data->noise_list;
}




const routine*
script::
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
script::
print() const noexcept
{
  printf("source path: %s\n",m_data->source_path.data());

  printf("reference_count: %ld\n",m_data->reference_count);

  printf("objects: %d{\n",m_data->object_list.size());

    for(auto&  obj: m_data->object_list)
    {
      obj.print();

      printf("\n");
    }


  printf("}\n\n");
}




script
script::
build_from_string(gbstd::string_view  sv, gbstd::string_view  source_path) noexcept
{
  tok::stream_reader  r(sv);

  script_token_string  toks(r,0,0);


  script  scr(toks,source_path);

  return std::move(scr);
}


script
script::
build_from_file(gbstd::string_view  path) noexcept
{
  gbstd::string_copy  sc(path);

  auto  f = fopen(sc.data(),"rb");

    if(f)
    {
      auto  scr = build_from_file(f,path);

      fclose(f);

      return std::move(scr);
    }

  else
    {
      printf("%sを開けない\n",sc.data());
    }


  return script();
}


script
script::
build_from_file(FILE*  f, gbstd::string_view  source_path) noexcept
{
  gbstd::string  s;

    for(;;)
    {
      auto  c = fgetc(f);

        if(feof(f) || ferror(f))
        {
          break;
        }


      s += c;
    }


  return build_from_string(s,source_path);
}




}}




