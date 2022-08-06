// (GENIE) transverse momentum distribution without momentum smearing

#include <iostream>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TH1D.h"
#include "TH2.h"
#include <iomanip>
#include <map>
#include <random>


#include "GHepParticle.h"
#include "ScatteringType.h"
#include "ProcessInfo.h"
#include "TObjArray.h"
#include "PDGLibrary.h"

#include "TString.h" 

#include "NtpMCTreeHeader.h" 
#include "NtpMCEventRecord.h" 
#include "EventRecord.h" 
#include "BaryonResUtils.h"
     
#include "PDGUtils.h"
#include "GHepStatus.h"

#include "PDGCodes.h"
#include "TDatabasePDG.h"

#include "TVector3.h"


void kine3()
{

using namespace genie;

// Open the GHEP/ROOT file

  string filename = "/hepstore/ivohra/miniboone1.ghep.root"; //miniboone 1 for numu, miniboone 2 for antinumu
  TFile infile(filename.c_str());


  // Get the tree header & print it


  NtpMCTreeHeader * header =
    dynamic_cast<NtpMCTreeHeader*> (infile.Get("header"));

  // Get the GENIE GHEP tree and set its branch address

  TTree * tree = dynamic_cast<TTree*> (infile.Get("gtree"));
  NtpMCEventRecord * mcrec = 0;
  tree->SetBranchAddress("gmcrec", &mcrec);

  //Make Hist

auto myHist = new TH1D("h1","Total Transverse Momentum;Momentum;Probability",100,0,1);

  // Event loop

//loop over each event
  for(Long64_t i=0; i < (tree->GetEntries()); i++){
    tree->GetEntry(i);

 // put your event analysis code here

 EventRecord & event = *(mcrec->event);

   TObjArrayIter iter(&event);
   GHepParticle * p = 0;

    double Ptotx = 0; //sum of momentum x coords
    double Ptoty = 0; //sum of momentum y coords

   // loop over event particles
   while((p = dynamic_cast<GHepParticle *>(iter.Next()))) { 

    int myStatus = p->Status();

      if(myStatus == 1){ //if its a final state particle

        double myPx = p->Px();
        double myPy = p->Py();

        Ptotx += myPx;
        Ptoty += myPy;


            } 
        }

    TVector3 Ptotvec (Ptotx, Ptoty,0); //total transverse momentum vector

    myHist->Fill(Ptotvec.Mag()); // for each event, fill total transverse momentum vector magnitude

    mcrec->Clear();

    }

 TH1*normh = (TH1D*)(myHist->Clone("normh"));
   normh->Scale(1./normh->GetEntries());

TFile *outfile = new TFile("kine3_1.root","RECREATE"); 
outfile->cd();
normh->Write();   
outfile->Close();

}
