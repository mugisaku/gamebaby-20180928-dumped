#include"libgbstd/image.hpp"
#include"libgbstd/controller.hpp"
#include"libgbstd/widget.hpp"
#include"sdl.hpp"


#ifdef EMSCRIPTEN
#include<emscripten.h>
#include<SDL.h>
#endif


using namespace gbstd;


namespace{


constexpr int  screen_w = 480;
constexpr int  screen_h = 400;


namespace types{
class       farm;
class table_view;
}


namespace ptrs{
types::farm*      farm;
types::table_view*  tv;
}


gbstd::image
image(screen_w,screen_h);


widgets::root
root;


widgets::canvas*
cv;



bool
need_to_hide_cursors;


constexpr int  cv_w = 12;
constexpr int  cv_h = 24;

constexpr int  farm_w = 10;
constexpr int  farm_h = 13;

gbstd::image
farm_image(cv_w*farm_w,cv_h*4);


gbstd::image
cv_image(cv_w,cv_h,{
00000,00000,00000,00000,01000,01000,01000,01000,00000,00000,00000,00000,
00000,00000,00000,01000,01000,01030,01050,01000,01000,00000,00000,00000,
00000,00000,01000,01000,01030,01050,01050,01050,01000,01000,00000,00000,
00000,00000,01000,01000,01050,01050,01050,01050,01000,01000,00000,00000,
00000,01000,01000,01030,01050,01030,01030,01050,01050,01000,01000,00000,
00000,01000,01000,01030,01030,01050,01030,01050,01050,01000,01000,00000,
01000,01000,01030,01030,01050,01050,01050,01050,01050,01030,01000,01000,
01000,01000,01050,01030,01030,01050,01050,01030,01050,01030,01000,01000,
01000,01030,01050,01030,01030,01050,01030,01050,01050,01050,01050,01000,
01000,01030,01030,01050,01050,01050,01050,01030,01050,01050,01050,01000,
01000,01050,01030,01030,01050,01050,01030,01050,01050,01050,01030,01000,
01000,01030,01050,01050,01030,01050,01050,01050,01030,01050,01050,01000,
01000,01030,01050,01050,01050,01050,01030,01030,01050,01050,01050,01000,
01000,01030,01030,01030,01050,01050,01050,01030,01030,01050,01030,01000,
01000,01030,01030,01030,01050,01030,01050,01030,01050,01030,01030,01000,
01000,01030,01030,01030,01030,01030,01050,01050,01030,01030,01030,01000,
01000,01000,01030,01030,01030,01030,01030,01030,01030,01030,01000,01000,
00000,01000,01000,01030,01030,01030,01030,01030,01030,01000,01000,00000,
00000,00000,01000,01000,01030,01030,01030,01030,01000,01000,00000,00000,
00000,00000,00000,01000,01000,01000,01000,01000,01000,00000,00000,00000,
00000,00000,00000,00000,01000,01200,01200,01000,00000,00000,00000,00000,
00000,00000,00000,00000,01000,01200,01200,01000,00000,00000,00000,00000,
00000,00000,00000,00000,01000,01200,01200,01000,00000,00000,00000,00000,
00000,00000,00000,00000,00000,01000,01000,00000,00000,00000,00000,00000,
00000,00000,00000,00000,01000,01000,01000,01000,00000,00000,00000,00000,
00000,00000,00000,01000,01000,01030,01050,01000,01000,00000,00000,00000,
00000,00000,01000,01000,01030,01050,01050,01050,01000,01000,00000,00000,
00000,00000,01000,01000,01050,01050,01050,01050,01000,01000,00000,00000,
00000,01000,01000,01030,01050,01030,01030,01050,01050,01000,01000,00000,
00000,01000,01000,01030,01030,01050,01030,01050,01050,01000,01000,00000,
01000,01000,01030,01030,01050,01050,01050,01050,01050,01030,01000,01000,
01000,01000,01050,01030,01030,01050,01050,01030,01050,01030,01000,01000,
01000,01030,01050,01030,01030,01050,01030,01050,01050,01050,01050,01000,
01000,01030,01030,01050,01050,01050,01050,01030,01050,01050,01050,01000,
01000,01050,01030,01030,01050,01050,01030,01050,01050,01050,01030,01000,
01000,01030,01050,01050,01030,01050,01050,01050,01030,01050,01050,01000,
01000,01030,01050,01050,01050,01050,01030,01030,01050,01050,01050,01000,
01000,01030,01030,01030,01050,01050,01050,01030,01030,01050,01030,01000,
01000,01030,01030,01030,01050,01030,01050,01030,01050,01030,01030,01000,
01000,01030,01030,01030,01030,01030,01050,01050,01030,01030,01030,01000,
01000,01000,01030,01030,01030,01030,01030,01030,01030,01030,01000,01000,
00000,01000,01000,01030,01030,01030,01030,01030,01030,01000,01000,00000,
00000,00000,01000,01000,01030,01030,01030,01030,01000,01000,00000,00000,
00000,00000,00000,01000,01000,01000,01000,01000,01000,00000,00000,00000,
00000,00000,00000,00000,01000,01200,01200,01000,00000,00000,00000,00000,
00000,00000,00000,00000,01000,01200,01200,01000,00000,00000,00000,00000,
00000,00000,00000,00000,01000,01200,01200,01000,00000,00000,00000,00000,
00000,00000,00000,00000,00000,01000,01000,00000,00000,00000,00000,00000,
});


constexpr int  table_width = 6;

gbstd::point
table[table_width][table_width];


void
make_farm() noexcept
{
  struct row{
    int  offset;
    int  number;

  };

  static constexpr row  rows[farm_h]={
    {7, 3},
    {6, 4},
    {5, 5},
    {4, 6},
    {3, 7},
    {2, 8},
    {1, 9},
    {0,10},
    {1,9},
    {2,8},
    {3,7},
    {4,6},
    {5,5},
  };


  farm_image.fill(gbstd::images::predefined_color::blue);

  image_cursor  dst_base_cur(farm_image);

    for(int  y = 0;  y < farm_h;  ++y)
    {
      auto&  row = rows[y];

      image_cursor  dst_cur(dst_base_cur)                    ;
                            dst_base_cur.add_offset(0,cv_h/4);

      dst_cur.add_offset((cv_w/2)*row.offset,0);

        for(int  n = 0;  n < row.number;  ++n)
        {
          images::transfer(cv_image,point(),0,0,dst_cur);

          dst_cur.add_offset(cv_w,0);
        }
    }
}


namespace types{


class
farm: public gbstd::widget
{
  point  m_fixed_point;
  point  m_float_point;

public:
  farm() noexcept: widget(cv_w*farm_w,cv_h*4){}

  const gbstd::point&  get_fixed_point() const noexcept{return m_fixed_point;}

  void  do_when_mouse_acted(int  x, int  y) noexcept override
  {
    m_float_point.x = x/(cv_w*2);
    m_float_point.y = y/(cv_h  );

      if(gbstd::ctrl.is_mouse_lbutton_pressed())
      {
        m_fixed_point = m_float_point;
      }


    need_to_redraw();
  }
  

  void  render(gbstd::image_cursor  cur) noexcept override
  {
    images::transfer(farm_image,point(),0,0,cur);

      if(!need_to_hide_cursors)
      {
        constexpr int  w = cv_w*2;
        constexpr int  h = cv_h;

        using gbstd::images::predefined_color;

        cur.draw_doubleline_rectangle(predefined_color::black,predefined_color::white,w*m_fixed_point.x,h*m_fixed_point.y,w,h);
        cur.draw_doubleline_rectangle(predefined_color::red  ,predefined_color::white,w*m_float_point.x,h*m_float_point.y,w,h);
      }
  }

};




class
table_view: public gbstd::widget
{
  point  m_point;

public:
  table_view() noexcept: widget(cv_w*2*table_width,cv_h*table_width){}

  void  do_when_mouse_acted(int  x, int  y) noexcept override
  {
    m_point.x = x/(cv_w*2);
    m_point.y = y/(cv_h  );

      if(gbstd::ctrl.is_mouse_lbutton_pressed())
      {
        auto&  pt = table[m_point.y][m_point.x];

        pt.x = (cv_w*2)*ptrs::farm->get_fixed_point().x;
        pt.y = (cv_h  )*ptrs::farm->get_fixed_point().y;
      }


    need_to_redraw();
  }

  void  render(gbstd::image_cursor  cur) noexcept override
  {
      for(int  y = 0;  y < table_width;  ++y){
      for(int  x = 0;  x < table_width;  ++x){
        auto  pt = table[y][x];

        images::transfer(farm_image,pt,cv_w*2,cv_h,cur+point(cv_w*2*x,cv_h*y));
      }}


    cur.draw_rectangle(gbstd::images::predefined_color::white,(cv_w*2)*m_point.x,cv_h*m_point.y,cv_w*2,cv_h);
  }

};


}




void
save(widgets::button&  btn) noexcept
{
    if(btn.get_count())
    {
      btn.reset_count();

#ifdef EMSCRIPTEN
      need_to_hide_cursors = true;

      ptrs::farm->redraw(image);

      need_to_hide_cursors = false;

      sdl::update_screen(image);


      char  buf[256];

      auto&  pt = ptrs::farm->get_absolute_point();
      auto    w = ptrs::farm->get_width();
      auto    h = ptrs::farm->get_height();

      snprintf(buf,sizeof(buf),
        "  var  src = document.getElementById(\"canvas\");"
        "  var  clp = document.createElement(\"canvas\");"
        "  var  ctx = clp.getContext(\"2d\");"
        "  ctx.drawImage(src,%d,%d,%d,%d,0,0,%d,%d);"
        "  var  img = document.getElementById(\"img\");"
        "  img.src = clp.toDataURL();",pt.x,pt.y,w,h,w,h);


      emscripten_run_script(buf);
#else
        for(int  y = 0;  y < cv_h;  ++y)
        {
            for(int  x = 0;  x < cv_w;  ++x)
            {
              auto  v = cv_image.get_pixel(x,y);

              printf("0%04o,",static_cast<unsigned int>(v.color));
            }


          printf("\n");
        }
#endif
    }
}


void
main_loop()
{
  sdl::update_controller();

  root.react();

    if(root.redraw_only_needed_widgets(image))
    {
      sdl::update_screen(image);
    }
}


}


int
main(int  argc, char**  argv)
{
  sdl::init(screen_w,screen_h);

  cv = new widgets::canvas(cv_image,[](widgets::canvas&  cv){
    make_farm();

    ptrs::farm->need_to_redraw();
    ptrs::tv->need_to_redraw();
  });

  cv->set_grid();
  cv->set_pixel_size(12);

  ptrs::farm = new types::farm;
  ptrs::tv   = new types::table_view;

  auto  save_btn = new widgets::button(new widgets::label(u"SAVE"),save);;

  auto  mcol = new widgets::table_column({pal,save_btn});
  auto  lcol = new widgets::table_column({ptrs::farm,ptrs::tv});

  auto  row = new widgets::table_row({cv,mcol,lcol});

  root->append_child(row,0,0);

  root->show_all();

  update_color();

  make_farm();

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




