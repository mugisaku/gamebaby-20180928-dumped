#include"libgbscr/expr.hpp"
#include"libgbscr/stream.hpp"
#include"libgbscr/stmt.hpp"


namespace gbscr{
namespace exprs{




namespace{


class
last
{
  enum class kind{
    null,
    operator_,
    operand,
  } m_kind=kind::null;

public:
  bool  is_operator() const noexcept{return m_kind == kind::operator_;}
  bool  is_operand()  const noexcept{return m_kind == kind::operand;}

  void  be_operator() noexcept{m_kind = kind::operator_;}
  void  be_operand()  noexcept{m_kind = kind::operand;}
  void  be_null()     noexcept{m_kind = kind::null;}

};


class
operator_egg
{
  enum class kind{
    prefix_unary,
    postfix_unary,
    binary,
  } m_kind;


  operator_word  m_word;

  int  m_precedence;

  bool  m_right_to_left;

public:
  constexpr operator_egg(prefix_unary_operator  op, int  pred, bool  r2l=false) noexcept:
  m_kind(kind::prefix_unary),
  m_word(op.word),
  m_precedence(0xFF-pred),
  m_right_to_left(r2l){}


  constexpr operator_egg(postfix_unary_operator  op, int  pred, bool  r2l=false) noexcept:
  m_kind(kind::postfix_unary),
  m_word(op.word),
  m_precedence(0xFF-pred),
  m_right_to_left(r2l){}


  constexpr operator_egg(binary_operator  op, int  pred, bool  r2l=false) noexcept:
  m_kind(kind::binary),
  m_word(op.word),
  m_precedence(0xFF-pred),
  m_right_to_left(r2l){}


  constexpr bool  is_prefix_unary()  const noexcept{return m_kind == kind::prefix_unary;}
  constexpr bool  is_postfix_unary() const noexcept{return m_kind == kind::postfix_unary;}
  constexpr bool  is_binary()        const noexcept{return m_kind == kind::binary;}

  constexpr int  precedence() const noexcept{return m_precedence;}

  constexpr bool  operator==(const operator_egg&  rhs) const noexcept{return m_precedence == rhs.m_precedence;}
  constexpr bool  operator< (const operator_egg&  rhs) const noexcept{return m_precedence <  rhs.m_precedence;}

  constexpr bool  is_right_to_left() const noexcept{return m_right_to_left;}

  expr_element  make_element() const noexcept
  {
    return is_prefix_unary() ? expr_element( prefix_unary_operator{m_word})
          :is_postfix_unary()? expr_element(postfix_unary_operator{m_word})
          :                    expr_element(       binary_operator{m_word});
  }

};


class
maker
{
  last  m_last;

  std::vector<expr_element>   m_element_stack;
  std::vector<operator_egg>  m_operator_stack;

  void  transfer_operator() noexcept
  {
    m_element_stack.emplace_back(m_operator_stack.back().make_element());

    m_operator_stack.pop_back();
  }


public:
  void  clear() noexcept
  {
    m_element_stack.clear();
    m_operator_stack.clear();

    m_last.be_null();
  }


  const last&  get_last() const noexcept{return m_last;}

  void  push(operand&&  o) noexcept
  {
    m_element_stack.emplace_back(std::move(o));

    m_last.be_operand();
  }

  void  push(operator_egg  opeg) noexcept
  {
      while(m_operator_stack.size())
      {
        auto&  bk = m_operator_stack.back();

          if(bk == opeg)
          {
              if(opeg.is_right_to_left())
              {
                break;
              }
          }

        else
          if(bk < opeg)
          {
            break;
          }


        transfer_operator();
      }


    m_operator_stack.emplace_back(opeg);

      if(opeg.is_binary())
      {
        m_last.be_operator();
      }
  }


  expr  output() noexcept
  {
      while(m_operator_stack.size())
      {
        transfer_operator();
      }


    expr  e(m_element_stack.data(),m_element_stack.size());

    return std::move(e);
  }

};


enum class
result
{
  got_end,
  got_semicolon,
  got_colon,
  got_comma,
  got_error,

};


result
read(cursor&  cur, maker&  mk, table&  tbl) noexcept
{
  using  oe = operator_egg;

  using  preunop = prefix_unary_operator;
  using  posunop = postfix_unary_operator;
  using    binop = binary_operator;

    while(cur)
    {
      auto&  tok = *cur;

      operand  o;

        if(read_operand(cur,o,tbl))
        {
          mk.push(std::move(o));
        }

      else
        if(tok.is_punctuations())
        {
          operator_word  opw(tok.get_string().view());

            if(opw == operator_word("++"))
            {
              mk.push(oe(preunop{opw},0));
            }

          else
            if(opw == operator_word("--"))
            {
              mk.push(oe(preunop{opw},0));
            }

          else
            if(opw == operator_word("-"))
            {
              mk.push(mk.get_last().is_operand()? oe(  binop{opw},5     )
                                                : oe(preunop{opw},2,true));
            }

          else
            if(opw == operator_word("*"))
            {
              mk.push(mk.get_last().is_operand()? oe(  binop{opw},4     )
                                                : oe(preunop{opw},2,true));
            }

          else
            if(opw == operator_word("&"))
            {
              mk.push(mk.get_last().is_operand()? oe(  binop{opw},9     )
                                                : oe(preunop{opw},2,true));
            }

          else if(opw == operator_word("!")){mk.push(oe(preunop{opw},2,true));}
          else if(opw == operator_word("~")){mk.push(oe(preunop{opw},2,true));}

          else if(opw == operator_word( "+")){mk.push(oe(binop{opw},5));}
          else if(opw == operator_word( "/")){mk.push(oe(binop{opw},4));}
          else if(opw == operator_word( "%")){mk.push(oe(binop{opw},4));}
          else if(opw == operator_word("<<")){mk.push(oe(binop{opw},6));}
          else if(opw == operator_word(">>")){mk.push(oe(binop{opw},6));}

          else if(opw == operator_word(  "|")){mk.push(oe(binop{opw},13));}
          else if(opw == operator_word(  "^")){mk.push(oe(binop{opw},10));}
          else if(opw == operator_word( "||")){mk.push(oe(binop{opw},11));}
          else if(opw == operator_word( "&&")){mk.push(oe(binop{opw},12));}

          else if(opw == operator_word( "==")){mk.push(oe(binop{opw},8));}
          else if(opw == operator_word( "!=")){mk.push(oe(binop{opw},8));}
          else if(opw == operator_word(  "<")){mk.push(oe(binop{opw},7));}
          else if(opw == operator_word( "<=")){mk.push(oe(binop{opw},7));}
          else if(opw == operator_word(  ">")){mk.push(oe(binop{opw},7));}
          else if(opw == operator_word( ">=")){mk.push(oe(binop{opw},7));}

          else if(opw == operator_word(  "=")){mk.push(oe(binop{opw},15,true));}
          else if(opw == operator_word( "+=")){mk.push(oe(binop{opw},15,true));}
          else if(opw == operator_word( "-=")){mk.push(oe(binop{opw},15,true));}
          else if(opw == operator_word( "*=")){mk.push(oe(binop{opw},15,true));}
          else if(opw == operator_word( "/=")){mk.push(oe(binop{opw},15,true));}
          else if(opw == operator_word( "%=")){mk.push(oe(binop{opw},15,true));}
          else if(opw == operator_word("<<=")){mk.push(oe(binop{opw},15,true));}
          else if(opw == operator_word(">>=")){mk.push(oe(binop{opw},15,true));}
          else if(opw == operator_word( "|=")){mk.push(oe(binop{opw},15,true));}
          else if(opw == operator_word( "&=")){mk.push(oe(binop{opw},15,true));}
          else if(opw == operator_word( "^=")){mk.push(oe(binop{opw},15,true));}
          else if(opw == operator_word(  ".")){mk.push(oe(binop{opw}, 1));}
          else if(opw == operator_word( "->")){mk.push(oe(binop{opw}, 1));}
          else if(opw == operator_word(  ",")){return result::got_comma;}
          else if(opw == operator_word(  ":")){return result::got_colon;}
          else
            if(opw == operator_word(  "?"))
            {
              ++cur;

              mk.push(oe(binop{opw},16));

              maker  comk;

                if(read(cur,comk,tbl) != result::got_colon)
                {
                  printf("\'?\'に対応する\':\'が見つからない\n");

                  return result::got_error;
                }


              ++cur;

              auto  l = comk.output();

              comk.clear();

                if(read(cur,comk,tbl) == result::got_error)
                {
                  printf("paired_expr作成エラー\n");

                  return result::got_error;
                }


              auto  r = comk.output();

              mk.push(operand(paired_expr(l,r)));

              continue;
            }

          else
            if(opw == operator_word(";"))
            {
              return result::got_semicolon;
            }

          else
            {
              printf("不明な演算子\n");

              return result::got_error;
            }


          ++cur;
        }

      else
        if(tok.is_block())
        {
          auto&  blk = tok.get_block();

            if(blk.test('(',')'))
            {
              cursor  cocur(blk);

                if(mk.get_last().is_operand())
                {
                  mk.push(oe(binop{operator_word("()")},1));


                  auto  els = make_expr_list(cocur,tbl);

                  mk.push(operand(std::move(els)));
                }

              else
                {
                  auto  e = make_expr(cocur,tbl);

                  mk.push(operand(std::move(e)));
                }
            }

          else
            if(blk.test('[',']'))
            {
              cursor  cocur(blk);

                if(mk.get_last().is_operand())
                {
                  mk.push(oe(binop{operator_word("[]")},1));

                  mk.push(operand(make_expr(cocur,tbl)));
                }

              else
                {
                  printf("不正なサブスクリプト\n");

                  return result::got_error;
                }
            }

          else
            {
              printf("不明な字句列\n");

//              toks.print(stdout,0);

              return result::got_error;
            }


          ++cur;
        }
    }


  return result::got_end;
}


}




expr
make_expr(gbstd::string_view  sv, table&  tbl) noexcept
{
  stream  s(sv.data());

  std::vector<token>  toks;

    for(;;)
    {
      auto  tok = s.read_token();

        if(!s)
        {
          break;
        }


      toks.emplace_back(std::move(tok));
    }


  cursor  cur(toks.data(),toks.data()+toks.size());

  return make_expr(cur,tbl);
}


expr
make_expr(cursor&  cur, table&  tbl) noexcept
{
  maker  mk;

    for(;;)
    {
      auto  ptr = cur->get_pointer();

        switch(read(cur,mk,tbl))
        {
      case(result::got_end):
      case(result::got_semicolon):
          goto QUIT;
          break;
      case(result::got_colon):
          printf("処理できない\':\'\n");

//          ctx.print();
          goto QUIT;
          break;
      case(result::got_comma):
          mk.push(operator_egg(binary_operator{operator_word(",")},16));

          ++cur;
          break;
      case(result::got_error):
          printf("make_expr error\n");

//          ctx.print();
          goto QUIT;
          break;
        }
    }


QUIT:
  auto  e = mk.output();

  return std::move(e);
}


expr_list
make_expr_list(cursor&  cur, table&  tbl) noexcept
{
  std::vector<expr>  buf;

  maker  mk;

  expr  e;

    for(;;)
    {
      auto  ptr = cur->get_pointer();

      mk.clear();

        switch(read(cur,mk,tbl))
        {
      case(result::got_end):
          e = mk.output();

            if(e)
            {
              buf.emplace_back(std::move(e));
            }


          goto QUIT;
          break;
      case(result::got_colon):
          printf("処理できない\':\'\n");

//          ctx.print();

          ++cur;

          goto QUIT;
          break;
      case(result::got_semicolon):
          printf("処理できない\';\'\n");

//          ctx.print();

          ++cur;

          goto QUIT;
          break;
      case(result::got_comma):
          e = mk.output();

            if(!e)
            {
              goto QUIT;
            }


          buf.emplace_back(std::move(e));

          ++cur;
          break;
      case(result::got_error):
          printf("make_expr_list error\n");

//          ctx.print();
          goto QUIT;
          break;
        }
    }


QUIT:
  expr_list  els(buf.data(),buf.size());

  return std::move(els);
}




}}




