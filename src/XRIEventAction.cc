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
/// \file XRIEventAction.cc
/// \brief Implementation of the XRIEventAction class

#include "XRIEventAction.hh"
#include "XRIRunAction.hh"
#include "XRIHistogramManager.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4AccumulableManager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XRIEventAction::XRIEventAction(XRIRunAction* runAction)
    : G4UserEventAction(),
      fRunAction(runAction),
      //      fEdep(0.),
      fEdepFluo(0.)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XRIEventAction::~XRIEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void XRIEventAction::BeginOfEventAction(const G4Event*)
{    
    //    fEdep = 0.;
    fEdepFluo = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void XRIEventAction::EndOfEventAction(const G4Event*)
{   
    // accumulate statistics in run action
    //  fRunAction->AddEdep(fEdep);

    // Get analysis manager instance
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    // Fill the histogram with the energy deposited (via charge collection) in fluorescence detector
    if (fEdepFluo > 0.)
        analysisManager->FillH1(2, fEdepFluo);
    //        analysisManager->FillH1(2, AddNoise(fEdepFluo));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//G4double XRIEventAction::AddNoise(G4double energy) const
//{
//    constexpr G4double fanoFactor = 0.1; // Ratio of the actual variance and Poissonian variance based on the number of created electron hole pairs: F = s/sqrt(N)
//    constexpr G4double nElectronHolePairs = 4.43; // The average energy to produce an electron-hole pair
//    constexpr G4double elecSigma = 95.; // Electronic noise [eV]. From FWHM, it can be calculated as elecDel = elecSigma * 2.355

//    const G4double charge = (energy / eV) / nElectronHolePairs; // get charge from deposited energy
//    const G4double delSq_eV = pow(2.355,2) * (fanoFactor * nElectronHolePairs * (charge * nElectronHolePairs) + pow(elecSigma, 2));
//    const G4double sigma_eV = std::sqrt(delSq_eV) / 2.355; // units: eV
//    const G4double sigma = sigma_eV / nElectronHolePairs; // units: charge
//    G4double charge_Noisy = CLHEP::RandGauss::shoot(charge, sigma); // add Gaussian noise

//    return charge_Noisy * nElectronHolePairs * eV; // convert back to energy
//}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
