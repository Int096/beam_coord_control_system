#include "../headers/PrimaryGenerator.hh"
#include "G4ios.hh"
#include <cmath>

PrimaryGenerator::PrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);
}

PrimaryGenerator::~PrimaryGenerator()
{
    delete fParticleGun;
}

void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    auto particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particle = particleTable->FindParticle("proton");

    G4ThreeVector momentum(0., 0., 1.);
    
    setDistributionParametrs(1., 0.8, 4);
    G4ThreeVector position = CreateEventCoordinates();

    G4cout << position << G4endl;

    fParticleGun->SetParticlePosition(position);
    fParticleGun->SetParticleMomentumDirection(momentum);
    fParticleGun->SetParticleMomentum(800*GeV);
    fParticleGun->SetParticleDefinition(particle);

    fParticleGun->GeneratePrimaryVertex(anEvent);
}

void PrimaryGenerator::setSeed(long seed)
{
}  

void PrimaryGenerator::setDistributionParametrs(G4double sigmaX, G4double sigmaY, G4double p0)
{
    this->sigmaX = sigmaX;
    this->sigmaY = sigmaY;
}

G4ThreeVector PrimaryGenerator::CreateEventCoordinates()
{
    G4double x, y, z;
    G4double s;
    
    do 
    {
        x = 2 * G4UniformRand() - 1;
        y = 2 * G4UniformRand() - 1;

        s = x*x + y*y;
    } while ((0 == s) || (s > 1));

    G4double X = sigmaX * x * std::sqrt(-2 * std::log(s) / s);
    G4double Y = sigmaY * y * std::sqrt(-2 * std::log(s) / s);

    return G4ThreeVector(X, Y, 0);
}
