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
/// \file XRIEventAction.hh
/// \brief Definition of the XRIEventAction class

#ifndef XRIEventAction_h
#define XRIEventAction_h 1

#include "G4UserEventAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

class XRIRunAction;

/// Event action class
///

class XRIEventAction : public G4UserEventAction
{
public:
    XRIEventAction(XRIRunAction* runAction);
    virtual ~XRIEventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    //    void AddEdep(G4double edep) { fEdep += edep; }
    void AddEdepFluo(G4double edep) { fEdepFluo += edep; }
    //    G4double AddNoise(G4double energy) const;

private:
    XRIRunAction* fRunAction;
    //    G4double fEdep;
    G4double fEdepFluo;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


