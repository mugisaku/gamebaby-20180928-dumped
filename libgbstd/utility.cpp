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
EM_JS(void,js_new_array,(int  n),{
  g_transferred_data = new Uint8Array(n);
});
EM_JS(void,js_put,(int  c, int  i),{
  g_transferred_data[i] = c;
});
EM_JS(void,js_download_image,(),{
  var  i = 3;
  var  w  = g_transferred_data[i++]<<8;
       w |= g_transferred_data[i++]   ;
  var  h  = g_transferred_data[i++]<<8;
       h |= g_transferred_data[i++]   ;
  var  n = w*h;

  var  cv = document.createElement("canvas");

  cv.width  = w;
  cv.height = h;

  var  ctx = cv.getContext("2d");

  var  imgdat = ctx.createImageData(w,h);

  var  dst_i = 0;

    while(n--)
    {
      imgdat.data[dst_i++] = g_transferred_data[i++];
      imgdat.data[dst_i++] = g_transferred_data[i++];
      imgdat.data[dst_i++] = g_transferred_data[i++];
      imgdat.data[dst_i++] = g_transferred_data[i++];
    }


  ctx.putImageData(imgdat,0,0);

  var  a = document.createElement("a");

  a.download = "new.png";

  a.href = cv.toDataURL();

  a.click();
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
download_image() noexcept
{
  js_download_image();
}
void
transfer_to_javascript(const uint8_t*  data, size_t  data_size) noexcept
{
  js_new_array(data_size);

    for(int  i = 0;  i < data_size;  ++i)
    {
      js_put(*data++,i);
    }
}
#endif


bool
is_image_data(const void*  ptr) noexcept
{
  static const uint8_t  sig[] = {'i','m','g'};

  return std::memcmp(ptr,sig,sizeof(sig)) == 0;
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




