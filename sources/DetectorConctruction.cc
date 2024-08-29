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
    // Алюминий
    auto aluminium = new G4Material("Aluminium", 13., 26.98*g/mole, 2.7*g/cm3);
    
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
    constexpr G4double world_x = 0.1*m;
    constexpr G4double world_y = 0.1*m;
    constexpr G4double world_z = 0.3*m;
    
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
    //          Создание мишени 
    /*-----------------------------------------------------------------------*/
    
    // Геометрия мишени
    constexpr G4double target_Dx = 24*mm/2;
    constexpr G4double target_Dy = 14*mm/2;
    constexpr G4double target_Dz = 40*mm/2;
    constexpr G4double target_thickness = 30* 1.e-6*m;

    // Создание объемов
    auto solidTarget_inner = new G4EllipticalTube("solidTarget_inner",
                                                  target_Dx-2*target_thickness,
                                                  target_Dy-2*target_thickness,
                                                  target_Dz);
    auto solidTarget_external = new G4EllipticalTube("solidTarget_external",
                                                     target_Dx,
                                                     target_Dy,
                                                     target_Dz);

    auto solidTarget = new G4UnionSolid("solidTarget", solidTarget_external, solidTarget_inner);
    
    auto logicTarget = new G4LogicalVolume(solidTarget, aluminium, "logicTarget");
    auto physTarget  = new G4PVPlacement(0,
                                         G4ThreeVector(0., 0., -world_z+target_Dz+(world_z/10)),
                                         logicTarget,
                                         "physTarget",
                                         logicWorld,
                                         false,
                                         0);

    /*-----------------------------------------------------------------------*/
    //          Создание стенки
    /*-----------------------------------------------------------------------*/
   
    // Геометрические размеры стенки
    constexpr G4double plate_x = world_x;
    constexpr G4double plate_y = world_y;
    constexpr G4double plate_z = 0.5*cm;

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
    
    /*-----------------------------------------------------------------------*/
    //          Создание сцинтилляционных палочек 
    /*-----------------------------------------------------------------------*/
    
    // Геометрия палочек и иже с ними
    constexpr G4double tube_radius = 2.*mm;
    constexpr G4double tube_length = 4.*cm;

    constexpr G4double distance = 0.5 * 6*cm;

    constexpr G4double x_center = 0.*cm;

    // Поворот для палочек
    auto Rotation = new G4RotationMatrix();
    Rotation->rotateX(90*deg);
    Rotation->rotateY(0*deg);
    Rotation->rotateZ(0*deg);


    auto solidTube = new G4Tubs("solidTube", 0, tube_radius, tube_length, 0, 160);
    auto logicTube = new G4LogicalVolume(solidTube, aluminium, "logicTube");
    auto physTube_left  = new G4PVPlacement(Rotation,
                                       G4ThreeVector(distance - x_center, 0., world_z-tube_radius-(world_z/10)),
                                       logicTube,
                                       "physTube_left",
                                       logicWorld,
                                       false,
                                       0);
    auto physTube_right  = new G4PVPlacement(Rotation,
                                       G4ThreeVector(-distance - x_center, 0., world_z-tube_radius-(world_z/10)),
                                       logicTube,
                                       "physTube_right",
                                       logicWorld,
                                       false,
                                       0);
    return physWorld;
}
