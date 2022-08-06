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


void prac4b()
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

auto myHist = new TH1D("h1","Particle Event Count",30,-0.5,30.5);

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
 myHist->GetXaxis()->SetBinLabel(11, "Phi");
 myHist->GetXaxis()->SetBinLabel(12, "Jpsi");
 myHist->GetXaxis()->SetBinLabel(13, "KP");
 myHist->GetXaxis()->SetBinLabel(14, "KM");
 myHist->GetXaxis()->SetBinLabel(15, "K0");
 myHist->GetXaxis()->SetBinLabel(16, "AntiK0");
 myHist->GetXaxis()->SetBinLabel(17, "KStarP");
 myHist->GetXaxis()->SetBinLabel(18, "KStarM");
 myHist->GetXaxis()->SetBinLabel(19, "KStar0");
 myHist->GetXaxis()->SetBinLabel(20, "DP");
 myHist->GetXaxis()->SetBinLabel(21, "DM");
 myHist->GetXaxis()->SetBinLabel(22, "D0");
 myHist->GetXaxis()->SetBinLabel(23, "AntiD0");
 myHist->GetXaxis()->SetBinLabel(24, "DPs");
 myHist->GetXaxis()->SetBinLabel(25, "DMs");
 myHist->GetXaxis()->SetBinLabel(26, "Gamma");
 myHist->GetXaxis()->SetBinLabel(27, "Proton/Neutron");
 myHist->GetXaxis()->SetBinLabel(28, "AntiProton/AntiNeutron");
 myHist->GetXaxis()->SetBinLabel(29, "Muon");
 myHist->GetXaxis()->SetBinLabel(30, "AntiMuon");



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
      pdgToBin.insert(pair<int, int>(kPdgPhi, 11));
      pdgToBin.insert(pair<int, int>(kPdgJpsi, 12));
      pdgToBin.insert(pair<int, int>(kPdgKP, 13));
      pdgToBin.insert(pair<int, int>(kPdgKM, 14));
      pdgToBin.insert(pair<int, int>(kPdgK0, 15));
      pdgToBin.insert(pair<int, int>(kPdgAntiK0, 16));
      pdgToBin.insert(pair<int, int>(kPdgKStarP, 17));
      pdgToBin.insert(pair<int, int>(kPdgKStarM, 18));
      pdgToBin.insert(pair<int, int>(kPdgKStar0, 19));
      pdgToBin.insert(pair<int, int>(kPdgDP, 20));
      pdgToBin.insert(pair<int, int>(kPdgDM, 21));
      pdgToBin.insert(pair<int, int>(kPdgD0, 22));
      pdgToBin.insert(pair<int, int>(kPdgAntiD0, 23));
      pdgToBin.insert(pair<int, int>(kPdgDPs, 24));
      pdgToBin.insert(pair<int, int>(kPdgDMs, 25));
      pdgToBin.insert(pair<int, int>(kPdgGamma, 26));
      pdgToBin.insert(pair<int, int>(kPdgProton, 27));
      pdgToBin.insert(pair<int, int>(kPdgAntiProton, 28));
       pdgToBin.insert(pair<int, int>(kPdgMuon, 29));
        pdgToBin.insert(pair<int, int>(kPdgAntiMuon, 30));


  // Event loop


    tree->GetEntry(2);

 EventRecord & event = *(mcrec->event);

   TObjArrayIter iter(&event);
   GHepParticle * p = 0;

   while((p = dynamic_cast<GHepParticle *>(iter.Next()))) { 

      int pdgc   = p->Pdg();
      int status = p->Status();

      if(status == 1){

 auto binNumber = (pdgToBin.find(pdgc))->second;
        myHist->Fill(binNumber);

            } 

   }

TFile *outfile = new TFile("output_countevent2.root","RECREATE"); 
outfile->cd();
myHist->Write();   
outfile->Close();

}


