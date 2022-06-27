//Generate the plots for gamma, incoming neutrino, outgoing neutrino, and recoil in the COM frame for any given event.

#include "TFile.h"
#include <string>
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TH1D.h"
#include "TVector3.h"
#include "TLorentzVector.h"

void exercise9b(){

//open file and create 1D histogram

	//auto myHist = new TH1D("hist","Gamma Energy;Energy;Count",100,0,1.5);
	//auto myHist = new TH1D("hist","Recoil Energy;Energy;Count",100,37.2,37.4);
	auto myHist = new TH1D("hist","Initial Neutrino Energy;Energy;Count",100,0,5);
	//auto myHist = new TH1D("hist","Final Neutrino Energy;Energy;Count",100,0,5);

	TFile *myFile = TFile::Open("/Users/ishaanvohra/Desktop/Neutrino/numu_on_argon.gst.root");

//use Ttree reader to find 
// pxv, pyv, pzv, --> neutrino initial momentum vector
// pxl, pyl, pzl, --> neutrino final momentum vector
// Ev --> energy of incoming neutrino
// El --> energy of final neutrino
// pdgf --> final PDG code of hadronic system, Ef --> final energy in hadronic system
// pxf, pyf, pzf --> final momentum vector of hadronic system 

   	TTreeReader myReader("gst", myFile);

   	TTreeReaderValue<double> myPxv(myReader, "pxv");
   	TTreeReaderValue<double> myPyv(myReader, "pyv");
   	TTreeReaderValue<double> myPzv(myReader, "pzv");

  	TTreeReaderValue<double> myPxl(myReader, "pxl");
  	TTreeReaderValue<double> myPyl(myReader, "pyl");
  	TTreeReaderValue<double> myPzl(myReader, "pzl");

	TTreeReaderValue<double> myEv(myReader, "Ev");

	TTreeReaderValue<double> myEl(myReader, "El");

  	TTreeReaderArray<int> myPdgf(myReader, "pdgf");
  	TTreeReaderArray<double> myEf(myReader, "Ef");

  	TTreeReaderArray<double> myPxf(myReader, "pxf");
  	TTreeReaderArray<double> myPyf(myReader, "pyf");
  	TTreeReaderArray<double> myPzf(myReader, "pzf");


	while (myReader.Next()) {

 //Calculate beta for a given event

		//define doubles and vectors outside of for loop and if function (since beta calculation needs both for a given event in myReader)

		double E_gamma;
		double Px_gamma;
		double Py_gamma;
		double Pz_gamma;
		double E_recoil;
		double Px_recoil;
		double Py_recoil;
		double Pz_recoil;
		TLorentzVector v_gamma;
		TLorentzVector v_nui(*myPxv, *myPyv, *myPzv, *myEv);
		TLorentzVector v_nuf(*myPxl, *myPyl, *myPzl, *myEl);
		TLorentzVector v_recoil;


		for (int i = 0; i < myPdgf.GetSize(); i++) { //the size of myPdgf is just 2 - it switches back and forth between gamma and nucleus.

			if ( myPdgf[i] == 22 ) {

				E_gamma = myEf[i];
				Px_gamma = myPxf[i];
				Py_gamma = myPyf[i];
				Pz_gamma = myPzf[i];

				v_gamma.SetXYZT(Px_gamma, Py_gamma, Pz_gamma, E_gamma);

			} else if ( myPdgf[i] == 1000180400 ) {

				E_recoil = myEf[i];
				Px_recoil = myPxf[i];
				Py_recoil = myPyf[i];
				Pz_recoil = myPzf[i];

				v_recoil.SetXYZT(Px_recoil, Py_recoil, Pz_recoil, E_recoil);
			
			}

		}



	TLorentzVector v_tot = v_nuf + v_gamma + v_recoil; //total momentum vector

	double E_tot = *myEl+E_gamma+E_recoil; // total energy

	TVector3 beta = -(1/E_tot*(v_tot.Vect())); //beta is 1/E * 3-vector component of v_tot (which itself is the total momentum 3-vector)
												//Must use -beta instead of beta because of weird formula 


//boost v_tot to check we are in COM (if v_tot's vector part = 0)
	v_tot.Boost(beta); 

//boost other vectors
	v_gamma.Boost(beta); 
	v_recoil.Boost(beta); 
	v_nui.Boost(beta);
	v_nuf.Boost(beta);

	 //Fill histogram

	// myHist->Fill(v_gamma.E());

	// myHist->Fill(v_recoil.E());

	 myHist->Fill(v_nui.E());

	// myHist->Fill(v_nuf.E());


	}

	myHist->Draw();

}














