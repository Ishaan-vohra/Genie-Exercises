//plot Ev on a histogram and draw it

#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TH1D.h"



void exercise4()
{

	auto myHist = new TH1D("h1","Neutrino Energies",100,0.,5.);

	TFile *myFile = TFile::Open("final_numu.gst.root");

   	TTreeReader myReader("gst", myFile);

  	TTreeReaderValue<double> myEv(myReader, "Ev");

   	while (myReader.Next()) {
 
      myHist->Fill(*myEv);

	}

myHist->Draw();

}