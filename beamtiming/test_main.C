#include "BeamTiming_main.C"
#include <iostream>
    
void test_main() {

	///////////////////////////////////////////////
	// Load BeamTiming Class
	///////////////////////////////////////////////
	gInterpreter->ProcessLine("#include \"BeamTiming_main.h\"");
    gSystem->Load("BeamTiming_main.C");

	BeamTiming bt;
	for (int irun = 40; irun <= 56; irun++) {
		TString dir = Form("/data3/submet_exp/e00000/tree/main/r000%i", irun);
		bt.GetBeamTimings(dir, 1, Form("r000%i", irun));
		std::cout << bt.GetNbeams() << " beams are found" << std::endl;

		std::vector<double> timings = bt.GetTimings();
		std::vector<double> counts = bt.GetCounts();
		std::vector<double> arisings = bt.GetArisings();
		std::vector<double> fallings = bt.GetFallings();

		//for (int i = 0; i < bt.GetNbeams(); i++) {
		//	cout << "timing : " << timings . at(i) << " | counts : " << counts . at(i) << " | arising : " << arisings . at(i) << " | falling : " << fallings.at(i)<< endl;
		//}

		////////////////////////////////////////////////
		// Check "isBeamTiming" works well
		////////////////////////////////////////////////
		TFile *file = new TFile(Form("%s/b1.root", dir.Data()), "READ");
		if ( !file || file->IsZombie() ) { 
			std::cerr << "Error: Cannot open file: " << Form("%s/b1.root", dir.Data()) << std::endl;
			continue;
		}

		TTree *tree = (TTree *) file -> Get("ch0/pulse_ch0");

		int n = tree -> GetEntries();

		double istime, zctime1;
		tree -> SetBranchAddress("istime", &istime);
		//tree -> SetBranchAddress("ZCtime1", &zctime1);

		for (int i = 0; i < n; i++) {
			tree -> GetEntry(i);
			if ( i % 1000 == 0 && bt.isBeamTiming(istime) ) {
				cout << istime << endl;
			}
		}
	}
}

