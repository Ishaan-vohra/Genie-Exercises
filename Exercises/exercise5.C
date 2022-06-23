//draw neutrino energies on a histogram

#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TH1D.h"

void exercise5()
{
	

	auto myHist = new TH1D("h1","Neutrino Energies",100,0.,5.);

	TFile *myFile = TFile::Open("numu_on_argon.gst.root");

   	TTreeReader myReader("gst", myFile);

  	TTreeReaderValue<double> myEv(myReader, "Ev");

   	while (myReader.Next()) {
 
      myHist->Fill(*myEv);

	}

myHist->Draw();

}