// (GHEP) total energy of all final state particles with individual energy smearing using lognormal distribution.
// ALL HADRON PRODUCTS ARE FORCED TO DECAY INTO: pion, muon, Kp/Km, electron, proton, neutron, gamma, each with their own resolutions
// and some final state particles have a chance to be left out of the total energy sum - e.g. 20% chance to see neutrons at all, 0% chance to see neutrinos and K0.
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


void kine5sm()
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

auto myHist = new TH1D("h1","Total Final State Energy;Probability",100,0,40);

//Mapping

//first element is resolution, second element is chance to observe the given particle

double PiP_info[2] = {0.15, 1}; 
double PiM_info[2] = {0.15, 1};
double Pi0_info[2] = {0.15, 1};
double KP_info[2] = {0.2, 1};
double KM_info[2] = {0.2, 1};
double Gamma_info[2] = {0.3, 0.5};
double Proton_info[2] = {0.4, 1};
double Antiproton_info[2] = {0.4, 1};
double Electron_info[2] = {0.4, 1};
double Positron_info[2] = {0.4, 1};
double Muon_info[2] = {0.15, 1};
double Antimuon_info[2] = {0.15, 1};
double Neutron_info[2] = {0.8, 0.5};
double Antineutron_info[2] = {0.8, 0.5};


std::map<int,double*> myMap; //map particle to individual smearing resolution and chanceToSee

myMap[kPdgPiP] = PiP_info;
myMap[kPdgPiM] = PiM_info;
myMap[kPdgPi0] = Pi0_info;
myMap[kPdgKP] = KP_info;
myMap[kPdgKM] = KM_info;
myMap[kPdgGamma] = Gamma_info;
myMap[kPdgProton] = Proton_info;
myMap[kPdgAntiProton] = Antiproton_info;
myMap[kPdgElectron] = Electron_info;
myMap[kPdgPositron] = Positron_info;
myMap[kPdgMuon] = Muon_info;
myMap[kPdgAntiMuon] = Antimuon_info;
myMap[kPdgNeutron] = Neutron_info;
myMap[kPdgAntiNeutron] = Antineutron_info;


//RNG

    std::random_device rd;
    std::mt19937 gen(rd()); //generate random numbers according to mersenne twister


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
    int myPdg = p->Pdg();
    double myE = p->E();

    double resolution = (myMap.find(myPdg)->second)[0]; //links PDG code to first element of info array - resolution
    double chanceToSee = (myMap.find(myPdg)->second)[1]; //links PDG code to second element of info array - chance of particle being observed

    double var = pow((resolution*myE),2); //define Var[X]
    double eSq = pow(myE,2); //define E[X]^2
    
    double m = log(eSq/(pow(var+eSq,0.5)));
    double s = pow(log(1+(var/eSq)),0.5);


          if(myStatus == 1 && resolution > 0){ //if its a final state particle and resolution is greater than zero 
//this is done to eliminate all particles we don't see - they won't be in the map, so their resolution cannot be greater than 0

            std::lognormal_distribution<double> distLognorm(m,s); //takes the parameters: m and the s
           
            std::uniform_real_distribution<double> distUni(0,1); //generates a random double between 0 and 1 (i.e. in the interval [0,1) ) according to uniform distribution
           
          if (distUni(gen) < chanceToSee){ //e.g. if chance to see is 0.8, then 80% of the time, disUni will generate a number less than 0.8, allowing the smeared energy value to be added to the total energy sum

            eSum += distLognorm(gen);

          }

      }
   }

    myHist->Fill(eSum); // for each event, fill smeared energy sum of each final state particle

    mcrec->Clear();
 
  }

 TH1*normh = (TH1D*)(myHist->Clone("normh"));
   normh->Scale(1./normh->GetEntries());

TFile *outfile = new TFile("kine5sm_1.root","RECREATE"); 
outfile->cd();
normh->Write();   
outfile->Close();

}
