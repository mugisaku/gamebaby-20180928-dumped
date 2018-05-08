#include"libgbstd/canvas.hpp"
#include"sdl.hpp"


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


namespace{


canvas*
cv;


widgets::menu*
mnu;


widget*
prv;


images::image
final_image;


widgets::root
root;


constexpr int  cv_w = 8;
constexpr int  cv_h = 8;


images::image
cv_image(cv_w,cv_h);

uint8_t
preview_data[cv_h][cv_w];



void
fputc_u16be(uint16_t  c, FILE*  f)
{
  fputc(c>>8,f);
  fputc(c   ,f);
}


void
fputc_u32be(uint32_t  c, FILE*  f)
{
  fputc(c>>24,f);
  fputc(c>>16,f);
  fputc(c>> 8,f);
  fputc(c    ,f);
}


template<typename  T, int  W, int  H>
void
process(T  (&bitmap)[H][W]) noexcept
{
    for(int  x = 0;  x < W;  ++x)
    {
        for(int  y = 0;  y < H-1;  ++y)
        {
            if(!bitmap[y][x] && (bitmap[y+1][x] == 1))
            {
              bitmap[y][x] = 2;
            }
        }
    }


    for(int  y = 0;  y < H;  ++y)
    {
        for(int  x = 0;  x < W-1;  ++x)
        {
            if(!bitmap[y][x] && (bitmap[y][x+1] == 1))
            {
              bitmap[y][x] = 2;
            }
        }
    }


    for(int  x = 0;  x < W;  ++x)
    {
        for(int  y = H-1;  y > 0;  --y)
        {
            if(!bitmap[y][x] && (bitmap[y-1][x] == 1))
            {
              bitmap[y][x] = 3;
            }
        }
    }


    for(int  y = 0;  y < H;  ++y)
    {
        for(int  x = W-1;  x > 0;  --x)
        {
            if(!bitmap[y][x] && (bitmap[y][x-1] == 1))
            {
              bitmap[y][x] = 3;
            }
        }
    }
}


template<typename  T, int  W, int  H>
void
print(int  unicode, const T  (&bitmap)[H][W], FILE*  f) noexcept
{
  fprintf(f,"{0x%04X,{",unicode);

    for(int  y = 0;  y < H;  ++y)
    {
      int  v = 0;

        for(int  x = 0;  x < W;  ++x)
        {
          v <<= 2;
          v  |= bitmap[y][x];
        }


      fprintf(f,"0x%04X,",v);
    }


  fprintf(f,"}},\n");
}




namespace types{


class
sample: public widget
{
  static constexpr int  x_element_number = 60;
  static constexpr int  y_element_number =  8;

public:
  sample() noexcept;

  void  render(image_cursor  cur) noexcept override;

};


class
preview: public widget
{
  static constexpr int  m_pixel_size = 6;

public:
  preview() noexcept;

  void  render(image_cursor  cur) noexcept override;

};


class
selector: public widget
{
  static constexpr int  x_element_number = 60;
  static constexpr int  y_element_number =  8;

public:
  selector() noexcept;

  void  render(image_cursor  cur) noexcept override;

};


struct
character
{
  static constexpr int  size = 8;

  char16_t  unicode;

  uint8_t  data[size];

};


struct
combined
{
  char16_t  unicode;

  char16_t  upper;
  char16_t  lower;

};
}


using types::character;
using types::combined;


types::character
characters[] =
{
#include"characters.txt"
};


const types::combined
combineds[] =
{
#include"combineds.txt"
};


namespace character_table{

constexpr int       width = 16;
constexpr int      height = 8;
constexpr int  pixel_size = 2;

int    offset;
point  cursor;

character*  current = characters;

character*
get_pointer(point  index) noexcept
{
  auto  i = ((width*(offset+index.y))+index.x);

  auto  ptr = characters+i;

  return (ptr < std::end(characters))? ptr:nullptr;
}


void
send() noexcept
{
    for(int  y = 0;  y < character::size;  ++y)
    {
      auto  code = current->data[y];

        for(int  x = 0;  x < character::size;  ++x)
        {
          cv_image.set_pixel((code&0x80)? colors::white:color(),x,y);

          code <<= 1;
        }
    }
}


void
receive() noexcept
{
    for(int  y = 0;  y < character::size;  ++y)
    {
      auto&  dst = current->data[y];

      dst = 0;

        for(int  x = 0;  x < character::size;  ++x)
        {
          dst <<= 1;

          dst |= cv_image.get_pixel(x,y).color? 1:0;
        }
    }
}


}


types::sample*
sample;






types::character&
get_character_by_unicode(char16_t  unicode) noexcept
{
  static types::character*  table[0x10000];

  static types::character  null;

  auto  ptr = table[unicode];

    if(!ptr)
    {
        for(auto&  c: characters)
        {
            if(c.unicode == unicode)
            {
              table[unicode] = &c;

              return c;
            }
        }


      return null;
    }


  return *ptr;
}


void
print_characters(FILE*  f) noexcept
{
  fprintf(f,"//characters\n");

    for(auto&  c: characters)
    {
      fprintf(f,"{0x%04X,{",c.unicode);

        for(int  i = 0;  i < types::character::size;  ++i)
        {
          fprintf(f,"0x%02X,",c.data[i]);
        }


      fprintf(f,"}},\n");
    }
}


void
print_combineds(FILE*  f) noexcept
{
    for(auto&  c: combineds)
    {
      auto&  u = get_character_by_unicode(c.upper);
      auto&  l = get_character_by_unicode(c.lower);

      uint8_t  bitmap[16][8] = {0};

        for(int  y = 0;  y < 8;  ++y)
        {
          auto  v = u.data[y];

            for(int  x = 0;  x < 8;  ++x)
            {
                if(v&(0x80>>x))
                {
                  bitmap[y][x] = 1;
                }
            }
        }


        for(int  y = 0;  y < 8;  ++y)
        {
          auto  v = l.data[y];

            for(int  x = 0;  x < 8;  ++x)
            {
                if(v&(0x80>>x))
                {
                  bitmap[8+y][x] = 1;
                }
            }
        }


      process(bitmap);
      print(c.unicode,bitmap,f);
    }
}


void
render_character(image_cursor  cur, const types::character&  c, int  pixel_size, int  x, int  y) noexcept
{
  auto  src = c.data;

    for(int  yy = 0;  yy < types::character::size;  ++yy)
    {
      auto  code = *src++;

        for(int  xx = 0;  xx < types::character::size;  ++xx)
        {
          auto&  white = colors::white;
          auto&  black = colors::black;

          cur.fill_rectangle((code&0x80)? white:black,pixel_size*(x+xx),pixel_size*(y+yy),pixel_size,pixel_size);

          code <<= 1;
        }
    }
}




const char16_t*
string = 
u"0123456789!?abcdefghijklmnopqrstuvwxyz"
u"ABCDEFGHIJKLMNOPQRSTUVWXYZ+-*/=!\"#$%&\'()~|{}[]:;_"
u"ぁあぃいぅうぇえぉおかがきぎくぐけげこごさざしじすずせぜそぞただちぢっつづてでとどなにぬねの"
u"はばぱひびぴふぶぷへべぺほぼぽまみむめもゃやゅゆょよらりるれろわをんー"
u"ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノ"
u"ハバパヒビピフブプヘベペホボポマミムメモャヤュユョヨラリルレロワヲン";


types::sample::
sample() noexcept
{
  m_width  = (  8*x_element_number);
  m_height = (16*y_element_number);
}




void
types::sample::
render(image_cursor  cur) noexcept
{
  int  x = 0;
  int  y = 0;

    for(auto&  c: combineds)
    {
        if((x+8) >= get_width())
        {
          x  =  0;
          y += 16;
        }


        if((y+16) >= get_height())
        {
          break;
        }


      render_character(cur,get_character_by_unicode(c.upper),1,x,y  );
      render_character(cur,get_character_by_unicode(c.lower),1,x,y+8);

      x += 8;
    }
}






types::preview::
preview() noexcept
{
  m_width  = (cv_w*m_pixel_size);
  m_height = (cv_h*m_pixel_size);
}


void
types::preview::
render(image_cursor  cur) noexcept
{
    for(int  y = 0;  y < cv_h;  ++y){
    for(int  x = 0;  x < cv_w;  ++x){
      auto  v = preview_data[y][x];

      auto  color = (v == 0)? colors::black
                   :(v == 1)? colors::white
                   :(v == 2)? colors::red
                   :          colors::blue;

      cur.fill_rectangle(color,m_pixel_size*x,m_pixel_size*y,m_pixel_size,m_pixel_size);
    }}
}




void
update_preview_data() noexcept
{
    for(int  y = 0;  y < cv_h;  ++y)
    {
      auto  v = character_table::current->data[y];

      uint8_t*  dst = preview_data[y];

        for(int  x = 0;  x < cv_w;  ++x)
        {
          *dst++ = (v&0x80)? 1:0;

          v <<= 1;
        }
    }


  process(preview_data);
}




void
save() noexcept
{
#ifdef EMSCRIPTEN
#else
  auto  f = fopen("characters.txt","wb");

    if(f)
    {
      print_characters(f);

      fclose(f)                             ;
             f = fopen("font_data.txt","wb");

      print_combineds(f);

      fclose(f)                             ;
    }
#endif
}




void
main_loop() noexcept
{
  auto&  condev = sdl::update_control_device();

  root.react(condev);

    if(root.redraw_only_needed_widgets(final_image) || condev.needed_to_redraw)
    {
      sdl::update_screen(final_image);
    }
}


}


int
main(int  argc, char**  argv)
{
  cv = new canvases::canvas(cv_image,0,0,[](canvas&  cv){
    character_table::receive();

    update_preview_data();

       mnu->need_to_redraw();
       prv->need_to_redraw();
    sample->need_to_redraw();
  });

  cv->set_grid();
  cv->set_pixel_size(12);
  cv->set_drawing_color(colors::white);

  widgets::menu_item_parameter  mip = {character_table::pixel_size*character::size,
                                       character_table::pixel_size*character::size,
    [](widgets::menu&  menu, point  index, mouse_button  left, mouse_button  right)
    {
        if(left)
        {
          auto  ptr = character_table::get_pointer(index);

            if(ptr)
            {
              character_table::current = ptr;

              character_table::send();

              cv->cancel_drawing();

              cv->get_drawing_recorder().clear();

              cv->need_to_redraw();

              update_preview_data();

              prv->need_to_redraw();

              menu.need_to_redraw();
            }
        }
    },

    [](widgets::menu&  menu, point  index, image_cursor  cur)
    {
      auto  ptr = character_table::get_pointer(index);

        if(ptr)
        {
          render_character(cur,*ptr,character_table::pixel_size,0,0);

            if(ptr == character_table::current)
            {
              cur.draw_rectangle(colors::white,0,0,character_table::pixel_size*character::size,
                                                   character_table::pixel_size*character::size);
            }
        }

      else
        {
          cur.fill_rectangle(colors::black,0,0,character_table::pixel_size*character::size,
                                               character_table::pixel_size*character::size);
        }
    }
  };


  mnu = new widgets::menu(mip,character_table::width,character_table::height);

  auto  mnu_tool_col = new widgets::table_column({
    new widgets::button(new widgets::label(u"↑"),[](widgets::button&  btn){
        if(btn.get_count())
        {
          btn.reset_count();

            if(character_table::offset)
            {
              --character_table::offset;

              mnu->need_to_redraw();
            }
        }
    }),
    new widgets::button(new widgets::label(u"↓"),[](widgets::button&  btn){
        if(btn.get_count())
        {
          btn.reset_count();

          ++character_table::offset;

          mnu->need_to_redraw();
        }
    }),
    new widgets::button(new widgets::label(u"Save"),[](widgets::button&  btn){
        if(btn.get_count())
        {
          btn.reset_count();

          save();
        }
    }),
  });


  sample = new types::sample;
  prv    = new types::preview;

  auto  chrtbl = new widgets::table_row({mnu,mnu_tool_col});

  auto  urow = new widgets::table_row({new widgets::frame(cv,"canvas"),
                                       cv->create_operation_widget(),
                                       new widgets::frame(chrtbl,"character_table")
                                     });

  auto  drow = new widgets::table_row({new widgets::frame(prv,"preview"),
                                       new widgets::frame(sample,"sample")
                                     });

  root.set_node_target(new widgets::table_column({urow,drow}));


  auto&  root_node = root.get_node();

  sdl::init(root_node.get_width(),root_node.get_height());

  final_image = sdl::make_screen_image();

  root.redraw(final_image);

#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop,0,false);
#else
    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }


  sdl::quit();
#endif


  return 0;
}




