#include"libgbscr/stmt.hpp"
#include"libgbstd/utility.hpp"


namespace gbscr{
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
      const char*  continue_label, types::switch_data&  swdat, context&  ctx, cursor&  cur,
      buffer_type&  buf, process&  proc) noexcept;


void
build_for(const char*  label_base,
      const char*     break_label,
      const char*  continue_label, types::switch_data&  swdat, context&  ctx, cursor&  cur,
      buffer_type&  buf, process&  proc) noexcept
{
  gbstd::tmpstr  co_label_base("FOR%03d",ctx.for_count++);
  gbstd::tmpstr    begin_label("%s_BEGIN"   ,*co_label_base);
  gbstd::tmpstr      end_label("%s_END"     ,*co_label_base);
  gbstd::tmpstr    conti_label("%s_CONTINUE",*co_label_base);


  cursor  para_cur(cur[0].get_block());

  expr  init_expr = make_expr(para_cur,proc);

    if(para_cur->is_punctuations(";"))
    {
      ++para_cur;
    }


  expr  cond_expr = make_expr(para_cur,proc);

    if(para_cur->is_punctuations(";"))
    {
      ++para_cur;
    }


  expr  mod_expr = make_expr(para_cur,proc);

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


  cursor  blk_cur(cur[1].get_block());

  build(*co_label_base,*end_label,*conti_label,swdat,ctx,blk_cur,buf,proc);


  buf.emplace_back(stmt_kind::label,*conti_label);

  buf.emplace_back(stmt_kind::evaluate_and_dump,expr(mod_expr));

  buf.emplace_back(stmt_kind::jump ,*begin_label);
  buf.emplace_back(stmt_kind::label,  *end_label);

  cur += 2;
}


void
build_while(const char*  label_base,
      const char*     break_label,
      const char*  continue_label, types::switch_data&  swdat, context&  ctx, cursor&  cur,
      buffer_type&  buf, process&  proc) noexcept
{
  gbstd::tmpstr  co_label_base("WHILE%03d",ctx.while_count++);
  gbstd::tmpstr    begin_label("%s_BEGIN" ,*co_label_base);
  gbstd::tmpstr      end_label("%s_END"   ,*co_label_base);


  buf.emplace_back(stmt_kind::label,*begin_label);


  cursor  expr_cur(cur[0].get_block());

  buf.emplace_back(stmt_kind::evaluate_and_zero,make_expr(expr_cur,proc));
  buf.emplace_back(stmt_kind::jump_by_condition,*end_label);


  cursor  blk_cur(cur[1].get_block());

  build(*co_label_base,*end_label,*begin_label,swdat,ctx,blk_cur,buf,proc);


  buf.emplace_back(stmt_kind::jump ,*begin_label);
  buf.emplace_back(stmt_kind::label,  *end_label);

  cur += 2;
}


void
build_if(const char*  label_base,
      const char*     break_label,
      const char*  continue_label, types::switch_data&  swdat, context&  ctx, cursor&  cur,
      buffer_type&  buf, process&  proc) noexcept
{
  int  block_number = 0;

  gbstd::tmpstr  co_label_base("IF%03d",ctx.if_count++);
  gbstd::tmpstr      end_label("%s_END",*co_label_base);
  gbstd::tmpstr     next_label("%s_%03d",*co_label_base,block_number++);


  cursor  expr_cur(cur[0].get_block());

  buf.emplace_back(stmt_kind::evaluate_and_zero,make_expr(expr_cur,proc));
  buf.emplace_back(stmt_kind::jump_by_condition,*next_label);


  cursor  blk_cur(cur[1].get_block());

  build(*co_label_base,break_label,continue_label,swdat,ctx,blk_cur,buf,proc);


  buf.emplace_back(stmt_kind::jump , *end_label);
  buf.emplace_back(stmt_kind::label,*next_label);

  cur += 2;

    while(cur[0].is_identifier("else"))
    {
      cur += 1;

        if(cur[0].is_block('{','}'))
        {
          blk_cur = cursor(cur[0].get_block());

          build(*co_label_base,break_label,continue_label,swdat,ctx,blk_cur,buf,proc);

          cur += 1;

          break;
        }

      else
        if(cur[0].is_identifier("if") &&
           cur[1].is_block('(',')') &&
           cur[2].is_block('{','}'))
        {
          next_label("%s_%03d",*co_label_base,block_number++);

          expr_cur = cursor(cur[1].get_block());

          buf.emplace_back(stmt_kind::evaluate_and_zero,make_expr(expr_cur,proc));
          buf.emplace_back(stmt_kind::jump_by_condition,*next_label);


          blk_cur = cursor(cur[2].get_block());

          build(*co_label_base,break_label,continue_label,swdat,ctx,blk_cur,buf,proc);

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
      const char*  continue_label, types::switch_data&  swdat, context&  ctx, cursor&  cur,
      buffer_type&  buf, process&  proc) noexcept
{
  types::switch_data  new_swdat;

  gbstd::tmpstr  co_label_base("SWITCH%03d",ctx.switch_count++);
  gbstd::tmpstr    begin_label("%s_BEGIN" ,*co_label_base);
  gbstd::tmpstr      end_label("%s_END"   ,*co_label_base);

  new_swdat.label_base = *co_label_base;

  buf.emplace_back(stmt_kind::label,*begin_label);


  cursor  expr_cur(cur[0].get_block());
  cursor   blk_cur(cur[1].get_block());


  buffer_type  tmp_buf;

  build(*co_label_base,*end_label,*begin_label,new_swdat,ctx,blk_cur,tmp_buf,proc);


  buf.emplace_back(stmt_kind::evaluate_and_save,make_expr(expr_cur,proc));

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
      const char*  continue_label, types::switch_data&  swdat, context&  ctx, cursor&  cur,
      buffer_type&  buf, process&  proc) noexcept
{
    while(cur)
    {
        if(cur[0].is_identifier())
        {
          using  sv = gbstd::string_view;

          sv  id(cur[0].get_string().view());

            if(id == sv("return"))
            {
              ++cur;

              buf.emplace_back(stmt_kind::return_,make_expr(cur,proc));
            }

          else
            if(id == sv("sleep"))
            {
              ++cur;

              buf.emplace_back(stmt_kind::sleep,make_expr(cur,proc));
            }

          else
            if(id == sv("print"))
            {
              ++cur;

              buf.emplace_back(stmt_kind::print,make_expr(cur,proc));
            }

          else
            if(id == sv("while"))
            {
              ++cur;

                if(cur[0].is_block('(',')') &&
                   cur[1].is_block('{','}'))
                {
                  build_while(label_base,break_label,continue_label,swdat,ctx,cur,buf,proc);
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

                if(cur[0].is_block('(',')') &&
                   cur[1].is_block('{','}'))
                {
                  build_if(label_base,break_label,continue_label,swdat,ctx,cur,buf,proc);
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

                if(cur[0].is_block('(',')') &&
                   cur[1].is_block('{','}'))
                {
                  build_for(label_base,break_label,continue_label,swdat,ctx,cur,buf,proc);
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

                if(cur[0].is_block('(',')') &&
                   cur[1].is_block('{','}'))
                {
                  build_switch(label_base,break_label,continue_label,swdat,ctx,cur,buf,proc);
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

                if(cur->is_block('(',')'))
                {
                  gbstd::tmpstr  label("%s_CASE%03d",swdat.label_base,swdat.case_exprs.size());


                  cursor  expr_cur(cur->get_block());

                  auto  e = make_expr(expr_cur,proc);

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


              buf.emplace_back(stmt_kind::jump,cur->get_string().view());

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


              buf.emplace_back(stmt_kind::label,cur->get_string().view());

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


              buf.emplace_back(stmt_kind::jump,cur->get_string().view());

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
              auto  e = make_expr(cur,proc);

                if(e)
                {
                  buf.emplace_back(stmt_kind::evaluate_and_dump,std::move(e));
                }
            }
        }

      else
        if(cur[0].is_punctuations(";"))
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
stmt_list(const block&  tokproc, process&  proc) noexcept
{
  buffer_type  buf;

  cursor  cur(tokproc);

  context  ctx = {0};

  types::switch_data  swdat;

  build("","","",swdat,ctx,cur,buf,proc);


  assign(buf.data(),buf.size());
}




}}




