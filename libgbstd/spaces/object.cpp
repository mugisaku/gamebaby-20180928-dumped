#include"libgbstd/space.hpp"




namespace gbstd{
namespace spaces{




void
object::
update_core() noexcept
{
  auto  ene = get_kinetic_energy();

  add_base_point(ene);
}


void
object::
print() const noexcept
{
  printf("object: {\n");

  body::print();

  printf("}\n\n");
}




}}




