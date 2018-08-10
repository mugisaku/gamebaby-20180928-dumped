#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




void
chunk_set::
clear() noexcept
{
  m_ihdr = nullptr;

  m_between_ihdr_and_idat.clear();

  m_actl     = nullptr;
  m_plte     = nullptr;
  m_top_fctl = nullptr;

  m_idat.clear();

  m_after_idat.clear();

  m_animation_elements.clear();
}


void
chunk_set::
read_after_ihdr(const chunk*&  it, const chunk*  it_end) noexcept
{
    while(it != it_end)
    {
      auto&  chk = *it++;

        if(chk == "IDAT")
        {
          m_idat.emplace_back(&chk);

          read_idat(it,it_end);

          return;
        }

      else
        if(chk == "PLTE")
        {
          m_between_ihdr_and_idat.emplace_back(&chk);

            if(m_plte)
            {
              printf("chunk_set read error: there are multiple palettes\n");
            }


          m_plte = &chk;
        }

      else
        if(chk == "acTL")
        {
          m_between_ihdr_and_idat.emplace_back(&chk);

            if(m_actl)
            {
              printf("chunk_set read error: there are multiple animation controls\n");
            }


          m_actl = &chk;
        }

      else
        if(chk == "fcTL")
        {
          m_between_ihdr_and_idat.emplace_back(&chk);

            if(m_top_fctl)
            {
              printf("chunk_set read error: there are multiple top flame controls\n");
            }


          m_top_fctl = &chk;
        }

      else
        {
          m_between_ihdr_and_idat.emplace_back(&chk);
        }
    }


  printf("chunk_set read_png error: there is no IDAT\n");
}


void
chunk_set::
read_idat(const chunk*&  it, const chunk*  it_end) noexcept
{
    while(it != it_end)
    {
      auto&  chk = *it++;

        if(chk == "IDAT")
        {
          m_idat.emplace_back(&chk);
        }

      else
        {
          m_after_idat.emplace_back(&chk);

          break;
        }
    }
}


void
chunk_set::
read_after_idat(const chunk*&  it, const chunk*  it_end) noexcept
{
    while(it != it_end)
    {
      auto&  chk = *it++;

        if(chk == "fcTL")
        {
          m_animation_elements.emplace_back(&chk);
        }

      else
        if(chk == "fdAT")
        {
            if(m_animation_elements.empty())
            {
              printf("chunk_set read error: invalid position fdAT\n");
            }

          else
            {
              auto&  e = m_animation_elements.back();

              e.append(&chk);

              read_fdat(it,it_end,&e);
            }
        }

      else
        {
          m_after_idat.emplace_back(&chk);
        }
    }
}


void
chunk_set::
read_fdat(const chunk*&  it, const chunk*  it_end, animation_element*  e) noexcept
{
    while(it != it_end)
    {
      auto&  chk = *it++;

        if(chk == "fdAT")
        {
          e->append(&chk);
        }

      else
        if(chk == "dcTL")
        {
          m_animation_elements.emplace_back(&chk);

          e = &m_animation_elements.back();
        }

      else
        {
          m_after_idat.emplace_back(&chk);
        }
    }
}


chunk_set&
chunk_set::
assign(const chunk_list&  ls) noexcept
{
  clear();

  auto  it     = ls.begin();
  auto  it_end = ls.end();

    if((it == it_end) || (*it != "IHDR"))
    {
      printf("chunk_set assign error: \n");

      return *this;
    }


  m_ihdr = it++;

  read_after_ihdr(it,it_end);

  return *this;
}




void
chunk_set::
print() const noexcept
{
  m_ihdr->print();

    for(auto  chk: m_between_ihdr_and_idat)
    {
      chk->print();
    }


    if(m_plte)
    {
      printf("have palette\n");
    }


    if(m_actl)
    {
      printf("have animation control\n");
    }


    if(m_top_fctl)
    {
      printf("have top frame control\n");
    }


    for(auto  chk: m_idat)
    {
      chk->print();
    }


    for(auto  chk: m_after_idat)
    {
      chk->print();
    }


    if(m_animation_elements.size())
    {
      printf("have %4d animation elements\n",m_animation_elements.size());
    }
}




}




