//Deflection angle vs incoming neutrino energy in degrees 2d histogram

#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TH2D.h"
 

void exercise7()
{


	auto myHist = new TH2D("hist","Deflection angle vs Incoming Neutrino Energy; Deflection angle [Radians]; Incoming Neutrino Energy [GeV]",100,0.,200.,100,0.,6.);

	TFile *myFile = TFile::Open("numu_on_argon.gst.root");

   	TTreeReader myReader("gst", myFile);

   	TTreeReaderValue<double> myEv(myReader, "Ev");
  	TTreeReaderValue<double> myPxl(myReader, "pxl");
  	TTreeReaderValue<double> myPyl(myReader, "pyl");
  	TTreeReaderValue<double> myPzl(myReader, "pzl");

  	

   	while (myReader.Next()) {
 	
 	double theta_deg = 180/3.14159265*acos((*myPzl)/(sqrt(pow(*myPxl,2)+pow(*myPyl,2)+pow(*myPzl,2))));
      myHist->Fill(theta_deg, *myEv);

	}

myHist->Draw("COLZ");

}