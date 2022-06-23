//print out all the Ev values inside gst


#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

void exercise3()
{

   TFile *myFile = TFile::Open("final_numu.gst.root");

   TTreeReader myReader("gst", myFile);

   TTreeReaderValue<double> myEv(myReader, "Ev");

   while (myReader.Next()) {
      
      
      std::cout << *myEv << std::endl;
   }
}