// (GENIE) plot distribution (2D histogram) of number of particle per event vs particle type (labeled) vs count (don't forget to normalize)

#include <iostream>
#include <string>

#include "GHepParticle.h"
#include "ScatteringType.h"
#include "ProcessInfo.h"
#include "GHepParticle.h"
#include "TObjArray.h"
#include "PDGLibrary.h"

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
#include "TDatabasePDG.h"



void prac6()
{

using namespace genie;

// Open the GHEP/ROOT file

  string filename = "/hepstore/ivohra/miniboone2.ghep.root"; //miniboone 1 for numu, miniboone 2 for antinumu
  TFile infile(filename.c_str());


  // Get the tree header & print it


  NtpMCTreeHeader * header =
    dynamic_cast<NtpMCTreeHeader*> (infile.Get("header"));

  // Get the GENIE GHEP tree and set its branch address

  TTree * tree = dynamic_cast<TTree*> (infile.Get("gtree"));
  NtpMCEventRecord * mcrec = 0;
  tree->SetBranchAddress("gmcrec", &mcrec);

  //Make Hist

auto myHist = new TH2D("h1","Particle Count Distribution;Counts Per Event;Type of Particle",30,-0.5,29.5, 30, 0.5, 30.5);


//mapping

std::map<int,int> pdgToBin; //use map to fit in bins

pdgToBin[kPdgPiP] = 1;
pdgToBin[kPdgPiM] = 2;
pdgToBin[kPdgPi0] = 3;
pdgToBin[kPdgEta] = 4;
pdgToBin[kPdgEtaPrm] = 5;
pdgToBin[kPdgEtac] = 6;
pdgToBin[kPdgEtab] = 7;
pdgToBin[kPdgRhoP] = 8;
pdgToBin[kPdgRhoM] = 9;
pdgToBin[kPdgRho0] = 10;
pdgToBin[kPdgPhi] = 11;
pdgToBin[kPdgJpsi] = 12;
pdgToBin[kPdgKP] = 13;
pdgToBin[kPdgKM] = 14;
pdgToBin[kPdgK0] = 15;
pdgToBin[kPdgAntiK0] = 16;
pdgToBin[kPdgKStarP] = 17;
pdgToBin[kPdgKStarM] =18;
pdgToBin[kPdgKStar0] = 19;
pdgToBin[kPdgDP] = 20;
pdgToBin[kPdgDM] = 21;
pdgToBin[kPdgD0] = 22;
pdgToBin[kPdgAntiD0] = 23;
pdgToBin[kPdgDPs] = 24;
pdgToBin[kPdgDMs] = 25;
pdgToBin[kPdgGamma] = 26;
pdgToBin[kPdgProton] = 27;
pdgToBin[kPdgAntiProton] = 28;
pdgToBin[kPdgMuon] = 29;
pdgToBin[kPdgAntiMuon] = 30;


//Y Axis Labelling 

TDatabasePDG * db = TDatabasePDG::Instance();

for (auto& x: pdgToBin){ //iterate through map

TParticlePDG * part = db->GetParticle(x.first);

myHist->GetYaxis()->SetBinLabel(x.second, part->GetName());

}


  // Event loop

//loop over each event
  for(Long64_t i=0; i < (tree->GetEntries()); i++){
    tree->GetEntry(i);

 // put your event analysis code here

 EventRecord & event = *(mcrec->event);

   TObjArrayIter iter(&event);
   GHepParticle * p = 0;

    int counter[31] = {0}; //remember - arrays start at 0.

   // loop over event particles
   while((p = dynamic_cast<GHepParticle *>(iter.Next()))) { 

      int pdgc   = p->Pdg();
      int status = p->Status();

      if(status == 1){ //if its a final state particle and is the chosen type of particle determined by pdgc...

      auto binNumber = (pdgToBin.find(pdgc))->second;

      counter[binNumber]++;

            } 
        }
            for(int i = 1; i<= 30; i++){ //after each event, fill the number of counts for each particle

                myHist->Fill(counter[i], i);


            }

           mcrec->Clear();
   }


 TH2*normh = (TH2D*)(myHist->Clone("normh"));
   normh->Scale(1./normh->GetEntries());

TFile *outfile = new TFile("antinumuoutput_partPDist.root","RECREATE"); 
outfile->cd();
normh->Write();   
outfile->Close();



}