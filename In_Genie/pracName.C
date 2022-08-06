// practice getting PDG name from code

#include <iostream>
#include <string>

#include "GHepParticle.h"
#include "ScatteringType.h"
#include "ProcessInfo.h"
#include "GHepParticle.h"
#include "TObjArray.h"
#include "PDGLibrary.h"

#include "TString.h" 
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TAxis.h"

#include "NtpMCTreeHeader.h" 
#include "NtpMCEventRecord.h" 
#include "EventRecord.h" 
#include "BaryonResUtils.h"

#include "GHepParticle.h"       
#include "PDGUtils.h"
#include "GHepStatus.h"

#include "PDGCodes.h"
#include "TDatabasePDG.h"

void pracName()
{

    int particleCode = 211;
    TDatabasePDG * db = TDatabasePDG::Instance();
    TParticlePDG * part = db->GetParticle(particleCode);


    std::cout << part->GetName() << std::endl; 


}

