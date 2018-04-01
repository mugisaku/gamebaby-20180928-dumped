#include<SDL.h>
#include"libgbstd/image.hpp"
#include"libgbstd/controller.hpp"
#include"sdl_screen.cpp"
#include"sdl_controller.cpp"

#include"libgbstd/widget.hpp"


#ifdef EMSCRIPTEN
#include<emscripten.h>
#include<SDL.h>
#endif


using namespace gbstd;


namespace{


constexpr int  screen_w = 480;
constexpr int  screen_h = 400;


namespace types{
class     canvas;
class       farm;
class table_view;
}


namespace ptrs{
types::canvas*      cv;
types::farm*      farm;
types::table_view*  tv;
}


gbstd::image
image(screen_w,screen_h);


widgets::root
root;


gbstd::widget*
color_sample;


gbstd::images::color_index
current_color;


bool
need_to_hide_cursors;


void
update_color() noexcept;


widgets::dial*  r_dial;
widgets::dial*  g_dial;
widgets::dial*  b_dial;


void
update_color() noexcept
{
  current_color = gbstd::images::color_index(r_dial->get_current(),
                                             g_dial->get_current(),
                                             b_dial->get_current());

  color_sample->need_to_redraw();
}


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


  farm_image.fill(gbstd::images::predefined::blue);

  image_cursor  dst_base_cur(farm_image);

  image_frame   src_frm(cv_image,point(),0,0);

    for(int  y = 0;  y < farm_h;  ++y)
    {
      auto&  row = rows[y];

      image_cursor  dst_cur(dst_base_cur)                    ;
                            dst_base_cur.add_offset(0,cv_h/4);

      dst_cur.add_offset((cv_w/2)*row.offset,0);

        for(int  n = 0;  n < row.number;  ++n)
        {
          images::transfer(src_frm,dst_cur);

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
    images::transfer(image_frame(farm_image,point(),0,0),cur);

      if(!need_to_hide_cursors)
      {
        constexpr int  w = cv_w*2;
        constexpr int  h = cv_h;

        using gbstd::images::predefined;

        cur.draw_doubleline_rectangle(predefined::black,predefined::white,w*m_fixed_point.x,h*m_fixed_point.y,w,h);
        cur.draw_doubleline_rectangle(predefined::red  ,predefined::white,w*m_float_point.x,h*m_float_point.y,w,h);
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

        image_frame   src(farm_image,pt,cv_w*2,cv_h);

        images::transfer(src,cur+point(cv_w*2*x,cv_h*y));
      }}


    cur.draw_rectangle(gbstd::images::predefined::white,(cv_w*2)*m_point.x,cv_h*m_point.y,cv_w*2,cv_h);
  }

};


class
canvas: public widgets::widget
{
  static constexpr int  pixel_size = 12;


public:
  canvas() noexcept: widget(pixel_size*cv_w,pixel_size*cv_h){}

  void  do_when_mouse_acted(int  x, int  y) noexcept override
  {
    auto&  dst = cv_image.get_pixel(x/pixel_size,y/pixel_size);

      if(gbstd::ctrl.is_mouse_lbutton_pressed())
      {
        dst.color_index = current_color;

        need_to_redraw();

        make_farm();

        ptrs::farm->need_to_redraw();
        ptrs::tv  ->need_to_redraw();
      }

    else
      if(gbstd::ctrl.is_mouse_rbutton_pressed())
      {
        dst.color_index = 0;

        need_to_redraw();

        make_farm();

        ptrs::farm->need_to_redraw();
        ptrs::tv  ->need_to_redraw();
      }
  }

  void  render(gbstd::image_cursor  cur) noexcept override
  {
    cur.fill_rectangle(gbstd::images::predefined::blue,0,0,pixel_size*cv_w,pixel_size*cv_h);

      for(int  y = 0;  y < cv_h;  ++y){
      for(int  x = 0;  x < cv_w;  ++x){
        auto&  pix = cv_image.get_pixel(x,y);

          if(pix.color_index)
          {
            cur.fill_rectangle(cv_image.get_pixel(x,y),pixel_size*x,pixel_size*y,pixel_size,pixel_size);
          }
      }}


      for(int  y = 0;  y < cv_h;  ++y)
      {
        cur.draw_hline(gbstd::images::predefined::gray,0,pixel_size*y,pixel_size*cv_w);
      }


      for(int  x = 0;  x < cv_w;  ++x)
      {
        cur.draw_vline(gbstd::images::predefined::gray,pixel_size*x,0,pixel_size*cv_h);
      }


    cur.draw_hline(gbstd::images::predefined::light_gray,0,pixel_size*(cv_h/2),pixel_size*cv_w);
    cur.draw_vline(gbstd::images::predefined::light_gray,pixel_size*(cv_w/2),0,pixel_size*cv_h);
  }

};


class
color_sample: public gbstd::widget
{
  static constexpr int   size = 32;

public:
  color_sample() noexcept: widget(size,size){}

  void  render(gbstd::image_cursor  cur) noexcept override
  {
    cur.fill_rectangle(current_color,0,0,size,size);
  }

};
}




void
clear_all(widgets::button&  btn) noexcept
{
    if(btn.get_count())
    {
      btn.reset_count();

      image_frame  frm(cv_image);

      frm.fill(pixel());


      ptrs::cv->need_to_redraw();

      make_farm();

      ptrs::farm->need_to_redraw();
      ptrs::tv->need_to_redraw();
    }
}


void
fill_all(widgets::button&  btn) noexcept
{
    if(btn.get_count())
    {
      btn.reset_count();

      image_frame  frm(cv_image);

      frm.fill(current_color);


      ptrs::cv->need_to_redraw();

      make_farm();

      ptrs::farm->need_to_redraw();
      ptrs::tv->need_to_redraw();
    }
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

              printf("0%04o,",static_cast<unsigned int>(v.color_index));
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

    if(root.is_needed_to_redraw())
    {
      root.redraw(image);

      sdl::update_screen(image);
    }
}


}


int
main(int  argc, char**  argv)
{
  sdl::init(screen_w,screen_h);

  r_dial = new widgets::dial(0,7,[](widgets::dial&  d, int  old_value, int  new_value){update_color();});
  g_dial = new widgets::dial(0,7,[](widgets::dial&  d, int  old_value, int  new_value){update_color();});
  b_dial = new widgets::dial(0,7,[](widgets::dial&  d, int  old_value, int  new_value){update_color();});

  color_sample = new types::color_sample;

  auto  pal = new widgets::table_column({
    color_sample,
    new widgets::table_row({new widgets::label(u"[ R ]"),r_dial}),
    new widgets::table_row({new widgets::label(u"[ G ]"),g_dial}),
    new widgets::table_row({new widgets::label(u"[ B ]"),b_dial}),
  });


  ptrs::cv   = new types::canvas;
  ptrs::farm = new types::farm;
  ptrs::tv   = new types::table_view;

  auto  clra_btn = new widgets::button(new widgets::label(u"CLEAR ALL"),clear_all);;
  auto  fila_btn = new widgets::button(new widgets::label(u" FILL ALL"),fill_all);;
  auto  save_btn = new widgets::button(new widgets::label(u"SAVE"),save);;

  auto  mcol = new widgets::table_column({pal,clra_btn,fila_btn,save_btn});
  auto  lcol = new widgets::table_column({ptrs::farm,ptrs::tv});

  auto  row = new widgets::table_row({ptrs::cv,mcol,lcol});

  root.append_child(row,0,0);

  root.show_all();

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
#endif


//  sdl::quit();

  return 0;
}




