#include"libgbpng/png.hpp"


using namespace gbpng;


int
main(int  argc, char**  argv)
{
    if(argc != 2)
    {
      return 0;
    }


  auto  path = argv[1];


  chunk_list  ls;

  ls.read_png_from_file(path);

  printf("%s\n",path);

  ls.print();

  printf("\n\n");

  
  chunk_set  set(ls);

  image_source  isrc;

  isrc.ihdr = set.get_image_header();

  auto  idat = set.get_image_data();

  isrc.data = idat.extract(isrc.ihdr);

  auto  trns_chunk = set.get_trns_chunk();
  auto  plte_chunk = set.get_plte_chunk();

    if(trns_chunk)
    {
      isrc.trns.assign(*trns_chunk,isrc.ihdr.get_pixel_format());
    }


    if(plte_chunk)
    {
      isrc.plte.assign(*plte_chunk);
    }


  direct_color_image  img(isrc);


  img.save_file("__save_g1.png",pixel_format::grayscale,1);
  img.save_file("__save_g2.png",pixel_format::grayscale,2);
  img.save_file("__save_g4.png",pixel_format::grayscale,4);
  img.save_file("__save_g8.png",pixel_format::grayscale,8);

  img.save_file("__save_rgb.png",pixel_format::rgb,8);
  img.save_file("__save_rgba.png",pixel_format::rgba,8);


  indexed_color_image  tmp = img;

  tmp.save_file("__save_1.png",1);
  tmp.save_file("__save_2.png",2);
  tmp.save_file("__save_4.png",4);
  tmp.save_file("__save_8.png",8);

  return 0;
}




