#include"utility.hpp"
#include"string.hpp"
#include"text.hpp"
#include<cstdio>


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif




namespace gbstd{


int  reporting_counter;


gbstd::string
make_string_from_file(const char*  filepath) noexcept
{
  gbstd::string  s;

  auto  f = fopen(filepath,"rb");

    if(f)
    {
        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f))
            {
              break;
            }


          s.append(c);
        }


      fclose(f);
    }


  return std::move(s);
}


void
printf_with_indent(int  indent, const char*  fmt, ...) noexcept
{
  char  buf[256];

  va_list  ap;
  va_start(ap,fmt);

  vsnprintf(buf,sizeof(buf),fmt,ap);

  va_end(ap);

  const char*  p = buf;

    while(*p)
    {
      fputc(*p,stdout);

        if(*p++ == '\n')
        {
            for(int  n = 0;  n < indent;  ++n)
            {
              fputc(' ',stdout);
            }
        }
    }
}




void
set_caption(const char*  s) noexcept
{
#ifdef EMSCRIPTEN
  string_form  sf;

  emscripten_run_script(sf(
    "  var  cap = document.getElementById(\"caption\");"
    "  cap.innerHTML = \"%s\";"
  ,s));
#endif
}


void
set_description(const char*  s) noexcept
{
#ifdef EMSCRIPTEN
  string_form  sf;

  emscripten_run_script(sf(
    "  var  desc = document.getElementById(\"description\");"
    "  desc.innerHTML = \"%s\";"
  ,s));
#endif
}


void
generate_saved_image_link(int  w, int  h) noexcept
{
#ifdef EMSCRIPTEN
  string_form  sf;

  emscripten_run_script(sf("download_image(%d,%d);",w,h));
#endif
}




bool
is_png(const void*  ptr) noexcept
{
  static const uint8_t  sig[] = {0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A};

  return std::memcmp(ptr,sig,sizeof(sig)) == 0;
}


bool
is_webp(const void*  ptr) noexcept
{
  auto  p = static_cast<const char*>(ptr);

  return (std::memcmp(p  ,"RIFF",4) == 0) &&
         (std::memcmp(p+8,"WEBP",4) == 0);
}




std::random_device
random_device;


std::default_random_engine
random_number_engine;


}




