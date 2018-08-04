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
std::vector<point>
g_point_stack;
}


namespace animator{


int
index;


widgets::dial*
interval_dial;


widgets::label*
state_label;


uint32_t
last_time;


void
update_state_label() noexcept
{
    if(ge->m_animation_points.empty())
    {
      state_label->set_text("  / 0");
    }

  else
    {
      string_form  sf;

      state_label->set_text(sf("%2d/%2d",index+1,ge->m_animation_points.size()));
    }
}


class
view: public widget
{
public:
  view() noexcept{}

  void  advance() noexcept
  {
      if(++index >= ge->m_animation_points.size())
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

      if(ge->m_canvas->get_underlay_point_list())
      {
          for(auto&  pt: underlay::g_point_stack)
          {
            images::overlay(ge->m_source_image,rectangle(pt,g_cell_size,g_cell_size),cur);
          }
      }


      if(index < ge->m_animation_points.size())
      {
        images::overlay(ge->m_source_image,ge->get_rect(ge->m_animation_points[index]),cur);
      }
  }

};


animator::view
view;




void
check_time(uint32_t  now) noexcept
{
    if(ge->m_animation_points.size())
    {
        if(now >= (last_time+ge->m_animation_delay))
        {
          last_time = now;

          view.advance();
        }
    }
}


}


namespace underlay{


widgets::frame*
g_widget_frame;


widgets::label*
g_counter_label;


widgets::label*
g_switching_label;




void
onpush(widgets::button&  btn) noexcept
{
    if(btn.is_released() && btn.get_count())
    {
      btn.reset_count();

      g_point_stack.emplace_back(ge->m_canvas->get_cursor_offset());

      string_form  sf;

      g_counter_label->set_text(sf("%2d",g_point_stack.size()));


      ge->m_canvas->need_to_redraw();
      animator::view.need_to_redraw();
    }
}


void
onpop(widgets::button&  btn) noexcept
{
    if(btn.is_released() && btn.get_count())
    {
      btn.reset_count();

        if(g_point_stack.size())
        {
          g_point_stack.pop_back();

          string_form  sf;

          g_counter_label->set_text(sf("%2d",g_point_stack.size()));


          ge->m_canvas->need_to_redraw();
          animator::view.need_to_redraw();
        }
    }
}


void
onswitch(widgets::button&  btn) noexcept
{
    if(btn.is_released() && btn.get_count())
    {
      btn.reset_count();

        if(ge->m_canvas->get_underlay_point_list())
        {
          ge->m_canvas->set_underlay_point_list(nullptr);

          g_switching_label->modify_text(u"show");
        }

      else
        {
          ge->m_canvas->set_underlay_point_list(&g_point_stack);

          g_switching_label->modify_text(u"hide");
        }


      ge->m_canvas->need_to_redraw();
      animator::view.need_to_redraw();
    }
}


void
build() noexcept
{
  g_switching_label = new widgets::label(u"hide");


  auto  psh_btn = new widgets::button(new widgets::label(u"push"),onpush);
  auto  pop_btn = new widgets::button(new widgets::label(u"pop"),onpop);
  auto  swi_btn = new widgets::button(g_switching_label,onswitch);

  g_counter_label = new widgets::label(u" 0",styles::a_white_based_text_style);

  auto  tbl = new widgets::table_column({g_counter_label,psh_btn,pop_btn,swi_btn});

  g_widget_frame = new widgets::frame(tbl,"underlay");
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

    ge->m_animation_delay = table[new_value-1];
  });

  auto  psh_btn = new widgets::button(new widgets::label(u"Push"),[](widgets::button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

        ge->m_animation_points.emplace_back(ge->m_current_index);

        animator::view.need_to_redraw();
      }
  });

  auto  pop_btn = new widgets::button(new widgets::label(u"Pop"),[](widgets::button&  btn){
      if(btn.get_count())
      {
        btn.reset_count();

          if(ge->m_animation_points.size())
          {
            ge->m_animation_points.pop_back();

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
                  "◇tool\n"
                  "  *fill areaは指定地点と同色の領域を新たな色で塗りつぶす\n"
                  "  *selectはundoを除くoperationの有効範囲を変更する\n"
                  "   開始時は全体が範囲\n"
                  "  *select時、右クリックで範囲を全体に戻す\n"
                  "  *pasteは保持されたセルを透明色も含めて貼り付ける\n"
                  "  *layerは保持されたセルを透明色を除いて貼り付ける\n"
                  "◇operation\n"
                  "  *undoは最後の編集操作を取り消す。セルを移ると編集操作記録はクリアされる\n"
                  "  *copyは現在のセルから指定範囲を複製して保持する\n"
                  "◇animation\n"
                  "  *pushはアニメーションの最後尾に現在のセルを付け加える\n"
                  "  *popはアニメーションの最後尾から、ひとつ取り除く\n"
                  "◇underlay\n"
                  "  *pushは下敷きの最上段に現在のセルを乗せる\n"
                  "  *popは下敷きの最上段から、ひとつ取り除く\n"
                  "  *show/hideは下敷きの有効/無効を切り替える\n"
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




