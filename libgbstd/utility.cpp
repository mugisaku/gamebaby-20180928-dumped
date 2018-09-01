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

  else
    {
      printf("make_string_from_file error: %sは開けない\n",filepath);
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




#ifdef EMSCRIPTEN
#include<emscripten.h>
EM_JS(void,js_set_caption,(const char*  s),{
  var  cap = document.getElementById("caption");
  cap.innerHTML = UTF8ToString(s);
});
EM_JS(void,js_set_description,(const char*  s),{
  var  desc = document.getElementById("description");
  desc.innerHTML = UTF8ToString(s);
});
EM_JS(int,js_get_from_front_dropped_file,(int  i),{
  return g_dropped_file_list[0][i];
});
EM_JS(void,js_download,(const uint8_t*  ptr, size_t  size, const char*  filename),{
    if(!window.FileReader)
    {
      console.log("error");
    }


  var  base = new Blob([HEAP8],{type:"application/octet-stream"});

  var  target = base.slice(ptr,ptr+size);

  var  a = document.createElement("a");

  a.download = UTF8ToString(filename);

  var  fr = new FileReader();

  fr.anchor = a;

  fr.onload = function(e){
    var  a = this.anchor;

    a.href = this.result;

    a.click();
  };


  fr.readAsDataURL(target);
});


void
set_caption(const char*  s) noexcept
{
  js_set_caption(s);
}
void
set_description(const char*  s) noexcept
{
  js_set_description(s);
}
void
download(const uint8_t*  ptr, size_t  size, const char*  filename) noexcept
{
  js_download(ptr,size,filename);
}
int
get_number_of_dropped_files() noexcept
{
  return EM_ASM_INT(return g_dropped_file_list.length);
}
std::vector<uint8_t>
pop_front_dropped_file() noexcept
{
  int  n = EM_ASM_INT(return g_dropped_file_list[0].length);

  std::vector<uint8_t>  buf(n);

    for(int  i = 0;  i < n;  ++i)
    {
      buf[i] = js_get_from_front_dropped_file(i);
    }


  EM_ASM(
    g_dropped_file_list.shift();
  );

  return std::move(buf);
}
#endif


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




