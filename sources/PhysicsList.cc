#include "../headers/PhysicsList.hh"

PhysicsList::PhysicsList()
{
    RegisterPhysics(new G4EmStandardPhysics());
    RegisterPhysics(new G4EmLivermorePhysics());
}

PhysicsList::~PhysicsList()
{}
