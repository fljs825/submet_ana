#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TString.h>
#include <iostream>

void process(TString input = "timingtree.root") {

	TFile *file = new TFile(input);

	std::vector<std::vector<std::vector<double>>> t(4, std::vector<std::vector<double>>(25000, std::vector<double>(98)));

	int nEvent = 0;
	for (int ch = 0; ch < 4; ch++) {
		TTree *tree = (TTree *)file->Get(Form("tree_ch%i", ch));
		double timing;

		tree->SetBranchAddress("t", &timing);  

		int nEntries = tree->GetEntries();
		nEvent = nEntries / 98;

		std::cout << "Entries for ch" << ch << ": " << nEntries << std::endl;
		//		cout << nEvent << endl;
		for (int i = 0; i < nEntries; i++) {
			tree->GetEntry(i);
			int pulseN = i % 98;
			int ievt = i / 98;
			t[ch][ievt][pulseN] = timing;
			//cout << "ch : " << ch << " | ievt : " << ievt << " | pulseN : " << pulseN << " | timing : " << t[ch][ievt][pulseN] << endl;
		}
		delete tree;
	}

	TCanvas *c1 = new TCanvas("c1", "c1", 1600, 1200);
	c1->Divide(3, 2);

	int cnt = 1;
	// double diff = 0;
	for (int ch1 = 0; ch1 < 4; ch1++) {
		for (int ch2 = ch1 + 1; ch2 < 4; ch2++) {

			TString sTemp = Form("ch%i & ch%i", ch1 + 1, ch2 + 1);
			TH1D *h1 = new TH1D(sTemp, sTemp, 100, -5, 5);

			for (int evt = 0; evt < nEvent; evt++) {
				for (int npeak = 0; npeak < 98; npeak++) {
					double diff = t[ch1][evt][npeak] - t[ch2][evt][npeak];
					//cout << diff << endl;
					h1->Fill(diff);
				}
			}

			c1 -> cd( cnt );
			h1 -> Draw();
			//cout << cnt << endl;
			cnt += 1;

			//delete h1;
		}
	}

	c1 -> Draw();
	c1 -> SaveAs("dt_distribution.png", "png");

	gApplication -> Run();

	file -> Close();
}

