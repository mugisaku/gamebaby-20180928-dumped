#include"libgbsnd/stmt.hpp"
#include"libgbsnd/script.hpp"
#include"libgbstd/utility.hpp"


namespace gbsnd{
namespace stmts{




namespace{


struct
context
{
  int      if_count;
  int     for_count;
  int   while_count;
  int  switch_count;

};


using buffer_type = std::vector<stmt>;


namespace types{
struct
switch_data
{
  const char*  label_base="";

  std::vector<expr>  case_exprs;

  int  number_of_defaults=0;

};
}


void
build(const char*  label_base,
      const char*     break_label,
      const char*  continue_label, types::switch_data&  swdat, context&  ctx, script_token_cursor&  cur,
      buffer_type&  buf) noexcept;


void
build_for(const char*  label_base,
      const char*     break_label,
      const char*  continue_label, types::switch_data&  swdat, context&  ctx, script_token_cursor&  cur,
      buffer_type&  buf) noexcept
{
  gbstd::tmpstr  co_label_base("FOR%03d",ctx.for_count++);
  gbstd::tmpstr    begin_label("%s_BEGIN"   ,*co_label_base);
  gbstd::tmpstr      end_label("%s_END"     ,*co_label_base);
  gbstd::tmpstr    conti_label("%s_CONTINUE",*co_label_base);


  script_token_cursor  para_cur(cur[0].get_token_string());

  expr  init_expr = make_expr(para_cur);
  expr  cond_expr = make_expr(para_cur);
  expr   mod_expr = make_expr(para_cur);

    if(init_expr)
    {
      buf.emplace_back(stmt_kind::evaluate_and_dump,expr(init_expr));
    }


  buf.emplace_back(stmt_kind::label,*begin_label);

    if(cond_expr)
    {
      buf.emplace_back(stmt_kind::evaluate_and_zero,expr(cond_expr));
      buf.emplace_back(stmt_kind::jump_by_condition,*end_label);
    }


  script_token_cursor  blk_cur(cur[1].get_token_string());

  build(*co_label_base,*end_label,*conti_label,swdat,ctx,blk_cur,buf);


  buf.emplace_back(stmt_kind::label,*conti_label);

  buf.emplace_back(stmt_kind::evaluate_and_dump,expr(mod_expr));

  buf.emplace_back(stmt_kind::jump ,*begin_label);
  buf.emplace_back(stmt_kind::label,  *end_label);

  cur += 2;
}


void
build_while(const char*  label_base,
      const char*     break_label,
      const char*  continue_label, types::switch_data&  swdat, context&  ctx, script_token_cursor&  cur,
      buffer_type&  buf) noexcept
{
  gbstd::tmpstr  co_label_base("WHILE%03d",ctx.while_count++);
  gbstd::tmpstr    begin_label("%s_BEGIN" ,*co_label_base);
  gbstd::tmpstr      end_label("%s_END"   ,*co_label_base);


  buf.emplace_back(stmt_kind::label,*begin_label);


  script_token_cursor  expr_cur(cur[0].get_token_string());

  buf.emplace_back(stmt_kind::evaluate_and_zero,make_expr(expr_cur));
  buf.emplace_back(stmt_kind::jump_by_condition,*end_label);


  script_token_cursor  blk_cur(cur[1].get_token_string());

  build(*co_label_base,*end_label,*begin_label,swdat,ctx,blk_cur,buf);


  buf.emplace_back(stmt_kind::jump ,*begin_label);
  buf.emplace_back(stmt_kind::label,  *end_label);

  cur += 2;
}


void
build_if(const char*  label_base,
      const char*     break_label,
      const char*  continue_label, types::switch_data&  swdat, context&  ctx, script_token_cursor&  cur,
      buffer_type&  buf) noexcept
{
  int  block_number = 0;

  gbstd::tmpstr  co_label_base("IF%03d",ctx.if_count++);
  gbstd::tmpstr      end_label("%s_END",*co_label_base);
  gbstd::tmpstr     next_label("%s_%03d",*co_label_base,block_number++);


  script_token_cursor  expr_cur(cur[0].get_token_string());

  buf.emplace_back(stmt_kind::evaluate_and_zero,make_expr(expr_cur));
  buf.emplace_back(stmt_kind::jump_by_condition,*next_label);


  script_token_cursor  blk_cur(cur[1].get_token_string());

  build(*co_label_base,break_label,continue_label,swdat,ctx,blk_cur,buf);


  buf.emplace_back(stmt_kind::jump , *end_label);
  buf.emplace_back(stmt_kind::label,*next_label);

  cur += 2;

    while(cur[0].is_identifier({gbstd::string_view("else")}))
    {
      cur += 1;

        if(cur[0].is_token_string('{','}'))
        {
          blk_cur = script_token_cursor(cur[0].get_token_string());

          build(*co_label_base,break_label,continue_label,swdat,ctx,blk_cur,buf);

          cur += 1;

          break;
        }

      else
        if(cur[0].is_identifier({gbstd::string_view("if")}) &&
           cur[1].is_token_string('(',')') &&
           cur[2].is_token_string('{','}'))
        {
          next_label("%s_%03d",*co_label_base,block_number++);

          expr_cur = script_token_cursor(cur[1].get_token_string());

          buf.emplace_back(stmt_kind::evaluate_and_zero,make_expr(expr_cur));
          buf.emplace_back(stmt_kind::jump_by_condition,*next_label);


          blk_cur = script_token_cursor(cur[2].get_token_string());

          build(*co_label_base,break_label,continue_label,swdat,ctx,blk_cur,buf);

          buf.emplace_back(stmt_kind::jump , *end_label);
          buf.emplace_back(stmt_kind::label,*next_label);

          cur += 3;
        }
    }


  buf.emplace_back(stmt_kind::label,*end_label);
}


void
build_switch(const char*  label_base,
      const char*     break_label,
      const char*  continue_label, types::switch_data&  swdat, context&  ctx, script_token_cursor&  cur,
      buffer_type&  buf) noexcept
{
  types::switch_data  new_swdat;

  gbstd::tmpstr  co_label_base("SWITCH%03d",ctx.switch_count++);
  gbstd::tmpstr    begin_label("%s_BEGIN" ,*co_label_base);
  gbstd::tmpstr      end_label("%s_END"   ,*co_label_base);

  new_swdat.label_base = *co_label_base;

  buf.emplace_back(stmt_kind::label,*begin_label);


  script_token_cursor  expr_cur(cur[0].get_token_string());
  script_token_cursor   blk_cur(cur[1].get_token_string());


  buffer_type  tmp_buf;

  build(*co_label_base,*end_label,*begin_label,new_swdat,ctx,blk_cur,tmp_buf);


  buf.emplace_back(stmt_kind::evaluate_and_save,make_expr(expr_cur));

    for(int  i = 0;  i < new_swdat.case_exprs.size();  ++i)
    {
      gbstd::tmpstr  dst_label("%s_CASE%03d",*co_label_base,i);

      buf.emplace_back(stmt_kind::evaluate_and_equal,expr(new_swdat.case_exprs[i]));

      buf.emplace_back(stmt_kind::jump_by_condition,*dst_label);
    }


    if(new_swdat.number_of_defaults)
    {
      gbstd::tmpstr  dst_label("%s_DEFAULT",*co_label_base);

      buf.emplace_back(stmt_kind::jump,*dst_label);
    }


  buf.emplace_back(stmt_kind::jump,*end_label);

    for(auto&&  stmt: tmp_buf)
    {
      buf.emplace_back(std::move(stmt));
    }


  buf.emplace_back(stmt_kind::label,*end_label);

  cur += 2;
}


void
build(const char*  label_base,
      const char*     break_label,
      const char*  continue_label, types::switch_data&  swdat, context&  ctx, script_token_cursor&  cur,
      buffer_type&  buf) noexcept
{
    while(cur)
    {
        if(cur[0].is_identifier())
        {
          using  sv = gbstd::string_view;

          sv  id(cur[0].get_identifier().view());

            if(id == sv("return"))
            {
              ++cur;

              buf.emplace_back(stmt_kind::return_,make_expr(cur));
            }

          else
            if(id == sv("sleep"))
            {
              ++cur;

              buf.emplace_back(stmt_kind::sleep,make_expr(cur));
            }

          else
            if(id == sv("print"))
            {
              ++cur;

              buf.emplace_back(stmt_kind::print,make_expr(cur));
            }

          else
            if(id == sv("while"))
            {
              ++cur;

                if(cur[0].is_token_string('(',')') &&
                   cur[1].is_token_string('{','}'))
                {
                  build_while(label_base,break_label,continue_label,swdat,ctx,cur,buf);
                }

              else
                {
                  printf("while構文が不正\n");
                }
            }

          else
            if(id == sv("if"))
            {
              ++cur;

                if(cur[0].is_token_string('(',')') &&
                   cur[1].is_token_string('{','}'))
                {
                  build_if(label_base,break_label,continue_label,swdat,ctx,cur,buf);
                }

              else
                {
                  printf("if構文が不正\n");
                }
            }

          else
            if(id == sv("for"))
            {
              ++cur;

                if(cur[0].is_token_string('(',')') &&
                   cur[1].is_token_string('{','}'))
                {
                  build_for(label_base,break_label,continue_label,swdat,ctx,cur,buf);
                }

              else
                {
                  printf("for構文が不正\n");
                }
            }

          else
            if(id == sv("switch"))
            {
              ++cur;

                if(cur[0].is_token_string('(',')') &&
                   cur[1].is_token_string('{','}'))
                {
                  build_switch(label_base,break_label,continue_label,swdat,ctx,cur,buf);
                }

              else
                {
                  printf("switch構文が不正\n");
                }
            }

          else
            if(id == sv("case"))
            {
              ++cur;

                if(cur->is_token_string('(',')'))
                {
                  gbstd::tmpstr  label("%s_CASE%03d",swdat.label_base,swdat.case_exprs.size());


                  script_token_cursor  expr_cur(cur->get_token_string());

                  auto  e = make_expr(expr_cur);

                    if(!e)
                    {
                      printf("caseの後に式がない\n");
                    }


                  swdat.case_exprs.emplace_back(std::move(e));


                  buf.emplace_back(stmt_kind::label,*label);


                  ++cur;
                }

              else
                {
                  printf("case文に式が付いていない\n");
                }
            }

          else
            if(id == sv("default"))
            {
              ++cur;

                if(!swdat.number_of_defaults)
                {
                  swdat.number_of_defaults = 1;

                  gbstd::tmpstr  label("%s_DEFAULT",swdat.label_base);

                  buf.emplace_back(stmt_kind::label,*label);
                }

              else
                {
                  printf("switch文内のdefault文が唯一ではない\n");
                }
            }

          else
            if(id == sv("goto"))
            {
              ++cur;

                if(!cur[0].is_identifier())
                {
                  printf("gotoのための識別子がない\n");

                  break;
                }


              buf.emplace_back(stmt_kind::jump,cur->get_identifier().view());

              ++cur;
            }

          else
            if(id == sv("label"))
            {
              ++cur;

                if(!cur->is_identifier())
                {
                  printf("labelの後に識別子がない\n");

                  break;
                }


              buf.emplace_back(stmt_kind::label,cur->get_identifier().view());

              ++cur;
            }

          else
            if(id == sv("goto"))
            {
              ++cur;

                if(!cur->is_identifier())
                {
                  printf("gotoの後に識別子がない\n");

                  break;
                }


              buf.emplace_back(stmt_kind::jump,cur->get_identifier().view());

              ++cur;
            }

          else
            if(id == sv("break"))
            {
              ++cur;

              buf.emplace_back(stmt_kind::jump,break_label);
            }

          else
            if(id == sv("continue"))
            {
              ++cur;

              buf.emplace_back(stmt_kind::jump,continue_label);
            }

          else
            {
              auto  e = make_expr(cur);

                if(e)
                {
                  buf.emplace_back(stmt_kind::evaluate_and_dump,std::move(e));
                }
            }
        }

      else
        if(cur[0].is_semicolon())
        {
          ++cur;
        }

      else
        {
          printf("**error**\n");

          break;
        }
    }
}
}


stmt_list::
stmt_list(const script_token_string&  toks) noexcept
{
  buffer_type  buf;

  script_token_cursor  cur(toks);

  context  ctx = {0};

  types::switch_data  swdat;

  build("","","",swdat,ctx,cur,buf);


  assign(buf.data(),buf.size());
}




}}




