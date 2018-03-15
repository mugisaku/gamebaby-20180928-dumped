#include"libgbscr/processes/process_private.hpp"


namespace     gbscr{
namespace processes{




std::unique_ptr<process::entry>
process::
create_entry(const char*  filepath) noexcept
{
  auto  f = fopen(filepath,"rb");

    if(f)
    {
      auto  ent = new entry;

      ent->file_path.assign(filepath);

        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f) || ferror(f))
            {
              break;
            }


          ent->file_content += c;
        }


      fclose(f);


      stream  s(ent->file_content.data());

      ent->tokens = token_string(s,0,0);

      return std::unique_ptr<entry>(ent);
    }


  return nullptr;
}


void
process::
load_file(const char*  filepath) noexcept
{
  auto  ent = create_entry(filepath);

    if(!ent)
    {
      return;
    }


  token_cursor  cur(ent->tokens);

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

                  m_global_table.append(value(*rt),var_name);

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


  m_entry_list.emplace_back(std::move(ent));
}




}}




