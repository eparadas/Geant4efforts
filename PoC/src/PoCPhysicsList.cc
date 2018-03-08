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
//
// **********************************************************************
// *                                                                    *
// *                    GEANT 4 xray_telescope advanced example         *
// *                                                                    *
// * MODULE:            PoCPhysicsList.cc                           *
// * -------                                                            *
// *                                                                    *
// * Version:           0.4                                             *
// * Date:              06/11/00                                        *
// * Author:            R Nartallo                                      *
// * Organisation:      ESA/ESTEC, Noordwijk, THe Netherlands           *
// *                                                                    *
// **********************************************************************
// 
// CHANGE HISTORY
// --------------
//
// 06.11.2000 R.Nartallo
// - First implementation of xray_telescope Physics list
// - Based on Chandra and XMM models 
// 
//
// **********************************************************************

#include "PoCPhysicsList.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicsListHelper.hh"
#include "G4OpticalPhoton.hh"
#include "G4Gamma.hh"
#include "G4GammaConversion.hh"
#include "G4ComptonScattering.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

PoCPhysicsList::PoCPhysicsList():  G4VUserPhysicsList()
{

	defaultCutValue = 2.0*m;
	cutForGamma     = 1.0*m;
	cutForElectron  = 1.0*nm;


	SetVerboseLevel(3);
}

PoCPhysicsList::~PoCPhysicsList()
{}



void PoCPhysicsList::ConstructProcess()
{

	// Define transportation process
	AddTransportation();
	// electromagnetic processes
	ConstructEM();
}
void PoCPhysicsList::ConstructEM()
{
	// Get pointer to G4PhysicsListHelper
	G4PhysicsListHelper *ph = G4PhysicsListHelper::GetPhysicsListHelper();
	//  Get pointer to gamma
	G4ParticleDefinition *particle = G4Gamma::GammaDefinition();
	// Construct and register processes for gamma
	ph->RegisterProcess(new G4PhotoElectricEffect(), particle);
	ph->RegisterProcess(new G4ComptonScattering(), particle);
	ph->RegisterProcess(new G4GammaConversion(), particle);
	// ph->RegisterProcess(new G4RayleighScattering(), particle);
}

void PoCPhysicsList::SetCuts()
{
  // defaultCutValue you have typed in is used

  if (verboseLevel >1){
    G4cout << "PoCPhysicsList::SetCuts:" << G4endl;
  }

  // set cut values for gamma at first and for e- second
  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForElectron, "e+");

  // SetCutValueForOthers(defaultCutValue); 

  if (verboseLevel >1) {
    DumpCutValuesTable();
  }
}

void PoCPhysicsList::ConstructParticle()
{
  // Here are constructed all particles
  ConstructBosons();
  ConstructLeptons();
}

void PoCPhysicsList::ConstructBosons()
{
  // gamma
  G4Gamma::GammaDefinition();

  // optical photon
  G4OpticalPhoton::OpticalPhotonDefinition();
}

void PoCPhysicsList::ConstructLeptons()
{
  // leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
}