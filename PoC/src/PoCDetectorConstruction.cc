//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: PoCDetectorConstruction.cc 94307 2015-11-11 13:42:46Z gcosmo $
//
/// \file PoCDetectorConstruction.cc
/// \brief Implementation of the PoCDetectorConstruction class

#include "PoCDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Para.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoCDetectorConstruction::PoCDetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PoCDetectorConstruction::~PoCDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* PoCDetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4double env_sizeXY = 20*cm, env_sizeZ = 30*cm;
  // G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");

  G4Material* VacuumMaterial = new G4Material ("vacuum", 7., 14.007*g/mole, 0.0000000000001*g/cm3); 
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  // G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        VacuumMaterial,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Envelope
  //  
  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        VacuumMaterial,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 

  //Here we define the mirror

  // Build materials
  G4Material* Ni = nist->FindOrBuildMaterial("G4_Ni");//("Nickel", 28., 58.6934*g/mole, 8.902*g/cm3);
  G4Material* Au = nist->FindOrBuildMaterial("G4_Au");//("Gold", 79., 196.96654*g/mole, 19.300*g/cm3);
  G4Material* Al = nist->FindOrBuildMaterial("G4_Al");//("Aluminium", 13., 26.98*g/mole, 2.700*g/cm3);
  // Visualization attributes
  G4VisAttributes* VisAttMirror = new G4VisAttributes(
                                                      G4Colour(0/255., 0/255.,255/255.));
  G4VisAttributes* VisAttAuCoating = new G4VisAttributes(
                                                         G4Colour(255/255., 255/255., 0/255.));
  G4VisAttributes* VisAttBaffle = new G4VisAttributes(
                                                      G4Colour(128/255., 128/255., 128/255.));


  G4double NiThickness = 1.07*mm;
  G4double AuCoating = 50.0e-6*mm;
  G4double BaffleGap = 4.0*mm;

  G4Para *Mirror = new G4Para("Mirror",
                4.5*cm,
                NiThickness,
                4.5*cm,
                0,
                0,
                0);
  G4Para *Coating = new G4Para("Coating",
                               4.5*cm,
                               AuCoating,
                               4.5*cm,
                               0,
                               0,
                               0);
  G4Para *Baffle = new G4Para("Baffle",
                              4.5*cm,
                              BaffleGap,
                              4.5*cm,
                              0,
                              0,
                              0);

  G4LogicalVolume* MirrorLogicalVolume = new G4LogicalVolume(Mirror,
                                                             Ni,
                                                             "Mirror");
  G4LogicalVolume* CoatingLogicalVolume = new G4LogicalVolume(Coating,
                                                              Au,
                                                              "Coating");
  G4LogicalVolume* BaffleLogicalVolume = new G4LogicalVolume(Baffle,
                                                            Al,
                                                            "Baffle");



  MirrorLogicalVolume->SetVisAttributes(VisAttMirror);
  CoatingLogicalVolume->SetVisAttributes(VisAttAuCoating);
  BaffleLogicalVolume->SetVisAttributes(VisAttBaffle);

  G4ThreeVector MirrorPosition(0,8*cm,0);
  G4ThreeVector CoatingPosition(0,8*cm - AuCoating,0);
  G4ThreeVector BafflePosition(0,8*cm - (/*AuCoating + */BaffleGap),0);


  new G4PVPlacement(0,                       //no rotation
                    MirrorPosition,                    //at position
                    MirrorLogicalVolume,             //its logical volume
                    "Mirror_place",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);

  new G4PVPlacement(0,                       //no rotation
                    CoatingPosition,                    //at position
                    CoatingLogicalVolume,             //its logical volume
                    "Coating_place",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);

  new G4PVPlacement(0,                       //no rotation
                    BafflePosition,                    //at position
                    BaffleLogicalVolume,             //its logical volume
                    "Baffle_place",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);
  //     
  // Shape which will be the wafer
  //
  G4Material* WaferMaterial = nist->FindOrBuildMaterial("G4_Si");

  // G4Material* WaferMaterial = new G4Material("Silicon", 14., 28.0855*g/mole,  2.330*g/cm3);
  
  G4ThreeVector WaferPosition = G4ThreeVector(0, 0, 14*cm);


  //Wafer will be a tube with very small height
  G4Tubs *WaferTube = new G4Tubs("Wafer",  // name 
             0,  // inner radius 
             6*cm,  // outer radius 
             0.1*mm,    // Z half length 
             0*deg,  // starting Phi 
             360*deg); // segment angle 

                
  G4LogicalVolume* WaferLogicalVolume =                         
    new G4LogicalVolume(WaferTube,         //its solid
                        WaferMaterial,          //its material
                        "Wafer");           //its name
               
  new G4PVPlacement(0,                       //no rotation
                    WaferPosition,                    //at position
                    WaferLogicalVolume,             //its logical volume
                    "Wafer",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
                
  // Set Shape2 as scoring volume
  //
  fScoringVolume = WaferLogicalVolume;

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
