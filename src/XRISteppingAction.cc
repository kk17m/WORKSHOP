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
/// \file XRISteppingAction.cc
/// \brief Implementation of the XRISteppingAction class

#include "XRISteppingAction.hh"
#include "XRIEventAction.hh"
#include "XRIDetectorConstruction.hh"
#include "XRIHistogramManager.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XRISteppingAction::XRISteppingAction(XRIEventAction* eventAction)
    : G4UserSteppingAction(),
      fEventAction(eventAction),
      fScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XRISteppingAction::~XRISteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void XRISteppingAction::UserSteppingAction(const G4Step* step)
{
    // Check next volume, if nullptr (or out of world then return)
    const G4VPhysicalVolume* nextVol = step->GetTrack()->GetNextVolume();
    if(nextVol == nullptr)
        return;

    // get particle name
    const G4String particle = step->GetTrack()->GetParticleDefinition()->GetParticleName();

    // get volume name for the pre-step point
    const G4String preStepVol = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName();

    // get volume name for the post-step point
    const G4String postStepVol = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName();

    // get volume of the current step
    G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    // find local position coordinates using GetPtrTopTransform()
    const G4AffineTransform* transformation = step->GetTrack()->GetTouchable()->GetHistory()->GetPtrTopTransform();
    const G4ThreeVector localVetex = transformation->TransformPoint(step->GetTrack()->GetPosition());
    const G4double x = localVetex.x();
    const G4double y = localVetex.y();
    // const G4double z = localVetex.z();

    //
    // dose scoring volume
    //
    if (!fScoringVolume) {
        const XRIDetectorConstruction* detectorConstruction = static_cast<const XRIDetectorConstruction*>
                (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

        fScoringVolume = detectorConstruction->GetScoringVolume();
    }

    // check if we are in scoring volume
    if (volume == fScoringVolume) {

        // collect energy deposited in this step
        G4double edepStep = step->GetTotalEnergyDeposit();
        fEventAction->AddEdep(edepStep);
    }

    //
    // fluorescence detector
    //
    if ((preStepVol == "fluorescenceDet" && postStepVol == "World") && particle == "gamma")
    {
        const G4double ekin = step->GetTrack()->GetKineticEnergy();
        G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillH1(1, ekin);
    }

    //
    // transmission detector
    //
    if ((preStepVol == "transmissionDet" && postStepVol == "World") && particle == "gamma")
    {
        G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillH2(1, x, y);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

