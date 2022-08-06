// (GHEP) total energy of all final state particles

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


void kine5()
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

auto myHist = new TH1D("h1","Total Final State Energy;Probability",100,-10,40);

  // Event loop

//loop over each event
  for(Long64_t i=0; i < (tree->GetEntries()); i++){
    tree->GetEntry(i);

 // put your event analysis code here

 EventRecord & event = *(mcrec->event);

   TObjArrayIter iter(&event); //set up the particle iterator
   GHepParticle * p = 0;

    double eSum = 0; //sum of total final state energy

   while((p = dynamic_cast<GHepParticle *>(iter.Next()))) { //loop over particles

    int myStatus = p->Status();
    // int myPdg = p->Pdg(); PDG not needed - all final state particles are used in this case
    double myE = p->E();

          if(myStatus == 1){ //if its a final state particle

            eSum += myE;

          }

      }
   

    myHist->Fill(eSum); // for each event, fill smeared energy sum of each final state particle

    mcrec->Clear();
 
  }

 TH1*normh = (TH1D*)(myHist->Clone("normh"));
   normh->Scale(1./normh->GetEntries());

TFile *outfile = new TFile("kine5_1.root","RECREATE"); 
outfile->cd();
normh->Write();   
outfile->Close();

}
