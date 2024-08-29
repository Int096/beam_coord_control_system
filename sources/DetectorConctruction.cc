#include "../headers/DetectorConstruction.hh"

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
    constexpr G4double atomicNumber_vacuum = 1.;
    constexpr G4double massOfMole_vacuum = 1.008* g/mole;
    constexpr G4double density_vacuum = 1.e-25*g/cm3;
    constexpr G4double temperature_vacuum = 2.73*kelvin;
    constexpr G4double pressure_vacuum = 3.e-18*pascal;
    
    auto vacuum = new G4Material("vacuum", 
                                 atomicNumber_vacuum,
                                 massOfMole_vacuum,
                                 density_vacuum, 
                                 kStateGas,
                                 temperature_vacuum,
                                 pressure_vacuum);
    
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

    return physWorld;
}
