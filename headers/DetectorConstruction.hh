#pragma once 

#include "G4VPhysicalVolume.hh"
#include <G4VUserDetectorConstruction.hh>
#include <G4Material.hh>
#include <G4SystemOfUnits.hh>
#include <G4Box.hh>
#include <G4NistManager.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4UnitsTable.hh>
#include <G4EllipticalTube.hh>
#include <G4UnionSolid.hh>
#include <G4Tubs.hh>
#include <G4RotationMatrix.hh>

class DetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        DetectorConstruction();
        ~DetectorConstruction();

        G4VPhysicalVolume *Construct() override;
};
