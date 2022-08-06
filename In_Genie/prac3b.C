// (GENIE) plot a 2D histogram of the counts of each particle type in each event (particle vs # per event vs count) - ***needs to be run in compiled mode!!***
//using miniboone April07 data, separately for matter and antimatter fluxes (1 = matter, 2= antimatter)

#include <iostream>
#include <string>

#include "GHepParticle.h"
#include "ScatteringType.h"
#include "ProcessInfo.h"
#include "GHepParticle.h"
#include "TObjArray.h"

#include "TString.h" 
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TAxis.h"

#include "NtpMCTreeHeader.h" 
#include "NtpMCEventRecord.h" 
#include "EventRecord.h" 
#include "BaryonResUtils.h"

#include "GHepParticle.h"       
#include "PDGUtils.h"
#include "GHepStatus.h"

#include "PDGCodes.h"



void prac3b()
{

using namespace genie;

// Open the GHEP/ROOT file

  string filename = "/hepstore/ivohra/miniboone1.ghep.root";
  TFile infile(filename.c_str());


  // Get the tree header & print it


  NtpMCTreeHeader * header =
    dynamic_cast<NtpMCTreeHeader*> (infile.Get("header"));

  // Get the GENIE GHEP tree and set its branch address

  TTree * tree = dynamic_cast<TTree*> (infile.Get("gtree"));
  NtpMCEventRecord * mcrec = 0;
  tree->SetBranchAddress("gmcrec", &mcrec);

  //Make Hist

auto myHist = new TH2D("h1","Particle Count",35,-0.5,34.5, 30, -0.5, 29.5); //for y bins, assuming max of 30 particles of each type are generated in a given event

 myHist->GetXaxis()->SetBinLabel(0, "Unknown");
 myHist->GetXaxis()->SetBinLabel(j, to_string(pdgToBin[j]));

//array

int pdgToBin[34] = {
kPdgPiP,
kPdgPiM,
kPdgPi0,
kPdgEta,
kPdgEtaPrm,
kPdgEtac,
kPdgEtab,
kPdgRhoP,
kPdgRhoM,
kPdgRho0,
kPdgomega,
kPdgPhi,
kPdgJpsi,
kPdgY,
kPdgKP,
kPdgKM,
kPdgK0,
kPdgAntiK0,
kPdgK0L,
kPdgK0S,
kPdgKStarP,
kPdgKStarM,
kPdgKStar0,
kPdgDP, 
kPdgDM,
kPdgD0,
kPdgAntiD0,
kPdgDPs,
kPdgDMs,
kPdgGluon,
kPdgGamma,
kPdgZ0,
kPdgWP, 
kPdgWM
};


//loop over each event
  for(Long64_t i=0; i<tree->GetEntries(); i++){
    tree->GetEntry(i);

 // put your event analysis code here

 EventRecord & event = *(mcrec->event);

     int particleCounter[35] = {0};

   TObjArrayIter iter(&event);
   GHepParticle * p = 0;

   // loop over event particles
   while((p = dynamic_cast<GHepParticle *>(iter.Next()))) { 

      int pdgc   = p->Pdg();
      int status = p->Status();

      if(status == 1){ //if its a final state particle

      auto binNumber = (pdgToBin.find(pdgc))->second;

      particleCounter[binNumber]++; 

            }
        }

    for(int j = 1; j <= 34; j++){

    myHist->Fill(j, particleCounter[j]);
    std::cout << "bin filling: " << j << "     " << "particles in given bin for this event: " << particleCounter[j] << endl;

   }
    std::cout << "event number: " << i << std::endl;

   mcrec->Clear();
  }

TFile *outfile = new TFile("output_count1.root","RECREATE"); 
outfile->cd();
myHist->Write();   
outfile->Close();
}