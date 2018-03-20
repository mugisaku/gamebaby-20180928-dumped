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

      ent->block = tokens::block(s);

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


//ent->token_table.print();

  m_global_table.assign(ent->block,*this);
  m_global_table.print();

//  m_entry_list.emplace_back(std::move(ent));
}




}}




