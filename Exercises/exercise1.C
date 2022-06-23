//list number of entries in TTree

void exercise1()
{

	TFile *input = new TFile("final_numu.gst.root", "read");

	TTree *gst = (TTree*)input->Get("gst");

	int entries = gst->GetEntries();

	cout << entries <<endl;

}