#include "BeamTiming_main.C"
#include <iostream>
    
void test_main() {

	///////////////////////////////////////////////
	// Load BeamTiming Class
	///////////////////////////////////////////////
	gInterpreter->ProcessLine("#include \"BeamTiming_main.h\"");
    gSystem->Load("BeamTiming_main.C");

	BeamTiming bt;
	for (int irun = 1; irun <= 55; irun++) {
		TString dir;
		if (irun < 10) { dir = Form("/data3/submet_exp/e00000/tree/main/r0000%i", irun); }
		if (irun >= 10) { dir = Form("/data3/submet_exp/e00000/tree/main/r000%i", irun); }

		////////////////////////////////////////////////////////////////////////////////////////////
		// GetBeamTimings(TString directory, const int filetype, const int threshold, const int width, bool image);
		// filetype == 1 : recent version tree													  
		// filetype == 0 : previous version tree												  
		// default threshold == 1 : recommended threshold range 0.5 < threshold < 1.5			  
		// default width == 5 : choose beam timing duration. default is mean +- 5 * sigma.
		// bool image == true : save beam timing finder result as a png file.					  
		// bool image == false : not save beam timing finder result								  
		////////////////////////////////////////////////////////////////////////////////////////////
		bt.GetBeamTimings(dir, 1, 1, 5, 1);
		
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

		////////////////////////////////////////////////////////////
		// Print beam timings
		////////////////////////////////////////////////////////////
		for (int i = 0; i < bt.GetNbeams(); i++) {
			cout << "nBeams : " << bt.GetNbeams() << " | counts : " << counts . at(i) << " | arising : " << arisings . at(i) << " | peak timing : " << timings . at(i) << " | falling : " << fallings.at(i)<< endl;
		}

		////////////////////////////////////////////////
		// Check "isBeamTiming" works well
		////////////////////////////////////////////////
		if ( !file || file->IsZombie() ) { 
			std::cerr << "Error: Cannot open file: " << Form("%s/b1.root", dir.Data()) << std::endl;
			continue;
		}

		TFile *file = new TFile(Form("%s/b1.root", dir.Data()), "READ");
		TTree *tree = (TTree *) file -> Get("ch0/pulse_ch0");
		tree -> SetBranchAddress("istime", &istime);
		
		int n = tree -> GetEntries();
		double istime, zctime1;

		TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
		TH1D *h1 = new TH1D("h1", "h1", 100, 0, 4096);
		for (int i = 0; i < n; i++) {
			tree -> GetEntry(i);
			if ( bt.isBeamTiming(istime) ) { // Fill histogram if istime is in the beam timings
				h1 -> Fill(istime);
			}
		}
		h1 -> Draw();
		c1 -> SaveAs("test_figure.png", "png");
	}
}

