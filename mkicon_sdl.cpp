#include"sdl.hpp"
#include"libgbstd/widget.hpp"



using namespace gbstd;


namespace{


constexpr int  screen_w = 480;
constexpr int  screen_h = 400;


namespace types{
class canvas;
class   farm;
}


namespace ptrs{
types::canvas*  cv;
types::farm*  farm;
}


gbstd::image
image(screen_w,screen_h);


widgets::root
root;


gbstd::widget*
color_sample;


gbstd::images::color_index
current_color;


void
update_color() noexcept;


widgets::dial  r_dial(0,7,[](widgets::dial&  d, int  old_value, int  new_value){update_color();});
widgets::dial  g_dial(0,7,[](widgets::dial&  d, int  old_value, int  new_value){update_color();});
widgets::dial  b_dial(0,7,[](widgets::dial&  d, int  old_value, int  new_value){update_color();});


void
update_color() noexcept
{
  current_color = gbstd::images::color_index(r_dial.get_current(),
                                             g_dial.get_current(),
                                             b_dial.get_current());

  color_sample->need_to_redraw();
}


constexpr int  cv_w = 16;
constexpr int  cv_h = 24;

gbstd::image
cv_image(cv_w,cv_h);


namespace types{


class
farm: public gbstd::widget
{
  static constexpr int  w = 8;
  static constexpr int  h = 8;

  struct row{
    int  offset;
    int  number;

  };

  static constexpr row  m_rows[h]={
    {3,5},
    {2,6},
    {1,7},
    {0,8},
    {0,8},
    {1,7},
    {2,6},
    {3,5},
  };

public:
  farm() noexcept: widget(cv_w*w,cv_h*h){}

  void  render(gbstd::image_cursor  cur) noexcept override
  {
    image_cursor  dst_base_cur(cur);

    image_frame   src_frm(cv_image);

      for(int  y = 0;  y < h;  ++y)
      {
        cur.draw_hline(gbstd::images::predefined::gray,0,cv_h*y,cv_w*w);
      }


      for(int  x = 0;  x < w;  ++x)
      {
        cur.draw_vline(gbstd::images::predefined::gray,(cv_w*2)*x,0,cv_h*h);
      }


      for(int  y = 0;  y < h;  ++y)
      {
        auto&  row = m_rows[y];

        image_cursor  dst_cur(dst_base_cur)                    ;
                              dst_base_cur.add_offset(0,cv_h/2);

        dst_cur.add_offset((cv_w/2)*row.offset,0);

          for(int  n = 0;  n < row.number;  ++n)
          {
            images::transfer(src_frm,dst_cur);

            dst_cur.add_offset(cv_w,0);
          }
      }


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
        dst.index = current_color;

        need_to_redraw();

        ptrs::farm->need_to_redraw();
      }

    else
      if(gbstd::ctrl.is_mouse_rbutton_pressed())
      {
        dst.index = 0;

        need_to_redraw();
      }
  }

  void  render(gbstd::image_cursor  cur) noexcept override
  {
    cur.fill_rectangle(gbstd::images::predefined::black,0,0,pixel_size*cv_w,pixel_size*cv_h);

      for(int  y = 0;  y < cv_h;  ++y){
      for(int  x = 0;  x < cv_w;  ++x){
        cur.fill_rectangle(cv_image.get_pixel(x,y),pixel_size*x,pixel_size*y,pixel_size,pixel_size);
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
    }
}


void
save(widgets::button&  btn) noexcept
{
    for(int  y = 0;  y < cv_h;  ++y)
    {
        for(int  x = 0;  x < cv_w;  ++x)
        {
          auto  v = cv_image.get_pixel(x,y);

          printf("0%4o,",v.index);
        }


      printf("\n");
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

  color_sample = new types::color_sample;

  auto  pal = new widgets::table_column({
    color_sample,
    new widgets::table_row({new widgets::label(u"[ R ]"),r_dial.get_widget()}),
    new widgets::table_row({new widgets::label(u"[ G ]"),g_dial.get_widget()}),
    new widgets::table_row({new widgets::label(u"[ B ]"),b_dial.get_widget()}),
  });


  ptrs::cv   = new types::canvas;
  ptrs::farm = new types::farm;

  auto  clra_btn = new widgets::button(new widgets::label(u"CLEAR ALL"),clear_all);;
  auto  fila_btn = new widgets::button(new widgets::label(u" FILL ALL"),fill_all);;
  auto  save_btn = new widgets::button(new widgets::label(u"SAVE"),save);;

  auto  lcol = new widgets::table_column({pal,clra_btn,fila_btn,save_btn});

  auto  row = new widgets::table_row({ptrs::cv,lcol,ptrs::farm});

  root.append_child(row,0,0);

  root.show_all();

    for(;;)
    {
      main_loop();

      SDL_Delay(20);
    }


  sdl::quit();

  return 0;
}




