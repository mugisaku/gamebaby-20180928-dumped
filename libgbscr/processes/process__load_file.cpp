#include"libgbscr/processes/process_private.hpp"


namespace     gbscr{
namespace processes{




void
process::
load_from_file(const char*  filepath) noexcept
{
  auto  f = fopen(filepath,"rb");

    if(f)
    {
      load_from_file(f);

      fclose(f);
    }
}


void
process::
load_from_file(FILE*  f) noexcept
{
  gbstd::string  s;

    for(;;)
    {
      auto  c = fgetc(f);

        if(feof(f) || ferror(f))
        {
          break;
        }


      s.append(c);
    }


  load_from_string(s);
}


void
process::
load_from_string(gbstd::string_view  sv) noexcept
{
  clear();


  stream  s(sv.data());

  tokens::block  blk(s);

  m_global_table.load(blk,*this);
}




}}




