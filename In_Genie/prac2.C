// (GENIE) plot a 2D histogram of the scattering type vs the interaction type - ***needs to be run in compiled mode!!***
//using miniboone April07 data, separately for matter and antimatter fluxes (1 = matter, 2= antimatter)

#include <iostream>
#include <string>

#include "GHepParticle.h"
#include "ScatteringType.h"
#include "ProcessInfo.h"

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



void prac2()
{

using namespace genie;

// Open the GHEP/ROOT file

  string filename = "/hepstore/ivohra/miniboone2.ghep.root";
  TFile infile(filename.c_str());


  // Get the tree header & print it


  NtpMCTreeHeader * header =
    dynamic_cast<NtpMCTreeHeader*> (infile.Get("header"));

  // Get the GENIE GHEP tree and set its branch address

  TTree * tree = dynamic_cast<TTree*> (infile.Get("gtree"));
  NtpMCEventRecord * mcrec = 0;
  tree->SetBranchAddress("gmcrec", &mcrec);

  //Make Hist

auto myHist = new TH2D("h1","Different Interactions",20,-0.5,19.5, 3, 1.5, 4.5);

for (int i=1; i <= myHist->GetNbinsX(); i++){

auto binCenter = myHist->GetXaxis()->GetBinCenter(i);



string s = ScatteringType::AsString(ScatteringType_t(binCenter));

 myHist->GetXaxis()->SetBinLabel(i, s.c_str());

}

myHist->GetYaxis()->SetBinLabel(1, "CC");
myHist->GetYaxis()->SetBinLabel(2, "NC");
myHist->GetYaxis()->SetBinLabel(3, "Mix");

  // Event loop

  for(Long64_t i=0; i<tree->GetEntries(); i++){
    tree->GetEntry(i);

 // put your event analysis code here

 EventRecord & event = *(mcrec->event);

 const Interaction * in = event.Summary();
 
const ProcessInfo & proc = in->ProcInfo();

myHist->Fill(proc.ScatteringTypeId(), proc.InteractionTypeId());

   mcrec->Clear();
  }

TFile *outfile = new TFile("output_miniboone2.root","RECREATE"); 
outfile->cd();
myHist->Write();   
outfile->Close();
}