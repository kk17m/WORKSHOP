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
/// \file XRIDetectorConstruction.cc
/// \brief Implementation of the XRIDetectorConstruction class

#include "XRIDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XRIDetectorConstruction::XRIDetectorConstruction()
    : G4VUserDetectorConstruction(),
      fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XRIDetectorConstruction::~XRIDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* XRIDetectorConstruction::Construct()
{  
    // Get nist material manager
    G4NistManager* nist = G4NistManager::Instance();

    // Option to switch on/off checking of volumes overlaps
    //
    G4bool checkOverlaps = true;

    //
    // World
    //
    G4double world_sizeXY = 1.*m;
    G4double world_sizeZ  = 1.*m;
    G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

    // Solid volume
    //
    G4Box* solidWorld = new G4Box("World",               // its name
                                  world_sizeXY / 2.,     // its size in X
                                  world_sizeXY / 2.,     // its size in Y
                                  world_sizeZ / 2.);     // its size in Z

    // Logic volume
    //
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,     // its solid
                                                      world_mat,      // its material
                                                      "World");       // its name

    // Physical volume
    //
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0,                     // no rotation
                                                     G4ThreeVector(),       // at (0,0,0)
                                                     logicWorld,            // its logical volume
                                                     "World",               // its name
                                                     0,                     // its mother  volume
                                                     false,                 // no boolean operation
                                                     0,                     // copy number
                                                     checkOverlaps);        // overlaps checking

    // Visualization properties
    //
    G4VisAttributes* workd_Attributes  = new G4VisAttributes();
    workd_Attributes->SetForceSolid(false);
    workd_Attributes->SetColour(1., 1., 1., 0.05);
    logicWorld->SetVisAttributes(workd_Attributes);

    //
    // Transmission detector
    //
    G4Material* transDet_mat = nist->FindOrBuildMaterial("G4_AIR");
    G4ThreeVector transDet_pos = G4ThreeVector(0., 0., -300.*mm);

    // Transmission detector shape
    //
    G4double fPx = 100.*mm / 2.;    // half length in x
    G4double fPy = 100.*mm / 2.;    // half length in y
    G4double fPz = 1.*mm / 2.;      // half length in z

    // Solid volume
    //
    G4Box* transDet_solid = new G4Box("transmissionDet",      //its name
                                      fPx,                // half length in x
                                      fPy,                // half length in y
                                      fPz);               // half length in z

    // Logic volume
    //
    G4LogicalVolume* transDet_logic = new G4LogicalVolume(transDet_solid,        // its solid
                                                          transDet_mat,          // its material
                                                          "transmissionDet");    // its name

    // Physical volume
    //
    new G4PVPlacement(0,                       // rotation
                      transDet_pos,            // at position
                      transDet_logic,          // its logical volume
                      "transmissionDet",       // its name
                      logicWorld,              // its mother  volume
                      false,                   // no boolean operation
                      0,                       // copy number
                      checkOverlaps);          // overlaps checking

    // Visualization properties
    //
    G4VisAttributes* transDet_Attributes  = new G4VisAttributes();
    transDet_Attributes->SetForceSolid(true);
    transDet_Attributes->SetColour(0., 0.9, 0.9, 0.5);
    transDet_logic->SetVisAttributes(transDet_Attributes);

    //
    // Imaging Object
    //
    G4Material* imagingObj_mat = nist->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRP");
    G4ThreeVector ImagingObj_pos = G4ThreeVector(0., 0., -70.*mm);
    G4RotationMatrix *ImagingObj_rot = new G4RotationMatrix(0., 0., 0.);
    ImagingObj_rot->rotateX(90.*deg);   // Rotate object by 90 deg about the X-axis

    // Cylindircal mice shape
    //
    G4double fRMin = 0.*mm;          // Inner radius
    G4double fRMax = 30.*mm / 2.;    // Outer radius
    G4double fDz = 80.*mm / 2.;      // half length in z
    G4double fSPhi = 0.*deg;         // The starting phi angle
    G4double fDPhi = 360.*deg;       // Delta angle of the segment

    // Solid volume
    //
    G4Tubs* ImagingObj_solid = new G4Tubs("imagingObject",      //its name
                                          fRMin,                // Inner radius
                                          fRMax,                // Outer radius
                                          fDz,                  // half length in z
                                          fSPhi,                // The starting phi angle
                                          fDPhi);               // Delta angle of the segment

    // Logic volume
    //
    G4LogicalVolume* ImagingObj_logic = new G4LogicalVolume(ImagingObj_solid,        // its solid
                                                            imagingObj_mat,          // its material
                                                            "imagingObject");        // its name

    // Physical volume
    //
    new G4PVPlacement(ImagingObj_rot,          // rotation
                      ImagingObj_pos,          // at position
                      ImagingObj_logic,        // its logical volume
                      "imagingObject",         // its name
                      logicWorld,              // its mother  volume
                      false,                   // no boolean operation
                      0,                       // copy number
                      checkOverlaps);          // overlaps checking

    // Visualization properties
    //
    G4VisAttributes* ImagingObj_Attributes  = new G4VisAttributes();
    ImagingObj_Attributes->SetForceSolid(true);
    ImagingObj_Attributes->SetColour(1., 0.9, 0.9, 0.5);
    ImagingObj_logic->SetVisAttributes(ImagingObj_Attributes);

    // Set imaging object as scoring volume for dose calculation
    //
    fScoringVolume = ImagingObj_logic;

    //
    // Always return the physical World
    //
    return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
