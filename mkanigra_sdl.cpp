#include"libgbgui/canvas.hpp"
#include"libgbgui/preset.hpp"
#include"sdl.hpp"


#ifdef EMSCRIPTEN
#include<emscripten.h>
#endif


using namespace gbstd;
using namespace gbgui;


namespace{


images::image
final_image;


widgets::root
root;


constexpr int  g_cell_size = 48;


presets::graphics_editor*
ge;


namespace underlay{


widgets::frame*
g_widget_frame;


widgets::label*
g_counter_label;


std::vector<point>
g_point_stack;




void
onpush(widgets::button&  btn) noexcept
{
    if(btn.is_released())
    {
      g_point_stack.emplace_back(ge->m_canvas->get_cursor_offset());

      string_form  sf;

      g_counter_label->set_text(sf("%2d",g_point_stack.size()));


      ge->m_canvas->need_to_redraw();
    }
}


void
onpop(widgets::button&  btn) noexcept
{
    if(btn.is_released())
    {
        if(g_point_stack.size())
        {
          g_point_stack.pop_back();

          string_form  sf;

          g_counter_label->set_text(sf("%2d",g_point_stack.size()));


          ge->m_canvas->need_to_redraw();
        }
    }
}


void
build() noexcept
{
  auto  psh_btn = new widgets::button(new widgets::label(u"push"),onpush);
  auto  pop_btn = new widgets::button(new widgets::label(u"pop"),onpop);

  g_counter_label = new widgets::label(u" 0",styles::a_white_based_text_style);

  auto  tbl = new widgets::table_column({g_counter_label,psh_btn,pop_btn});

  g_widget_frame = new widgets::frame(tbl,"underlay");
}


}


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

    m_width  = ge->m_cell_width ;
    m_height = ge->m_cell_height;
  }


  void  render(image_cursor  cur) noexcept override
  {
    widget::render_background(cur);

      if(index < stack.size())
      {
          for(auto&  pt: underlay::g_point_stack)
          {
            images::overlay(ge->m_source_image,rectangle(pt,g_cell_size,g_cell_size),cur);
          }


        images::overlay(ge->m_source_image,ge->get_rect(stack[index]),cur);
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
main_loop() noexcept
{
  auto&  condev = sdl::update_control_device();

  auto&  cont = condev.dropped_file_content;

    if(cont.size())
    {
      ge->load(cont);

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

        animator::stack.emplace_back(ge->m_current_index);

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
#ifdef EMSCRIPTEN
  set_caption("mkanigra - " __DATE__);
  set_description("<pre>"
                  "*マウスの左ボタンで、任意色を置き、\n"
                  " 右ボタンで透明色を置く\n"
                  "*画像の編集単位をセルと呼ぶ\n"
                  "*PNGファイルをドラッグ・アンド・ドロップで読み込む\n"
                  "<tool>\n"
                  "  *selectはundoを除くoperationの適用範囲を、変更する\n"
                  "   開始時は全体が範囲\n"
                  "  *select時、右クリックで範囲を全体に戻す\n"
                  "  *pasteは保持されたセルを透明色も含めて、貼り付ける\n"
                  "  *layerは保持されたセルを透明色を除いて、貼り付ける\n"
                  "<operation>\n"
                  "  *undoは最後の編集操作を取り消す。セルを移ると記録はクリアされる\n"
                  "  *copyは現在のセルを複製して、保持する\n"
                  "<animation>\n"
                  "  *pushはアニメーションの最後尾に、現在のセルを付け加える\n"
                  "  *popはアニメーションの最後尾から、ひとつ取り除く\n"
                  "<underlay>\n"
                  "  *pushは下敷きの最上段に、現在のセルを付け加える\n"
                  "  *popは下敷きの最上段から、ひとつ取り除く\n"
                  "</pre>"
  );
#endif

  ge = presets::create_graphics_editor(g_cell_size,g_cell_size,6,2);

  auto  ani = create_animation_widget();
  

  underlay::build();

  ge->m_canvas->set_underlay_point_list(&underlay::g_point_stack);

  ge->m_callback = [](){animator::view.need_to_redraw();};

  auto  coloring_widget = new widgets::table_column({ge->m_color_maker_frame,ge->m_bg_change_buttons,ani});

  auto  to_col = new widgets::table_column({ge->m_tool_widget_frame,ge->m_operation_widget_frame});

  auto  last_w = new widgets::table_row({ge->m_cell_table_frame,underlay::g_widget_frame});

  auto  right_upper = new widgets::table_row({ge->m_color_holder_frame,coloring_widget,to_col});
  auto  right       = new widgets::table_column({right_upper,last_w});


  auto  left = new widgets::table_column({ge->m_canvas_frame,ge->m_save_button});

  root.set_node_target(new widgets::table_row({left,right}));

  auto&  root_node = root.get_node();


  sdl::init(root_node.get_width(),root_node.get_height());

  final_image = sdl::make_screen_image();
  animator::view.set_style(ge->m_bg_style);

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




