#pragma once

#include "G4ThreeVector.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4RandomTools.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"

class PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
    public:
        PrimaryGenerator();
        ~PrimaryGenerator();

        void GeneratePrimaries(G4Event *anEvent) override;
        
        void setSeed(long seed);
        void setDistributionParametrs(G4double sigmaX, G4double sigmaY, G4double p0);

    private:
        G4ParticleGun *fParticleGun;

        G4double sigmaX, sigmaY, p0;
        G4ThreeVector CreateEventCoordinates();
};
