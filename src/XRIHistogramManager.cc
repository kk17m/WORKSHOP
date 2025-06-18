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
/// \file XRIHistogramManager.cc
/// \brief Implementation of the XRIHistogramManager class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "XRIHistogramManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "XRIDetectorConstruction.hh"
#include "G4RunManager.hh"

#define ENABLE_NTUPLE 0

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XRIHistogramManager::XRIHistogramManager()
  : fFileName("XRI")
{
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XRIHistogramManager::~XRIHistogramManager()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void XRIHistogramManager::Book()
{
  // Create or get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetFileName(fFileName);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true);     //enable inactivation of histograms

  analysisManager->SetHistoDirectoryName("histo");
  analysisManager->SetFirstHistoId(1);

  G4int nBins = 1000;
  G4double eMin = 0.*keV;
  G4double eMax = 80.*keV;
  G4double hLength = 50.*mm;

  G4int ih = analysisManager->CreateH1("h1.1", "Ideal Fluorescence Detector: Incident energy spectrum", nBins, eMin, eMax, "keV");
  analysisManager->SetH1Activation(ih, true);

  ih = analysisManager->CreateH1("h1.2","Realistic Fluorescence Detector: Deposited energy, noise, ...", nBins, eMin, eMax, "keV");
  analysisManager->SetH1Activation(ih, true);

  ih = analysisManager->CreateH2("h2.1","Ideal Transmission Detector: Counts in pixels", 500, -hLength, hLength, 500, -hLength, hLength, "mm", "mm");
  analysisManager->SetH2Activation(ih, true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

