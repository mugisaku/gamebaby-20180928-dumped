#include"libgbstd/space.hpp"




namespace gbstd{
namespace spaces{




void
object::
update_core() noexcept
{
  auto  ene = get_kinetic_energy();

  add_base_point(ene);


  auto  env = get_environment();

    if(env)
    {
      ene.y += env->get_gravitation();

      ene *= (1.0-env->get_fluid_viscosity());

      ene += env->get_fluid_kinetic_energy();

      set_kinetic_energy(ene);
    }

}


void
object::
print() const noexcept
{
  printf("object: {\n");

  body::print();

//  printf("kinetic energy: {%8f,%8f}\n",m_kinetic_energy.x,m_kinetic_energy.y);

  printf("}\n\n");
}




}}




