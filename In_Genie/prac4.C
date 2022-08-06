// (GENIE) plot 1D histograms of the counts of a given particle in each event (given event vs count) - ***needs to be run in compiled mode!!***
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



void prac4()
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

auto myHist = new TH1D("h1","Particle Event Count",20000,0,20000);

  // Event loop

//loop over each event
  for(Long64_t i=0; i < (tree->GetEntries()); i++){
    tree->GetEntry(i);

 // put your event analysis code here

 EventRecord & event = *(mcrec->event);

   TObjArrayIter iter(&event);
   GHepParticle * p = 0;

   // loop over event particles
   while((p = dynamic_cast<GHepParticle *>(iter.Next()))) { 

      int pdgc   = p->Pdg();
      int status = p->Status();

      if(status == 1 && pdgc == 211){ //if its a final state particle and is the chosen type of particle determined by pdgc...

        myHist->Fill((i + 0.5)); //0.5 added so count gets put in bin tree->GetEntry(i)

            } 
        }
           mcrec->Clear();
   }


TFile *outfile = new TFile("output_count211.root","RECREATE"); 
outfile->cd();
myHist->Write();   
outfile->Close();

}


