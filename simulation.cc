#include <G4RunManagerFactory.hh>
#include <G4UImanager.hh>
#include <G4UIExecutive.hh>
#include <G4VisManager.hh>
#include <G4VisExecutive.hh>

#include "headers/DetectorConstruction.hh"
#include "headers/PhysicsList.hh"
#include "headers/ActionInitialization.hh"

int main(int argc, char** argv)
{
    // Создание дефолтного run manager`a 
    auto runManager = G4RunManagerFactory::CreateRunManager();

    // Инициализация обязательных классов
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserInitialization(new ActionInitialization());
    
    // Инициализация ядра Geant4
    runManager->Initialize();

    auto ui = new G4UIExecutive(argc, argv);

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    UImanager->ApplyCommand("/vis/open OGL");
    UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 1 1 1");
    UImanager->ApplyCommand("/vis/drawVolume");
    UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");
    UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");

    ui->SessionStart();

    // Окончание симуляции
    // Удаление ран менеджера
    delete runManager;
    return 0;
}
