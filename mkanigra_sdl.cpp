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


images::image
source_image;


widgets::root
root;


widgets::color_maker*
colmak;


widgets::color_holder*
colhol;


background_style
bg_style(color(0,0,4),color(0,0,6),4);


int  cell_width  = 16;
int  cell_height = 16;


rectangle
get_rect(point  index) noexcept
{
   return rectangle(cell_width *index.x,
                    cell_height*index.y,
                    cell_width,
                    cell_height);
}


constexpr int  table_width  = 20;
constexpr int  table_height =  2;


images::point  g_current_index;


namespace animator{


std::vector<images::point>
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

    m_width  = cell_width ;
    m_height = cell_height;
  }


  void  render(image_cursor  cur) noexcept override
  {
    widget::render_background(cur);

      if(index < stack.size())
      {
        images::overlay(source_image,get_rect(stack[index]),cur);
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
  cell_width  = w;
  cell_height = h;

  cv->set_cursor_offset(0,0);
  cv->set_editing_size(w,h);

  source_image.resize(cell_width*table_width,cell_height*table_height);

  mnu->set_item_size(w,h);


  mnu->need_to_reform();

  animator::view.need_to_reform();
}


void
save() noexcept
{
#ifdef EMSCRIPTEN
  sdl::update_screen(source_image);

  generate_saved_image_link(source_image.get_width(),source_image.get_height());
#else
  source_image.save_to_png("__anigra.png");
#endif
}


void
main_loop() noexcept
{
  auto&  condev = sdl::update_control_device();

    if(condev.dropped_file_content.size())
    {
      auto&  cont = condev.dropped_file_content;

        if(is_png(cont.data()))
        {
          source_image.load_from_png(cont.data(),cont.size());

           cv->need_to_redraw();
          mnu->need_to_redraw();
        }

      else
        if(is_webp(cont.data()))
        {
          source_image.load_from_webp(cont.data(),cont.size());

           cv->need_to_redraw();
          mnu->need_to_redraw();
        }


      cont.resize(0);
    }


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

        animator::stack.emplace_back(g_current_index);

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

  cv = new canvas(source_image,cell_width,cell_height,[](canvas&  cv){
    mnu->need_to_redraw();
  });


  cv->set_grid();
  cv->set_pixel_size(10);


  widgets::menu_item_parameter  mip = {0,0,
    [](widgets::menu&  menu, point  index, mouse_button  left, mouse_button  right)
    {
        if(left)
        {
          g_current_index = index;

          cv->set_cursor_offset(cell_width*index.x,cell_height*index.y);

          menu.need_to_redraw();
        }
    },

    [](widgets::menu&  menu, point  index, image_cursor  cur)
    {
      auto  src_rect = get_rect(index);

      images::overlay(source_image,src_rect,cur);

        if(index == g_current_index)
        {
          cur.draw_rectangle(colors::white,0,0,cell_width,
                                               cell_height);
        }
    }
  };


  mnu = new widgets::menu(mip,table_width,table_height);


  resize_cell_all(24,24*1);

  auto  color_list = {
    colors::black,
    colors::dark_gray,
    colors::gray,
    colors::light_gray,
    colors::white,
    colors::red,
    colors::green,
    colors::blue,
    colors::yellow,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
    colors::black,
  };


  colhol = new widgets::color_holder(color_list,[](widgets::color_holder&  holder, colors::color  color){
    colmak->set_color(color);

    cv->set_drawing_color(color);
  });


  colmak = new widgets::color_maker([](widgets::color_maker&  maker, colors::color  color){
    colhol->set_color(color);

    cv->set_drawing_color(color);
  });


  auto  color_maker_frame  = new widgets::frame(colmak,"color");
  auto  color_holder_frame = new widgets::frame(colhol,"palette");


  auto  cv_tool = cv->create_tool_widget();
  auto    cv_op = cv->create_operation_widget();


  auto  cv_frame = new widgets::frame(cv,"canvas");

  cv_frame->set_line_color(colors::black);


  auto  save_btn = new widgets::button(new widgets::label(u"Save"),[](widgets::button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        save();
      }
  });

  auto  ch1bg_btn = new widgets::button(new widgets::label(u"Change 1st bg color"),[](widgets::button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        bg_style.set_first_color(colmak->get_color());

        cv->need_to_redraw();
        mnu->need_to_redraw();
        animator::view.need_to_redraw();
      }
  });

  auto  ch2bg_btn = new widgets::button(new widgets::label(u"Change 2nd bg color"),[](widgets::button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        bg_style.set_second_color(colmak->get_color());

        cv->need_to_redraw();
        mnu->need_to_redraw();
        animator::view.need_to_redraw();
      }
  });


  auto  urow = new widgets::table_row({cv_tool,cv_op});
  auto  mrow = new widgets::table_row({color_maker_frame,create_animation_widget()});
  auto  ucol = new widgets::table_column({urow,mrow});


  auto  canvas_frame = new widgets::table_row({cv_frame,color_holder_frame,ucol});
  auto  celtbl_frame = new widgets::frame(mnu,"cell table");

  auto  lrow = new widgets::table_row({celtbl_frame,new widgets::table_column({ch1bg_btn,ch2bg_btn,save_btn})});

  root.set_node_target(new widgets::table_column({canvas_frame,lrow}));

  auto&  root_node = root.get_node();

  sdl::init(root_node.get_width(),root_node.get_height());

  final_image = sdl::make_screen_image();

  cv->set_style(bg_style);
  mnu->set_style(bg_style);
  animator::view.set_style(bg_style);

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




