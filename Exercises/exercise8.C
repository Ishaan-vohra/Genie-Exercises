//Generalized to any incoming particle direction, find E_gamma, cos(theta_nu), cos(theta_gamma), phi (angle between outgoing neutrino and gamma), and plot all 1D, 2D, and 3D combos

#include "TFile.h"
#include <string>
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TVector3.h"

void exercise8()
{

//Create histogram (4x 1D, 6x 2D, 4x 3D)
int dimension;
string x_axis, y_axis, z_axis;
cout << "1D, 2D or 3D? Enter 1, 2, or 3. " << endl;
cin >> dimension;

auto myHist1 = new TH1D("myHist1","Histogram",50,-1,6);
auto myHist2 = new TH2D("myHist2","Histogram",50,-1,6, 50,-1,6);
auto myHist3 = new TH3D("myHist3","Histogram",50,-1,6, 50,-1,6, 40, -1, 6);

if (dimension == 1){

cout << "X-axis variable? Enter E_gamma, cos_theta_nu, cos_theta_gamma, or phi." << endl;
cin >> x_axis;

const char *ax = x_axis.c_str();

myHist1->GetXaxis()->SetTitle(ax);

} else if (dimension == 2){

cout << "X-axis variable? Enter E_gamma, cos_theta_nu, cos_theta_gamma, or phi." << endl;
cin >> x_axis;

cout << "Y-axis variable? Enter E_gamma, cos_theta_nu, cos_theta_gamma, or phi." << endl;
cin >> y_axis;

const char *ax = x_axis.c_str();

myHist2->GetXaxis()->SetTitle(ax);

const char *ay = y_axis.c_str();

myHist2->GetYaxis()->SetTitle(ay);

}else if (dimension == 3){

cout << "X-axis variable? Enter E_gamma, cos_theta_nu, cos_theta_gamma, or phi." << endl;
cin >> x_axis;

cout << "Y-axis variable? Enter E_gamma, cos_theta_nu, cos_theta_gamma, or phi." << endl;
cin >> y_axis;

cout << "Z-axis variable? Enter E_gamma, cos_theta_nu, cos_theta_gamma, or phi." << endl;
cin >> z_axis;

const char *ax = x_axis.c_str();

myHist3->GetXaxis()->SetTitle(ax);

const char *ay = y_axis.c_str();

myHist3->GetYaxis()->SetTitle(ay);

const char *az = z_axis.c_str();

myHist3->GetZaxis()->SetTitle(az);

}


//open file

	TFile *myFile = TFile::Open("numu_on_argon.gst.root");

//use Ttree reader to find 
// pxv, pyv, pzv, --> neutrino initial momentum vector
// pxl, pyl, pzl, --> neutrino final momentum vector
// pdgf --> final PDG code of hadronic system, Ef --> final energy in hadronic system
// pxf, pyf, pzf --> final momentum vector of hadronic system (basically just gamma momentum vector, since recoiling nucleus momentum is negligible??)

   	TTreeReader myReader("gst", myFile);

   	TTreeReaderValue<double> myPxv(myReader, "pxv");
   	TTreeReaderValue<double> myPyv(myReader, "pyv");
   	TTreeReaderValue<double> myPzv(myReader, "pzv");

  	TTreeReaderValue<double> myPxl(myReader, "pxl");
  	TTreeReaderValue<double> myPyl(myReader, "pyl");
  	TTreeReaderValue<double> myPzl(myReader, "pzl");

  	TTreeReaderArray<int> myPdgf(myReader, "pdgf");
  	TTreeReaderArray<double> myEf(myReader, "Ef");

  	TTreeReaderArray<double> myPxf(myReader, "pxf");
  	TTreeReaderArray<double> myPyf(myReader, "pyf");
  	TTreeReaderArray<double> myPzf(myReader, "pzf");

//As MyReader scans through each event, cycle through the array [0,1] i.e. the kth particle.
//Whenever pdgf[k] = 22, set E_gamma equal to the corresponding Ef (i.e. Ef with the same k in the given event)
//and fill.
  
		double E_gamma;
		double Px_gamma;
		double Py_gamma;
		double Pz_gamma;


	while (myReader.Next()) {

//cycle through array and test for pdgf[k]=22, but treat array thingies like they're not pointers.

		for (int i = 0; i < myPdgf.GetSize(); i++) {
  			
			if ( myPdgf[i] == 22 ) {

				E_gamma = myEf[i];
				Px_gamma = myPxf[i];
				Py_gamma = myPyf[i];
				Pz_gamma = myPzf[i];

				TVector3 p_gamma(Px_gamma, Py_gamma, Pz_gamma);
				TVector3 p_nui(*myPxv, *myPyv, *myPzv);
				TVector3 p_nuf(*myPxl, *myPyl, *myPzl);

				double cos_theta_nu = cos(p_nui.Angle(p_nuf));
				double cos_theta_gamma = cos(p_nui.Angle(p_gamma));

//projection on perpendicular plane = final vector - (scaled magnitude of final vector)(unit vector in initial neutrino direction)

				TVector3 proj_nuf;
				TVector3 proj_gamma;

				proj_nuf = p_nuf-(((p_nuf.Mag())*cos_theta_nu)*(p_nui.Unit()));
				proj_gamma = p_gamma-(((p_gamma.Mag())*cos_theta_gamma)*(p_nui.Unit()));

				double phi = (proj_nuf.Angle(proj_gamma));

//arrays
				double variables[4] = {E_gamma, cos_theta_nu, cos_theta_gamma, phi};
				std::string variable_names[4] = {"E_gamma", "cos_theta_nu", "cos_theta_gamma", "phi"};

//plot the correct quantities (option by option...)

				if (dimension == 1){

				for (int j=0; j < 4; j++){
					if (x_axis == variable_names[j]) {
						myHist1->Fill(variables[j]);
					}

				}

				} else if (dimension == 2){

				for (int k=0; k < 4; k++){

					for(int l=0; l < 4; l++){

					if(x_axis == variable_names[k] && y_axis == variable_names[l]){
							myHist2->Fill(variables[k], variables[l]);
						}
					}
				}

				}else if (dimension == 3){


				for (int m=0; m < 4; m++){
					for (int n=0; n < 4; n++){
						for (int p=0; p < 4; p++){

							if(x_axis == variable_names[m] && y_axis == variable_names[n] && z_axis == variable_names[p]){
								myHist3->Fill(variables[m], variables[n], variables[p]);
							}
						}
					}
				  }
				}
			} 
		}
	}
	
	if (dimension == 1){

	myHist1->Draw();

	} else if (dimension == 2){

	myHist2->Draw("COLZ");

	}else if (dimension == 3){

	myHist3->Draw("BOX2Z");

	}		

}





