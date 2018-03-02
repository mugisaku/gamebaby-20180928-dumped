#include<SDL.h>
#include<cstdint>
#include<cstdlib>
#include<cstdio>
#include<cmath>
#include<list>
#include<random>


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


namespace{


constexpr int  screen_size = 512;


SDL_Window*    window;
SDL_Surface*  surface;


int  pitch;


SDL_PixelFormat  format;


constexpr int  pixel_size = 1;
constexpr int  table_size = screen_size/pixel_size;

uint32_t
bitmap[table_size][table_size];

bool
need_to_redraw = true;


std::random_device  randev;


void
quit() noexcept
{
  SDL_DestroyWindow(window);

  SDL_Quit();

  exit(0);
}


void
transfer1(uint8_t*  p_base) noexcept
{
    for(int  y = 0;  y < screen_size;  ++y)
    {
      auto  p = p_base         ;
                p_base += pitch;

        for(int  x = 0;  x < screen_size;  ++x)
        {
          *p++ = bitmap[y/pixel_size][x/pixel_size];
        }
    }
}


void
transfer2(uint8_t*  p_base) noexcept
{
    for(int  y = 0;  y < screen_size;  ++y)
    {
      auto  p = p_base         ;
                p_base += pitch;

        for(int  x = 0;  x < screen_size;  ++x)
        {
          *reinterpret_cast<uint16_t*>(p) = bitmap[y/pixel_size][x/pixel_size];

          p += sizeof(uint16_t);
        }
    }
}


void
transfer3(uint8_t*  p_base) noexcept
{
    for(int  y = 0;  y < screen_size;  ++y)
    {
      auto  p = p_base         ;
                p_base += pitch;

        for(int  x = 0;  x < screen_size;  ++x)
        {
          uint8_t  r;
          uint8_t  g;
          uint8_t  b;

          SDL_GetRGB(bitmap[y/pixel_size][x/pixel_size],&format,&r,&g,&b);

          *p++ = r;
          *p++ = g;
          *p++ = b;
        }
    }
}


void
transfer4(uint8_t*  p_base) noexcept
{
    for(int  y = 0;  y < screen_size;  ++y)
    {
      auto  p = p_base         ;
                p_base += pitch;

        for(int  x = 0;  x < screen_size;  ++x)
        {
          *reinterpret_cast<uint32_t*>(p) = bitmap[y/pixel_size][x/pixel_size];

          p += sizeof(uint32_t);
        }
    }
}




void
update() noexcept
{
  SDL_LockSurface(surface);

  SDL_memset(surface->pixels,0,surface->pitch*surface->h);

  auto  p_base = static_cast<uint8_t*>(surface->pixels);

    switch(surface->format->BytesPerPixel)
    {
  case(1): transfer1(p_base);break;
  case(2): transfer2(p_base);break;
  case(3): transfer3(p_base);break;
  case(4): transfer4(p_base);break;
  default:
      printf("%d\n",surface->format->BytesPerPixel);
    }


  SDL_UnlockSurface(surface);

  SDL_UpdateWindowSurface(window);
}




constexpr int  height_max = 1<<13;//8192
constexpr int   sea_level = 1<<12;//4096


namespace types{
class
direction
{
public:
  enum class source{
            up,
          left,
         right,
          down,
       up_left,
      up_right,
     down_left,
    down_right,
           end,
  };

  static constexpr int     up_left = static_cast<int>(source::up_left);
  static constexpr int    up_right = static_cast<int>(source::up_right);
  static constexpr int   down_left = static_cast<int>(source::down_left);
  static constexpr int  down_right = static_cast<int>(source::down_right);
  static constexpr int          up = static_cast<int>(source::up);
  static constexpr int        left = static_cast<int>(source::left);
  static constexpr int       right = static_cast<int>(source::right);
  static constexpr int        down = static_cast<int>(source::down);
  static constexpr int         end = static_cast<int>(source::end);

private:
  int  m_kind;

public:
           constexpr direction(      ) noexcept: m_kind(0){}
  explicit constexpr direction(int  i) noexcept: m_kind(i){}

  constexpr bool  is_up() const noexcept{return m_kind == up;}
  constexpr bool  is_left() const noexcept{return m_kind == left;}
  constexpr bool  is_righ() const noexcept{return m_kind == right;}
  constexpr bool  is_down() const noexcept{return m_kind == down;}
  constexpr bool  is_up_left() const noexcept{return m_kind == up_left;}
  constexpr bool  is_up_right() const noexcept{return m_kind == up_right;}
  constexpr bool  is_down_left() const noexcept{return m_kind == down_left;}
  constexpr bool  is_down_right() const noexcept{return m_kind == down_right;}

  constexpr const char*  get_name() const noexcept
  {
    return( (m_kind ==         up)? "up"
           :(m_kind ==       left)? "left"
           :(m_kind ==      right)? "right"
           :(m_kind ==       down)? "down"
           :(m_kind ==    up_left)? "up left"
           :(m_kind ==   up_right)? "up right"
           :(m_kind ==  down_left)? "down left"
           :(m_kind == down_right)? "down right"
           :"end");
  }


  constexpr bool  is_straight() const noexcept
  {
    return((m_kind ==    up) ||
           (m_kind ==  left) ||
           (m_kind == right) ||
           (m_kind ==  down));
  }

  constexpr operator bool() const noexcept{return m_kind < end;}

  constexpr bool  operator==(const direction&  rhs) const noexcept{return m_kind == rhs.m_kind;}
  constexpr bool  operator!=(const direction&  rhs) const noexcept{return m_kind != rhs.m_kind;}

  constexpr int  operator*() const noexcept{return m_kind;}

  direction&  operator++() noexcept
  {
    ++m_kind;

    return *this;
  }

  direction  operator++(int) noexcept
  {
    auto  dir = *this;

    ++m_kind;

    return dir;
  }

};


constexpr direction          up_dir(direction::up);
constexpr direction        left_dir(direction::left);
constexpr direction       right_dir(direction::right);
constexpr direction        down_dir(direction::down);
constexpr direction     up_left_dir(direction::up_left);
constexpr direction    up_right_dir(direction::up_right);
constexpr direction   down_left_dir(direction::down_left);
constexpr direction  down_right_dir(direction::down_right);


class
element
{
  int  m_x_pos;
  int  m_y_pos;

  int  m_durability;

  int32_t  m_distance=0;

  int32_t  m_height=0;
  int      m_fraction=0;

  element*  m_link[direction::end];

  bool  m_hot;

public:
  void  set_pos(int  x, int  y) noexcept
  {
    m_x_pos = x;
    m_y_pos = y;
  }

  int  get_x_pos() const noexcept{return m_x_pos;}
  int  get_y_pos() const noexcept{return m_y_pos;}

  void  set_hot() noexcept{m_hot = true;}

  bool  test_hot() const noexcept{return m_hot;}

  int  get_durability() const noexcept{return m_durability;}

  void      set_link(direction  dir, element*  e)       noexcept{       m_link[*dir] = e;}
  element*  get_link(direction  dir             ) const noexcept{return m_link[*dir]    ;}

  void     set_distance(int32_t  v)       noexcept{       m_distance = v;}
  int32_t  get_distance(          ) const noexcept{return m_distance    ;}

  void     set_height(int32_t  v)       noexcept{       m_height = v;}
  int32_t  get_height(          ) const noexcept{return m_height    ;}

  void  erode(element&  dst, int  fraction) noexcept
  {
        m_fraction -= fraction;
    dst.m_fraction += fraction;
  }

  void  collect_fraction() noexcept
  {
      if(m_fraction >= 1024)
      {
          while(m_fraction >= 1024)
          {
            ++m_height;

            m_fraction -= 1024;
          }
      }

    else
      if(m_fraction <= -1024)
      {
          while(m_fraction <= -1024)
          {
            --m_height;

            m_fraction += 1024;
          }
      }
  }

  void  add_height(int  v) noexcept{m_height = std::min(m_height+v,height_max);}

  void  print() const noexcept
  {
    printf("x:%3d y:%3d distamce:%4d\n",m_x_pos,m_y_pos,m_distance);
  }

};


}


constexpr int  edge_length = 80;

int  diagonal_length;


int
get_distance(int  x0, int  y0, int  x1, int  y1) noexcept
{
  int  x_diff = std::abs(x1-x0);
  int  y_diff = std::abs(y1-y0);

    if(x_diff < y_diff)
    {
      return (diagonal_length*(       x_diff))
            +(    edge_length*(y_diff-x_diff));
    }

  else
    {
      return (diagonal_length*(       y_diff))
            +(    edge_length*(x_diff-y_diff));
    }
}


using types::element;
using types::direction;
using types::        up_dir;
using types::      left_dir;
using types::     right_dir;
using types::      down_dir;
using types::   up_left_dir;
using types::  up_right_dir;
using types:: down_left_dir;
using types::down_right_dir;


element
table[table_size][table_size];


class
context
{
  element*  m_start=nullptr;

  int  m_center_height;

  int  m_y_pos;

  int  m_preasure;
  int  m_distance;

  int  m_result;

public:
  context() noexcept{}
  context(element&  start, int  preasure, int  distance) noexcept{reset(start,preasure,distance);}

  operator bool() const noexcept
  {
    return(m_distance > 0);
  }

  void  reset(element&  start, int  preasure, int  distance) noexcept
  {
    m_start = &start;

    start.set_hot();

    m_preasure = preasure;
    m_distance = distance;
  }

  void  prepare_to_flow() noexcept
  {
    m_center_height = m_start->get_height();

      if(m_center_height >= height_max)
      {
        return;
      }


    int  air_preasure = (m_center_height < sea_level)? (height_max-sea_level)
                                                     : (height_max-m_center_height);

    int  water_preasure = 2*((m_center_height < sea_level)? (sea_level-m_center_height)
                                                          : 0);

    int  weight = 4*m_center_height;

    m_result = m_preasure-(weight+water_preasure+air_preasure);

    m_y_pos = 0;

    m_preasure -= 200;
  }


  bool  flow_lava(int  h=1) noexcept
  {
      if((m_result <= 0) || (m_center_height >= height_max) || (m_distance <= 0))
      {
        m_distance = -1;

        return false;
      }


    int  start_x_pos = m_start->get_x_pos();
    int  start_y_pos = m_start->get_y_pos();

      while(h-- && (m_y_pos < table_size))
      {
        element*  row = table[m_y_pos];

          for(int  x = 0;  x < table_size;  ++x)
          {
            auto&  e = *row++;

            auto  diff_h = m_center_height-e.get_height();

              if(diff_h >= 0)
              {
                auto  e_distance = (get_distance(start_x_pos,start_y_pos,x,m_y_pos)+diff_h);

                  if(e_distance < m_distance)
                  {
                    int  rate = 120-(e_distance*100/m_distance);

                    e.add_height(((200<<10)/100*rate)>>10);
                  }
              }
          }


        ++m_y_pos;
      }


      if(m_y_pos >= table_size)
      {
        m_distance -= 1;
      }


    return(m_y_pos < table_size);
  }

};


void
print() noexcept
{
  auto  f = fopen("__land.txt","wb");

    for(int  y = 0;  y < table_size;  ++y)
    {
        for(int  x = 0;  x < table_size;  ++x)
        {
          auto&  e = table[y][x];

          fprintf(f,"%8d,",e.get_height());
        }


      fprintf(f,"\n");
    }


  fclose(f);
}


bool
erode(int  h=1) noexcept
{
  static int  y;

    while(h-- && (y < table_size))
    {
      element*  row = table[y++];

        for(int  x = 0;  x < table_size;  ++x)
        {
          auto&  e = *row++;

          direction  dir;

            while(dir)
            {
              auto  dst = e.get_link(dir++);

                if(dst && (e.get_height() > dst->get_height()))
                {
                  e.erode(*dst,64000);

                  dst->collect_fraction();
                }
            }


          e.collect_fraction();
        }
    }


    if(y >= table_size)
    {
      y = 0;

      return true;
    }


  return false;
}


void
draw() noexcept
{
    for(int  y = 0;  y < table_size;  ++y){
    for(int  x = 0;  x < table_size;  ++x){
      auto&  e = table[y][x];

      auto&  pixel = bitmap[y][x];

      auto  h = e.get_height();

        if(h > sea_level)
        {
          h -= sea_level;

          int  v = 128+(h>>5);

          pixel = SDL_MapRGB(&format,v,v,v);
        }

      else
        {
          int  v = (h>>5);

          pixel = SDL_MapRGB(&format,0,0,127-v);
        }
    }}


  auto  white = SDL_MapRGB(&format,255,255,255);
  auto    red = SDL_MapRGB(&format,255,  0,  0);

    for(int  x = 0;  x < table_size;  ++x)
    {
      bitmap[table_size-1-((sea_level>>5)/pixel_size)][x] = white;
    }


  int  prev_n = 0;

    for(int  x = 0;  x < table_size;  ++x)
    {
      int  n = 0;

      bool  hot = false;

        for(int  y = 0;  y < table_size;  ++y)
        {
          auto&  e = table[y][x];

            if(e.test_hot())
            {
              hot = true;
            }


          n = std::max(n,e.get_height());
        }


      n = table_size-1-((n>>5)/pixel_size);

        if(x)
        {
          int  mod =  (prev_n < n)?  1
                     :(prev_n > n)? -1
                     :               0; 

            while(n != prev_n)
            {
              bitmap[prev_n][x-1] = white;

              prev_n += mod;
            }
        }

      else
        {
          prev_n = n;
        }


      bitmap[n][x] = white;

        if(hot)
        {
            while(n < screen_size)
            {
              bitmap[n++][x] = red;
            }
        }
    }
}


std::list<context>
ctxs;


std::list<context>::iterator
current_ctx;


void
main_loop() noexcept
{
  static SDL_Event  evt;

    while(SDL_PollEvent(&evt))
    {
        switch(evt.type)
        {
      case(SDL_KEYDOWN):
//          print();
          break;
      case(SDL_MOUSEBUTTONDOWN):
            if(ctxs.size() < 20)
            {
              auto&  e = table[evt.button.y/pixel_size]
                              [evt.button.x/pixel_size];

              ctxs.emplace_back(e,30000,edge_length*120);
 
               if(current_ctx == ctxs.end())
               {
                 current_ctx = ctxs.begin();
               }
            }
          break;
      case(SDL_MOUSEMOTION):
            if(evt.motion.state&SDL_BUTTON_LMASK)
            {
            }
          break;
      case(SDL_QUIT):
          quit();
          break;
        }
    }


    if(ctxs.empty())
    {
        if(erode(64))
        {
          need_to_redraw = true;
        }
    }

  else
    {
      static int  pc;

        switch(pc)
        {
      case(0):
          current_ctx->prepare_to_flow();
          ++pc;
          break;
      case(1):
            if(!current_ctx->flow_lava(64))
            {
                if(!*current_ctx)
                {
                  current_ctx = ctxs.erase(current_ctx);
                }

              else
                {
                  ++current_ctx;
                }


                if(current_ctx == ctxs.end())
                {
                  current_ctx = ctxs.begin();

                  ++pc;
                }

              else
                {
                  pc = 0;
                }
            }
          break;
      case(2):
            if(erode(64))
            {
              pc = 0;

              need_to_redraw = true;
            }

          break;
        }
    }



    if(need_to_redraw)
    {
      draw();

      update();

      need_to_redraw = false;
    }
}


}




int
main(int  argc, char**  argv)
{
  diagonal_length = std::sqrt( (edge_length*edge_length)
                              +(edge_length*edge_length));

  SDL_Init(SDL_INIT_VIDEO);

  window  = SDL_CreateWindow("",0,0,screen_size,screen_size,0);
  surface = SDL_GetWindowSurface(window);

  pitch  = surface->pitch;
  format = *surface->format;

    for(int  y = 0;  y < table_size;  ++y){
    for(int  x = 0;  x < table_size;  ++x){
      auto&  e = table[y][x];

      e.set_pos(x,y);

      bool   is_left_exist = (x                 );
      bool  is_right_exist = (x < (table_size-1));
      bool     is_up_exist = (y                 );
      bool   is_down_exist = (y < (table_size-1));


      e.set_link(      left_dir,(is_left_exist                  )? &table[y  ][x-1]:nullptr);
      e.set_link(   up_left_dir,(is_left_exist && is_up_exist   )? &table[y-1][x-1]:nullptr);
      e.set_link( down_left_dir,(is_left_exist && is_down_exist )? &table[y+1][x-1]:nullptr);
      e.set_link(     right_dir,(is_right_exist                 )? &table[y  ][x+1]:nullptr);
      e.set_link(  up_right_dir,(is_right_exist && is_up_exist  )? &table[y-1][x+1]:nullptr);
      e.set_link(down_right_dir,(is_right_exist && is_down_exist)? &table[y+1][x+1]:nullptr);
      e.set_link(        up_dir,(                  is_up_exist  )? &table[y-1][x  ]:nullptr);
      e.set_link(      down_dir,(                  is_down_exist)? &table[y+1][x  ]:nullptr);
    }}


  current_ctx = ctxs.begin();

  draw();

  update();

#ifdef EMSCRIPTEN
  emscripten_set_main_loop(main_loop,0,true);
#else
    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }
#endif


  quit();

  return 0;
}





