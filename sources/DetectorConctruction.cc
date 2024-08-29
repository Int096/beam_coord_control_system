#include "../headers/DetectorConstruction.hh"
#include "CLHEP/Units/SystemOfUnits.h"

DetectorConstruction::DetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    /*-----------------------------------------------------------------------*/
    //          Работа с материалами
    /*-----------------------------------------------------------------------*/

    // Объект для работы с БД
    auto nist = G4NistManager::Instance();

    // Вакуум
    constexpr G4double vacuum_atomicNumber = 1.;
    constexpr G4double vacuum_massOfMole   = 1.008 * g/mole;
    constexpr G4double vacuum_density      = 1.e-25 * g/cm3;
    constexpr G4double vacuum_temperature  = 2.73 * kelvin;
    constexpr G4double vacuum_pressure     = 3.e-18 * pascal;
    
    auto vacuum = new G4Material("vacuum", 
                                 vacuum_atomicNumber,
                                 vacuum_massOfMole,
                                 vacuum_density, 
                                 kStateGas,
                                 vacuum_temperature,
                                 vacuum_pressure);
    
    // Нержавейка (Stainless steel)
    // https://hurel.hanyang.ac.kr/Geant4/Doxygen/10.03.p03/html/_brachy_material_8cc_source.html
    // (Medical Physics, Vol 25, No 10, Oct 1998)

    auto elMn = new G4Element("Manganese", "Mn", 25., 54.94 * g/mole);
    auto elSi = new G4Element("Silicon",   "Si", 14., 28.09 * g/mole);
    auto elCr = new G4Element("Chromium",  "Cr", 24., 52.00 * g/mole);
    auto elNi = new G4Element("Nickel",    "Ni", 28., 58.70 * g/mole);
    auto elFe = new G4Element("Iron",      "Fe", 26., 55.85 * g/mole);
    
    auto stainlessSteel = new G4Material("Stainless steel", 7.999*g/cm3, 5);
    stainlessSteel->AddElement(elMn, 0.02);
    stainlessSteel->AddElement(elSi, 0.01);
    stainlessSteel->AddElement(elCr, 0.19);
    stainlessSteel->AddElement(elNi, 0.10);
    stainlessSteel->AddElement(elFe, 0.68);

    /*-----------------------------------------------------------------------*/
    //          Создание Мира
    /*-----------------------------------------------------------------------*/

    // Геометрические размеры мира
    constexpr G4double world_x = 0.5*m;
    constexpr G4double world_y = 0.5*m;
    constexpr G4double world_z = 1.0*m;
    
    // Создание объемов мира
    auto solidWorld = new G4Box("solidWorld", world_x, world_y, world_z);
    auto logicWorld = new G4LogicalVolume(solidWorld, vacuum, "logicWorld");
    auto physWorld  = new G4PVPlacement(0,
                                        G4ThreeVector(0., 0., 0.),
                                        logicWorld,
                                        "physWorld",
                                        0,
                                        false,
                                        0);

    /*-----------------------------------------------------------------------*/
    //          Создание стенки
    /*-----------------------------------------------------------------------*/
   
    // Геометрические размеры стенки
    constexpr G4double plate_x = world_x;
    constexpr G4double plate_y = world_y;
    constexpr G4double plate_z = 2*cm;

    // Создание объемов пластины
    auto solidPlate = new G4Box("solidPlate", plate_x, plate_y, plate_z);
    auto logicPlate = new G4LogicalVolume(solidPlate, stainlessSteel, "logicPlate");
    auto physPlate  = new G4PVPlacement(0,
                                        G4ThreeVector(0., 0., 10. * cm),
                                        logicPlate,
                                        "physPlate",
                                        logicWorld,
                                        false,
                                        0);

    return physWorld;
}
