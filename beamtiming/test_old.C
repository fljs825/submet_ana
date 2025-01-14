#include "BeamTiming_main.C"
#include <iostream>
    
void test_old() {

	///////////////////////////////////////////////
	// Load BeamTiming Class
	///////////////////////////////////////////////
	gInterpreter->ProcessLine("#include \"BeamTiming_main.h\"");
    gSystem->Load("BeamTiming_main.C");

	BeamTiming bt;
	for (int irun = 8; irun <= 8; irun++) {
		TString dir;
		if (irun < 10) { dir =  Form("/Users/fljs825/research/submet/beamTiming/r0000%i", irun); }
		if (irun >= 10) { dir =  Form("/Users/fljs825/research/submet/beamTiming/r000%i", irun); }

		////////////////////////////////////////////////////////////////////////////////////////////
		// GetBeamTimings(TString directory, const int filetype, const int threshold, bool image);
		// filetype == 1 : recent version tree													  
		// filetype == 0 : previous version tree												  
		// default threshold == 1 : recommended threshold range 0.5 < threshold < 1.5			 
		// default width == 5 : choose beam timing duration. mean +- width * sigma. dafault : mean +- 5 * sigma.
		// bool image == true : save beam timing finder result as a png file.					  
		// bool image == false : not save beam timing finder result								  
		////////////////////////////////////////////////////////////////////////////////////////////
		bt.SetNsigma(5);	
		bt.GetBeamTimings(dir, 0, 1, true); 
		////////////////////////////////////////////////////////////////////////////////////////////
		// bt.GetNbeams() returns the number of discovered beams.
		////////////////////////////////////////////////////////////////////////////////////////////
		std::cout << bt.GetNbeams() << " beams are found" << std::endl;

		////////////////////////////////////////////////////////////////////////////////////////////
		// bt.GetTimings(), bt.GetCounts(), bt.GetArisings(), bt.GetFallings() example
		////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<double> timings = bt.GetTimings();
		std::vector<double> counts = bt.GetCounts();
		std::vector<double> arisings = bt.GetArisings();
		std::vector<double> fallings = bt.GetFallings();

		for (int i = 0; i < bt.GetNbeams(); i++) {
			cout << "iBeams : " << i << " | counts : " << counts . at(i) << " | arising : " << 
				arisings . at(i) << " | peak timing : " << timings . at(i)  << " | falling : "
				<< fallings.at(i)<< endl;
		}

		////////////////////////////////////////////////
		// Check "isBeamTiming" works well
		////////////////////////////////////////////////
		TFile *file = new TFile(Form("%s/board1.root", dir.Data()), "READ");
		if ( !file || file->IsZombie() ) { 
			std::cerr << "Error: Cannot open file: " << Form("%s/board1.root", dir.Data()) << std::endl;
			continue;
		}

		TTree *tree = (TTree *) file -> Get("ch0/pul_ch0");

		int n = tree -> GetEntries();

		double istime, zctime1;
		tree -> SetBranchAddress("ZCtime1", &zctime1);

		TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
		TH1D *h1 = new TH1D("h1", "h1", 100, 0, 4096);
		for (int i = 0; i < n; i++) {
			tree -> GetEntry(i);
			if ( bt.isBeamTiming(zctime1) ) { // Fill histogram if zctime1 is in the beam timings.
				h1 -> Fill(zctime1);
			}
		}
		h1 -> Draw();
		c1 -> SaveAs("test_figure.png", "png");
	}
}

