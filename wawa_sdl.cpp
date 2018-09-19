#include"sdl.hpp"
#include<cstdio>
#include<list>
#include<vector>
#include<cmath>
#include<string>


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;


namespace{


constexpr int   g_space_size = 8;

constexpr int   g_side_length = g_space_size;
constexpr int   g_volume      = g_side_length*g_side_length;


struct space;


class
real_t
{
  static constexpr int  m_shift_amount = 16;

  int  m_data;

  struct raw_data{int  m_data;};

  constexpr real_t(raw_data  raw) noexcept: m_data(raw.m_data){}

public:
  constexpr real_t() noexcept: m_data(0){}
  constexpr real_t(int  i) noexcept: m_data(i<<m_shift_amount){}

  constexpr int  to_integer() const noexcept{return m_data>>m_shift_amount;}

  constexpr operator bool() const noexcept{return m_data;}

  constexpr real_t  operator+(real_t  rhs) const noexcept{return real_t(raw_data{m_data+rhs.m_data});}
  constexpr real_t  operator-(real_t  rhs) const noexcept{return real_t(raw_data{m_data-rhs.m_data});}
  constexpr real_t  operator*(real_t  rhs) const noexcept{return real_t(raw_data{m_data*rhs.to_integer()});}
  constexpr real_t  operator/(real_t  rhs) const noexcept{return real_t(raw_data{m_data/rhs.to_integer()});}
  constexpr real_t  operator%(real_t  rhs) const noexcept{return real_t(raw_data{m_data%rhs.to_integer()});}

  constexpr real_t  operator+(int  i) const noexcept{return real_t(raw_data{m_data+(i<<m_shift_amount)});}
  constexpr real_t  operator-(int  i) const noexcept{return real_t(raw_data{m_data-(i<<m_shift_amount)});}
  constexpr real_t  operator*(int  i) const noexcept{return real_t(raw_data{m_data*i});}
  constexpr real_t  operator/(int  i) const noexcept{return real_t(raw_data{m_data/i});}
  constexpr real_t  operator%(int  i) const noexcept{return real_t(raw_data{m_data%i});}

  constexpr bool  operator< (real_t  rhs) const noexcept{return m_data <  rhs.m_data;}
  constexpr bool  operator<=(real_t  rhs) const noexcept{return m_data <= rhs.m_data;}
  constexpr bool  operator> (real_t  rhs) const noexcept{return m_data >  rhs.m_data;}
  constexpr bool  operator>=(real_t  rhs) const noexcept{return m_data >= rhs.m_data;}
  constexpr bool  operator==(real_t  rhs) const noexcept{return m_data == rhs.m_data;}
  constexpr bool  operator!=(real_t  rhs) const noexcept{return m_data != rhs.m_data;}

  constexpr bool  operator< (int  i) const noexcept{return m_data <  real_t(i).m_data;}
  constexpr bool  operator<=(int  i) const noexcept{return m_data <= real_t(i).m_data;}
  constexpr bool  operator> (int  i) const noexcept{return m_data >  real_t(i).m_data;}
  constexpr bool  operator>=(int  i) const noexcept{return m_data >= real_t(i).m_data;}
  constexpr bool  operator==(int  i) const noexcept{return m_data == real_t(i).m_data;}
  constexpr bool  operator!=(int  i) const noexcept{return m_data != real_t(i).m_data;}

  real_t&  operator+=(real_t  rhs) noexcept{m_data += rhs.m_data;  return *this;}
  real_t&  operator-=(real_t  rhs) noexcept{m_data -= rhs.m_data;  return *this;}
  real_t&  operator*=(real_t  rhs) noexcept{m_data *= rhs.to_integer();  return *this;}
  real_t&  operator/=(real_t  rhs) noexcept{m_data /= rhs.to_integer();  return *this;}
  real_t&  operator%=(real_t  rhs) noexcept{m_data %= rhs.to_integer();  return *this;}

  real_t&  operator =(int  i) noexcept{m_data  = real_t(i).m_data;  return *this;}
  real_t&  operator+=(int  i) noexcept{m_data += real_t(i).m_data;  return *this;}
  real_t&  operator-=(int  i) noexcept{m_data -= real_t(i).m_data;  return *this;}
  real_t&  operator*=(int  i) noexcept{m_data *= i               ;  return *this;}
  real_t&  operator/=(int  i) noexcept{m_data /= i               ;  return *this;}
  real_t&  operator%=(int  i) noexcept{m_data %= i               ;  return *this;}

  std::string  to_string() const noexcept
  {
    char  buf[80];

    snprintf(buf,sizeof(buf),"%f",((m_data>>m_shift_amount)*1.0)+((m_data&0xFFFF)/65536.0));

    return std::string(buf);
  }

  void  print() const noexcept
  {
    auto  s = to_string();

    printf("%s",s.data());
  }

};


struct
plant
{
  int  m_x=0;
  int  m_y=0;

  int  m_lifespan=3000;

  real_t  m_leaf_area=0;
  real_t  m_root_length=0;
  real_t  m_height=1;

  real_t  m_water_amount =16;
  real_t  m_starch_amount=8;

  space*  m_base_space=nullptr;

  std::vector<space*>  m_root_spaces;

  plant(int  x, int  y) noexcept: m_x(x),m_y(y){}

  void  compose_starch(real_t  light_amount) noexcept;

  void  absorb_water() noexcept;
  void    metabolize() noexcept;


  void    step() noexcept;

};


struct
space
{
  enum kind{
    null,
    earth,

  } m_kind=kind::null;


  int  m_counter=0;

  real_t  m_free_water_amount =0;
  real_t  m_bound_water_amount=0;

  space*  m_left =nullptr;
  space*  m_right=nullptr;
  space*  m_up   =nullptr;
  space*  m_down =nullptr;

  real_t  get_water_amount() const noexcept{return m_bound_water_amount+m_free_water_amount;}

  void  add_water(real_t  amount) noexcept;

  void  move_water_from(space*  src) noexcept;
  void  move_water() noexcept;

};


struct
water_drop
{
  int  m_x=0;
  int  m_y=0;

  int  m_y_last=0;

  constexpr water_drop(int  x=0, int  y=0) noexcept:
  m_x(x), m_y(y), m_y_last(y){}

};


constexpr int   g_table_w = 30;
constexpr int   g_table_h = 30;

space
g_table[g_table_h][g_table_w];

space*
get_space(int  x, int  y) noexcept
{
    if((x >=         0) &&
       (y >=         0) &&
       (x <  g_table_w) &&
       (y <  g_table_h))
    {
      return &g_table[y][x];
    }


  return nullptr;
}


constexpr int  g_screen_w = g_space_size*g_table_w;
constexpr int  g_screen_h = g_space_size*g_table_h;


std::list<water_drop>
g_water_drops;


std::list<plant>
g_plants;


images::image
g_final_image;


void
link() noexcept
{
    for(int  y = 0;  y < g_table_h;  ++y){
    for(int  x = 0;  x < g_table_w;  ++x){
      auto&  sp = g_table[y][x];

      sp.m_up    = (y                )? &g_table[y-1][x  ]:nullptr;
      sp.m_left  = (x                )? &g_table[y  ][x-1]:nullptr;
      sp.m_right = (x < (g_table_w-1))? &g_table[y  ][x+1]:nullptr;
      sp.m_down  = (y < (g_table_h-1))? &g_table[y+1][x  ]:nullptr;
    }}
}


void
render() noexcept
{
  g_final_image.fill();

    for(int  y = 0;  y < g_table_h;  ++y){
    for(int  x = 0;  x < g_table_w;  ++x){
      auto&  sp = g_table[y][x];

      int  xx = g_space_size*x;
      int  yy = g_space_size*y;

        switch(sp.m_kind)
        {
      case(space::kind::null):
          g_final_image.fill_rectangle(colors::white,xx,yy,g_space_size,g_space_size);
          break;
      case(space::kind::earth):
          {
            auto  w = sp.get_water_amount();

            int  g = (w >= 64)? 0:7-(real_t(7)/(g_space_size*g_space_size)*w.to_integer()).to_integer();

            g_final_image.fill_rectangle(color(7,g,0),xx,yy,g_space_size,g_space_size);
          }
          break;
        }
    }}


    for(auto&  drop: g_water_drops)
    {
      g_final_image.fill_rectangle_safely(colors::blue,
        drop.m_x-(g_space_size/2),
        drop.m_y-(g_space_size/2),g_space_size,g_space_size);
    }


    for(auto&  p: g_plants)
    {
      g_final_image.fill_rectangle_safely(colors::black,
        p.m_x-2,
        p.m_y-2,4,4);


      auto  l = p.m_root_length.to_integer();
      auto  h = p.m_height.to_integer();

        for(int  n = 0;  n < l;  ++n)
        {
          g_final_image.draw_dot_safely(colors::white,p.m_x  ,p.m_y+1+n);
          g_final_image.draw_dot_safely(colors::white,p.m_x-1,p.m_y+1+n);
          g_final_image.draw_dot_safely(colors::white,p.m_x+1,p.m_y+1+n);
        }


        for(int  n = 0;  n < h;  ++n)
        {
          g_final_image.draw_dot_safely(colors::green,p.m_x  ,p.m_y-1-n);
          g_final_image.draw_dot_safely(colors::green,p.m_x-1,p.m_y-1-n);
          g_final_image.draw_dot_safely(colors::green,p.m_x+1,p.m_y-1-n);
        }
    }
}


void
space::
add_water(real_t  amount) noexcept
{
/*
  m_water_amount += amount;

    if(amount > 0)
    {
      m_counter = 0;
    }
*/
}


void
space::
move_water_from(space*  src) noexcept
{
    if(src)
    {
        if(src->m_kind == space::kind::earth)
        {
            if(src->m_bound_water_amount > (m_bound_water_amount))
            {
              auto  v = (src->m_bound_water_amount-m_bound_water_amount)/128;

              src->m_bound_water_amount -= v;
                   m_bound_water_amount += v;
            }
        }

      else
        if(src->m_kind == space::kind::null)
        {
          constexpr real_t  v = real_t(1)/16;

            if(m_bound_water_amount >= v)
            {
              m_bound_water_amount -= v;
            }

          else
            {
              m_bound_water_amount = 0;
            }
        }
    }
}


void
space::
move_water() noexcept
{
    if(m_bound_water_amount < 64)
    {
      auto  amount = (real_t(64)-m_bound_water_amount);

      amount = std::min(amount,m_free_water_amount);

      m_free_water_amount  -= amount;
      m_bound_water_amount += amount;
    }


    if(m_free_water_amount >= 64)
    {
        if(m_down)
        {
          auto  amount = (m_free_water_amount-64)/12;

                  m_free_water_amount -= amount;
          m_down->m_free_water_amount += amount;
        }
    }

  else
    {
      move_water_from(m_up   );
      move_water_from(m_left );
      move_water_from(m_right);
      move_water_from(m_down );
    }
}


void
fall_drops() noexcept
{
  auto  it     = g_water_drops.begin();
  auto  it_end = g_water_drops.end();

    while(it != it_end)
    {
      auto&  drop = *it;

      int  x = drop.m_x/g_space_size;
      int  y = drop.m_y/g_space_size;

        if(y >= g_table_h)
        {
          it     = g_water_drops.erase(it);
          it_end = g_water_drops.end();

          continue;
        }


      auto  center = get_space(x,y);

        if(center->m_kind == space::kind::earth)
        {
          auto   left = get_space(x-1,y);
          auto  right = get_space(x+1,y);

          center->m_free_water_amount += 32;

            if( left){ left->m_free_water_amount += 12;}
            if(right){right->m_free_water_amount += 12;}

          it     = g_water_drops.erase(it);
          it_end = g_water_drops.end();
        }

      else
        {
          drop.m_y += 4;

          ++it;
        }
    }
}




void
plant::
metabolize() noexcept
{
  constexpr real_t  required_water_amount_per_growth  = real_t(1)/3;
  constexpr real_t  required_strach_amount_per_growth = real_t(1)/4;

    if((m_starch_amount >= required_strach_amount_per_growth) &&
       (m_water_amount  >=  required_water_amount_per_growth))
    {
      m_starch_amount -= required_strach_amount_per_growth;
      m_water_amount -= required_water_amount_per_growth;

      m_height      += real_t(1)/48;
      m_root_length += real_t(1)/52;


      auto  sp = &g_table[(m_y+m_root_length.to_integer())/g_space_size][m_x/g_space_size];

        if(m_root_spaces.empty() || (sp != m_root_spaces.back()))
        {
          m_root_spaces.emplace_back(sp);
        }
    }


    if(m_lifespan)
    {
      --m_lifespan;
    }
}


void
plant::
absorb_water() noexcept
{
  const real_t  water_amount_limit((m_root_length+m_height)*g_space_size*3);

    for(auto  sp: m_root_spaces)
    {
        if(m_water_amount >= water_amount_limit)
        {
          break;
        }


      constexpr real_t  v = real_t(1)/2;

        if(sp->m_free_water_amount > v)
        {
          sp->m_free_water_amount -= v;
                   m_water_amount += v;
        }

      else
        if(sp->m_bound_water_amount > v)
        {
          sp->m_bound_water_amount -= v;
                    m_water_amount += v;
        }
    }
}


void
plant::
compose_starch(real_t  light_amount) noexcept
{
  constexpr real_t  required_water_amount_per_starch(6);
  constexpr real_t  required_light_amount_per_starch(8);

  real_t  l(  light_amount/required_light_amount_per_starch);
  real_t  w(m_water_amount/required_water_amount_per_starch);

  int  n = std::min(l.to_integer(),w.to_integer());

  m_water_amount  -= w*n;
  m_starch_amount +=   n;
}


void
plant::
step() noexcept
{
  compose_starch(m_height*g_space_size*3);

  absorb_water();

  metabolize();

/*
printf("w ");
m_water_amount.print();
printf(", s ");
m_starch_amount.print();
printf("\r");
*/
}


void
move_plants() noexcept
{
  auto  it     = g_plants.begin();
  auto  it_end = g_plants.end();

    while(it != it_end)
    {
      auto&  p = *it;

      int  x = p.m_x/g_space_size;
      int  y = p.m_y/g_space_size;

        if((y >= g_table_h) || !p.m_lifespan || (p.m_height >= (g_screen_h/4)))
        {
          it     = g_plants.erase(it);
          it_end = g_plants.end();

          continue;
        }


      auto  center = get_space(x,y);

        if(center->m_kind == space::kind::earth)
        {
          p.m_base_space = center;

          p.step();

          ++it;
        }

      else
        {
          p.m_y += 4;

          ++it;
        }
    }
}


void
step() noexcept
{
  auto  p     = &g_table[        0][0];
  auto  p_end = &g_table[g_table_h][0];

  static int  flag;

    if(flag++&1)
    {
      p += g_table_w;
    }


    while(p < p_end)
    {
        for(int  x = 0;  x < g_table_w;  ++x)
        {
          p[x].move_water();
        }


      p += (g_table_w*2);
    }


  fall_drops();

  move_plants();
}


void
main_loop() noexcept
{
  auto&  condev = sdl::update_control_device();

  auto&  m = condev.mouse;

    if(m.left_button)
    {
      int  x = m->x;
      int  y = m->y;

      g_water_drops.emplace_back(m->x,m->y);
    }

  else
    if(m.right_button.get_counter())
    {
      m.right_button.reset_counter();

      int  x = m->x;
      int  y = m->y;

      g_plants.emplace_back(m->x,m->y);
    }


  static uint32_t  next_time;

    if((condev.time >= next_time) || condev.needed_to_redraw)
    {
      constexpr uint32_t  interval = 40;

      next_time = condev.time+interval;

        step();
      render();

//printf("%3d\n",g_water_drops.size());
      goto NEXT;
    }


  return;

NEXT:
  sdl::update_screen(g_final_image);
}


}




int
main(int  argc, char**  argv)
{
#ifdef EMSCRIPTEN
  show_github_link();

  set_caption("水の拡散と蒸発とついでに植物のシミュレート"/* __DATE__*/);
  set_description("<pre>"
                  "マウスの左ボタンを押して、水やり\n"
                  "右クリックで、種まき\n"
                  "植物は一定時間経つが、一定以上に成長すると消滅\n"
                  "</pre>"
  );
#endif
  link();

    for(int  y = g_table_h/2;  y < g_table_h;  ++y){
    for(int  x =           0;  x < g_table_w;  ++x){
      g_table[y][x].m_kind = space::kind::earth;
    }}


  sdl::init(g_screen_w,g_screen_h);

  g_final_image = sdl::make_screen_image();

  sdl::update_screen(g_final_image);


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




