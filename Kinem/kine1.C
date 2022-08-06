//neutrino energy balance, without energy smearing

#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TH1D.h"
#include "TH2.h"

#include <iostream>
#include <string>

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

void kine1()
{
	

	auto myHist = new TH1D("h1","Energy Balance;ratio of final energy to initial energy;probability",100,0.965,1.);

	TFile *myFile = TFile::Open("/Users/ishaanvohra/Desktop/Neutrino/numu_on_argon.gst.root");

   	TTreeReader myReader("gst", myFile);

// pdgf and pdgi

TTreeReaderArray<int> myPdgf(myReader, "pdgf");
TTreeReaderArray<int> myPdgi(myReader, "pdgi");


// initial energies
  	TTreeReaderValue<double> myEv(myReader, "Ev"); //incoming neutrino nrg
	TTreeReaderValue<double> myEn(myReader, "En"); //initial state hit nucleon nrg
	TTreeReaderArray<double> myEi(myReader, "Ei"); //energy of kth initial state particle in hadronic system

// final energies
	TTreeReaderValue<double> myEl(myReader, "El"); //final state primary lepton nrg
	TTreeReaderArray<double> myEf(myReader, "Ef"); //energy of kth final state particle in hadronic system

   	while (myReader.Next()) {

		double Efsum = 0; //sum of Ef for all particles
		double Eisum = 0; //sum of Ef for all particles
 
	for (int i = 0; i < myPdgf.GetSize(); i++) {

		Efsum += myEf[i];

	}

	for (int i = 0; i < myPdgi.GetSize(); i++) {

		Eisum += myEi[i];

	}

	      myHist->Fill((Efsum+*myEl)/(Eisum+*myEv+*myEn));

}

 TH1*normh = (TH1D*)(myHist->Clone("normh"));
   normh->Scale(1./normh->GetEntries());

normh->Draw();
}