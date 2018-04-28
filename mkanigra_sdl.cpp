#include"libgbstd/widget.hpp"
#include"libgbstd/window.hpp"
#include"sdl.hpp"


using namespace gbstd;


namespace{


constexpr int  screen_w = 600;
constexpr int  screen_h = 400;


widgets::canvas*
cv;


widgets::menu*
mnu;


images::image
image(screen_w,screen_h);


windows::window_manager
winman;


windows::window*  canvas_window;
windows::window*    cell_window;


images::image
cv_image;



namespace types{
struct
cell
{
  static int   width;
  static int  height;

  images::image  image;

};


int  cell::width ;
int  cell::height;


}


using types::cell;


cell  cells[80];


namespace cell_table{

constexpr int   width = 16;
constexpr int  height =  4;

cell*  current = cells;


void
resize_image(int  w, int  h) noexcept
{
  cv_image.resize(w,h);

  cv->need_to_reform();

  cell::width  = w;
  cell::height = h;

  mnu->set_item_size(w,h);

    for(auto&  cell: cells)
    {
      cell.image.resize(w,h);
    }


  mnu->need_to_reform();
}



cell*
get_pointer(point  index) noexcept
{
  int  i = ((width*index.y)+index.x);

  auto  ptr = &cells[i];

  return (ptr < std::end(cells))? ptr:nullptr;
}


void
send() noexcept
{
  auto  src = current->image.begin();
  auto  end = current->image.end();
  auto  dst =       cv_image.begin();

    while(src != end)
    {
      *dst++ = *src++;
    }
}


void
receive() noexcept
{
  auto  dst = current->image.begin();
  auto  src =       cv_image.begin();
  auto  end =       cv_image.end();

    while(src != end)
    {
      *dst++ = *src++;
    }
}


}


namespace animator{


std::vector<const cell*>
stack;


widgets::dial*
interval_dial;


uint32_t
interval_time;


uint32_t
last_time;


class
view: public widget
{
  int  m_index=0;

public:
  view() noexcept: widget(cell::width,cell::height+font_height){}

  void  advance() noexcept
  {
      if(++m_index >= stack.size())
      {
        m_index = 0;
      }


    need_to_redraw();
  }

  void  render(image_cursor  cur) noexcept override
  {
    cur.fill_rectangle(predefined_color::black,0,0,cell::width,cell::height+font_height);
    cur.draw_rectangle(predefined_color::white,0,0,cell::width,cell::height);

      if(m_index < stack.size())
      {
        auto&  img = stack[m_index]->image;

        images::transfer(img,img.get_rectangle(),cur);
      }


    string_form  sf;

    cur.draw_text(sf("%2d/%2d",m_index,stack.size()),text_style(),0,cell::height);
  }

};


animator::view
view;


void
check_time(uint32_t  now) noexcept
{
    if(stack.size())
    {
        if(now >= (last_time+interval_time))
        {
          last_time = now;

          view.advance();
        }
    }
}


}



void
save() noexcept
{
#ifdef EMSCRIPTEN
#else
#endif
}




void
main_loop() noexcept
{
  auto&  condev = sdl::update_control_device();

  animator::check_time(condev.time);

  winman.update(condev);

    if(winman.composite(image) || condev.needed_to_redraw)
    {
      sdl::update_screen(image);
    }
}


widget*
create_animation_widget() noexcept
{
  animator::interval_dial = new widgets::dial(1,8,[](widgets::dial&  d, int  old_value, int  new_value){
    animator::interval_time = 200*new_value;
  });

  auto  psh_btn = new widgets::button(new widgets::label(u"Push"),[](widgets::button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        animator::stack.emplace_back(cell_table::current);

        animator::view.need_to_redraw();
      }
  });

  auto  pop_btn = new widgets::button(new widgets::label(u"Pop"),[](widgets::button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

          if(animator::stack.size())
          {
            animator::stack.pop_back();

            animator::view.need_to_redraw();
          }
      }
  });

  auto  clr_btn = new widgets::button(new widgets::label(u"Clear"),[](widgets::button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

          if(animator::stack.size())
          {
            animator::stack.resize(0);

            animator::view.need_to_redraw();
          }
      }
  });


  auto  op_col = new widgets::table_column({psh_btn,pop_btn,clr_btn});

  return new widgets::frame(new widgets::table_row({&animator::view,op_col,animator::interval_dial}),"animation");
}


}


int
main(int  argc, char**  argv)
{
  sdl::init(screen_w,screen_h);


  cv = new widgets::canvas(cv_image,[](widgets::canvas&  cv){
    cell_table::receive();

    mnu->need_to_redraw();
  });


  cv->set_grid();
  cv->set_pixel_size(10);


  widgets::menu_item_parameter  mip = {0,0,
    [](point  index)->bool
    {
      auto&  mouse = winman.get_mouse();

        if(mouse.left_button)
        {
          auto  ptr = cell_table::get_pointer(index);

            if(ptr)
            {
              cell_table::current = ptr;

              cell_table::send();

              cv->get_drawing_recorder().clear();

              cv->need_to_redraw();

              return true;
            }
        }


      return false;
    },

    [](image_cursor  cur, point  index)
    {
      cur.fill_rectangle(images::predefined_color::blue,0,0,cell::width,
                                                            cell::height);

      auto  ptr = cell_table::get_pointer(index);

        if(ptr)
        {
          images::transfer(ptr->image,ptr->image.get_rectangle(),cur);

            if(ptr == cell_table::current)
            {
              cur.draw_rectangle(images::predefined_color::white,0,0,cell::width,
                                                                     cell::height);
            }
        }
    }
  };


  auto  color_maker = new widgets::frame(cv->create_color_maker(),"color");
  auto      cv_tool = cv->create_tool_widget();
  auto        cv_op = cv->create_operation_widget();

  mnu = new widgets::menu(mip,cell_table::width,cell_table::height);


  auto  urow = new widgets::table_row({new widgets::frame(cv,"canvas"),cv_tool,cv_op,color_maker});


  canvas_window = new window("canvas window");

  canvas_window->set_header_flag();
  canvas_window->set_movable_flag();

  (*canvas_window)->append_child(new widgets::table_column({urow}),0,0);

  (*canvas_window)->show_all();


  cell_window = new window("cell window");

  cell_window->set_header_flag();
  cell_window->set_movable_flag();

  (*cell_window)->append_child(new widgets::table_column({create_animation_widget(),new widgets::frame(mnu,"cell table")}),0,0);

  (*cell_window)->show_all();


  winman.append(canvas_window,0,0);
  winman.append(  cell_window,80,80);


  cell_table::resize_image(24,24);

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




