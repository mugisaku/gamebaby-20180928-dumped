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


images::image
final_image;


widgets::root
root;


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


int
index;


widgets::dial*
interval_dial;


widgets::label*
state_label;


uint32_t
interval_time = 1000;


uint32_t
last_time;


void
update_state_label() noexcept
{
    if(stack.empty())
    {
      state_label->set_text("  / 0");
    }

  else
    {
      string_form  sf;

      state_label->set_text(sf("%2d/%2d",index+1,stack.size()));
    }
}


class
view: public widget
{
public:
  view() noexcept{}

  void  advance() noexcept
  {
      if(++index >= stack.size())
      {
        index = 0;
      }


    update_state_label();

    need_to_redraw();
  }

  void  reform(gbstd::point  base_pt) noexcept override
  {
    widget::reform(base_pt);

    m_width  = std::max(cell::width,gbstd::font_width*5);
    m_height = (cell::height+font_height);
  }


  void  render(image_cursor  cur) noexcept override
  {
    cur.fill_rectangle(colors::black,0,0,cell::width,cell::height+font_height);
    cur.draw_rectangle(colors::white,0,0,cell::width,cell::height);

      if(index < stack.size())
      {
        auto&  img = stack[index]->image;

        images::paste(img,img.get_rectangle(),cur.get_image(),cur.get_offset());
      }
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
resize_cell_all(int  w, int  h) noexcept
{
  cell::width  = w;
  cell::height = h;

    for(auto&  cell: cells)
    {
      cell.image.resize(w,h);
    }


  cv_image.resize(w,h);

  cv->need_to_reform();


  mnu->set_item_size(w,h);

  mnu->need_to_reform();


  animator::view.need_to_reform();
}


void
save() noexcept
{
#ifdef EMSCRIPTEN
  root.redraw(final_image);

  sdl::update_screen(final_image);

  generate_saved_image_link(200,200);
#else
  final_image.save_to_webp("__test.webp");
#endif
}


void
main_loop() noexcept
{
  auto&  condev = sdl::update_control_device();

  animator::check_time(condev.time);

  root.react(condev);

    if(root.redraw_only_needed_widgets(final_image) || condev.needed_to_redraw)
    {
      sdl::update_screen(final_image);
    }
}


widget*
create_animation_widget() noexcept
{
  animator::interval_dial = new widgets::dial(1,4,[](widgets::dial&  d, int  old_value, int  new_value){
    static const uint32_t  table[] = {1000,600,200,80};

    animator::interval_time = table[new_value-1];
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

            animator::update_state_label();

            animator::view.need_to_redraw();
          }
      }
  });


  auto  op_col = new widgets::table_column({psh_btn,pop_btn});

  animator::state_label = new widgets::label(u"  / 0",styles::a_white_based_text_style);

  auto  speed_frame = new widgets::frame(animator::interval_dial,"speed");

  auto  frm_col = new widgets::table_column({&animator::view,animator::state_label});

  auto  urow = new widgets::table_row({frm_col,op_col});

  auto  frame = new widgets::frame(new widgets::table_column({urow,speed_frame}),"animation");

  return frame;
}


}


int
main(int  argc, char**  argv)
{
  set_caption("mkanigra - " __DATE__);
  set_description("マウスの左ボタンで、任意色を置き、右ボタンで透明色を置く");

  cv = new canvas(cv_image,[](canvas&  cv){
    cell_table::receive();

    mnu->need_to_redraw();
  });


  cv->set_grid();
  cv->set_pixel_size(10);


  widgets::menu_item_parameter  mip = {0,0,
    [](widgets::menu&  menu, point  index, mouse_button  left, mouse_button  right)
    {
        if(left)
        {
          auto  ptr = cell_table::get_pointer(index);

            if(ptr)
            {
              cell_table::current = ptr;

              cell_table::send();

              cv->get_drawing_recorder().clear();

              cv->need_to_redraw();

              menu.need_to_redraw();
            }
        }
    },

    [](widgets::menu&  menu, point  index, image_cursor  cur)
    {
      cur.fill_rectangle(colors::blue,0,0,cell::width,
                                          cell::height);

      auto  ptr = cell_table::get_pointer(index);

        if(ptr)
        {
          images::paste(ptr->image,ptr->image.get_rectangle(),cur);

            if(ptr == cell_table::current)
            {
              cur.draw_rectangle(colors::white,0,0,cell::width,
                                                   cell::height);
            }
        }
    }
  };


  mnu = new widgets::menu(mip,cell_table::width,cell_table::height);


  resize_cell_all(24,24);


  auto  color_maker_frame = new widgets::frame(cv->create_color_maker(),"color");

  auto      cv_tool = cv->create_tool_widget();
  auto        cv_op = cv->create_operation_widget();


  auto  cv_frame = new widgets::frame(cv,"canvas");

  cv_frame->set_line_color(colors::black);


  auto  save_btn = new widgets::button(new widgets::label(u"Save"),[](widgets::button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        save();
      }
  });

  auto  urow = new widgets::table_row({cv_tool,cv_op});
  auto  mrow = new widgets::table_row({color_maker_frame,create_animation_widget()});
  auto  ucol = new widgets::table_column({urow,mrow});


  auto  canvas_frame = new widgets::table_row({cv_frame,ucol});
  auto  celtbl_frame = new widgets::frame(mnu,"cell table");

  auto  lrow = new widgets::table_row({celtbl_frame,save_btn});

  root.set_node_target(new widgets::table_column({canvas_frame,lrow}));

  auto&  root_node = root.get_node();

  sdl::init(root_node.get_width(),root_node.get_height());

  final_image = sdl::make_screen_image();

  root.redraw(final_image);

  sdl::update_screen(final_image);


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




