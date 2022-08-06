//neutrino missing energy, with gaussian energy smearing (run in GENIE) -- needs gst format input root files

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

void kine2sm()
{
	using namespace genie;

	auto myHist = new TH1D("h1","Neutral Particle Energy;Energy;Probability",100,0.,2.);

	TFile *myFile = TFile::Open("/hepstore/ivohra/miniboone1.gst.root");

   	TTreeReader myReader("gst", myFile);

// pdgf

TTreeReaderArray<int> myPdgf(myReader, "pdgf");

// final energy of observable particles (cannot detect neutrinos, all other leptons are charged, so no need to include El)
	TTreeReaderArray<double> myEf(myReader, "Ef"); //energy of kth final state particle in hadronic system

//mapping for neutral particles

std::map<int,int> pdgToBin; //use map to find if particle is neutral

//neutral
pdgToBin[kPdgPi0] = 3;
pdgToBin[kPdgEta] = 4;
pdgToBin[kPdgEtaPrm] = 5;
pdgToBin[kPdgEtac] = 6;
pdgToBin[kPdgEtab] = 7;
pdgToBin[kPdgRho0] = 10;
pdgToBin[kPdgPhi] = 11;
pdgToBin[kPdgJpsi] = 12;
pdgToBin[kPdgK0] = 15;
pdgToBin[kPdgAntiK0] = 16;
pdgToBin[kPdgKStar0] = 19;
pdgToBin[kPdgD0] = 22;
pdgToBin[kPdgAntiD0] = 23;
pdgToBin[kPdgGamma] = 26;

//charged
pdgToBin[kPdgPiP] = 0;
pdgToBin[kPdgPiM] = 0;
pdgToBin[kPdgRhoP] = 0;
pdgToBin[kPdgRhoM] = 0;
pdgToBin[kPdgKP] = 0;
pdgToBin[kPdgKM] = 0;
pdgToBin[kPdgKStarP] = 0;
pdgToBin[kPdgKStarM] =0;
pdgToBin[kPdgDP] = 0;
pdgToBin[kPdgDM] = 0;
pdgToBin[kPdgDPs] = 0;
pdgToBin[kPdgDMs] = 0;
pdgToBin[kPdgProton] = 0;
pdgToBin[kPdgAntiProton] = 0;
pdgToBin[kPdgMuon] = 0;
pdgToBin[kPdgAntiMuon] = 0;



   	while (myReader.Next()) { //loop over events

		double E0sum = 0; //sum of Ef for all neutral particles
 
	for (int i = 0; i < myPdgf.GetSize(); i++) { //loop over final particles

        if (pdgToBin.find(myPdgf[i])->second){

		double m = myEf[i]; //m is mean value (energy center)
        double s = 0.2; //MANUAL ADJUST: height parameter - adjust based on detector accuracy??

        std::random_device rd;
        std::mt19937 gen(rd()); //generate random numbers according to mersenne twister

        std::normal_distribution<double> dist(m,s); //setting (m,s) according to desmos graph for normal dist

        E0sum += dist(gen); //generates x value (energy) near energy center based distribution, and adds to neutral energy sum.

        }

	}

//exclude all events where no neutral particles are produced (i.e. sum of neutral particle energies = 0)
    if (E0sum != 0){
	      myHist->Fill(E0sum); //fill energy sum for all neutral particles in a given event
}


}

 TH1*normh = (TH1D*)(myHist->Clone("normh"));
   normh->Scale(1./normh->GetEntries());

TFile *outfile = new TFile("kine2_1_sm.root","RECREATE"); 
outfile->cd();
normh->Write();   
outfile->Close();
}