//draw 2d neutrino energies vs Q^2 on a histogram

#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TH2D.h"

void exercise6()
{


	auto myHist = new TH2D("hist","Neutrino Energy vs Q^2; Energy; Q^2",100,0.,6., 100, 0., 0.5);

	TFile *myFile = TFile::Open("numu_on_argon.gst.root");

   	TTreeReader myReader("gst", myFile);

  	TTreeReaderValue<double> myEv(myReader, "Ev");
  	TTreeReaderValue<double> myQ2(myReader, "Q2");

   	while (myReader.Next()) {
 
      myHist->Fill(*myEv, *myQ2);

	}

myHist->Draw("COLZ");

}