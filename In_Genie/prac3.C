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



void prac3()
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
 myHist->GetXaxis()->SetBinLabel(1, "PiP");
 myHist->GetXaxis()->SetBinLabel(2, "PiM");
 myHist->GetXaxis()->SetBinLabel(3, "Pi0");
 myHist->GetXaxis()->SetBinLabel(4, "Eta");
 myHist->GetXaxis()->SetBinLabel(5, "EtaPrm");
 myHist->GetXaxis()->SetBinLabel(6, "Etac");
 myHist->GetXaxis()->SetBinLabel(7, "Etab");
 myHist->GetXaxis()->SetBinLabel(8, "RhoP");
 myHist->GetXaxis()->SetBinLabel(9, "RhoM");
 myHist->GetXaxis()->SetBinLabel(10, "Rho0");
 myHist->GetXaxis()->SetBinLabel(11, "omega");
 myHist->GetXaxis()->SetBinLabel(12, "Phi");
 myHist->GetXaxis()->SetBinLabel(13, "Jpsi");
 myHist->GetXaxis()->SetBinLabel(14, "Y");
 myHist->GetXaxis()->SetBinLabel(15, "KP");
 myHist->GetXaxis()->SetBinLabel(16, "KM");
 myHist->GetXaxis()->SetBinLabel(17, "K0");
 myHist->GetXaxis()->SetBinLabel(18, "AntiK0");
 myHist->GetXaxis()->SetBinLabel(19, "K0L");
 myHist->GetXaxis()->SetBinLabel(20, "K0S");
 myHist->GetXaxis()->SetBinLabel(21, "KStarP");
 myHist->GetXaxis()->SetBinLabel(22, "KStarM");
 myHist->GetXaxis()->SetBinLabel(23, "KStar0");
 myHist->GetXaxis()->SetBinLabel(24, "DP");
 myHist->GetXaxis()->SetBinLabel(25, "DM");
 myHist->GetXaxis()->SetBinLabel(26, "D0");
 myHist->GetXaxis()->SetBinLabel(27, "AntiD0");
 myHist->GetXaxis()->SetBinLabel(28, "DPs");
 myHist->GetXaxis()->SetBinLabel(29, "DMs");
 myHist->GetXaxis()->SetBinLabel(30, "Gluon");
 myHist->GetXaxis()->SetBinLabel(31, "Gamma");
 myHist->GetXaxis()->SetBinLabel(32, "Z0");
 myHist->GetXaxis()->SetBinLabel(33, "WP");
 myHist->GetXaxis()->SetBinLabel(34, "WM");


  // Event loop

//map
   std::map<int,int> pdgToBin;

      pdgToBin.insert(pair<int, int>(kPdgPiP, 1)); 
      pdgToBin.insert(pair<int, int>(kPdgPiM, 2)); 
      pdgToBin.insert(pair<int, int>(kPdgPi0, 3)); 
      pdgToBin.insert(pair<int, int>(kPdgEta, 4));
      pdgToBin.insert(pair<int, int>(kPdgEtaPrm, 5));
      pdgToBin.insert(pair<int, int>(kPdgEtac, 6));
      pdgToBin.insert(pair<int, int>(kPdgEtab, 7));
      pdgToBin.insert(pair<int, int>(kPdgRhoP, 8)); 
      pdgToBin.insert(pair<int, int>(kPdgRhoM, 9)); 
      pdgToBin.insert(pair<int, int>(kPdgRho0, 10));
      pdgToBin.insert(pair<int, int>(kPdgomega, 11));
      pdgToBin.insert(pair<int, int>(kPdgPhi, 12));
      pdgToBin.insert(pair<int, int>(kPdgJpsi, 13));
      pdgToBin.insert(pair<int, int>(kPdgY, 14));
      pdgToBin.insert(pair<int, int>(kPdgKP, 15));
      pdgToBin.insert(pair<int, int>(kPdgKM, 16));
      pdgToBin.insert(pair<int, int>(kPdgK0, 17));
      pdgToBin.insert(pair<int, int>(kPdgAntiK0, 18));
      pdgToBin.insert(pair<int, int>(kPdgK0L, 19));
      pdgToBin.insert(pair<int, int>(kPdgK0S, 20));
      pdgToBin.insert(pair<int, int>(kPdgKStarP, 21));
      pdgToBin.insert(pair<int, int>(kPdgKStarM, 22));
      pdgToBin.insert(pair<int, int>(kPdgKStar0, 23));
      pdgToBin.insert(pair<int, int>(kPdgDP, 24));
      pdgToBin.insert(pair<int, int>(kPdgDM, 25));
      pdgToBin.insert(pair<int, int>(kPdgD0, 26));
      pdgToBin.insert(pair<int, int>(kPdgAntiD0, 27));
      pdgToBin.insert(pair<int, int>(kPdgDPs, 28));
      pdgToBin.insert(pair<int, int>(kPdgDMs, 29));
      pdgToBin.insert(pair<int, int>(kPdgGluon, 30));
      pdgToBin.insert(pair<int, int>(kPdgGamma, 31));
      pdgToBin.insert(pair<int, int>(kPdgZ0, 32));
      pdgToBin.insert(pair<int, int>(kPdgWP, 33));
      pdgToBin.insert(pair<int, int>(kPdgWM, 34));

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