void timingtree(const char* input = "rb06_ch00010203_sine.dat", unsigned int intChMask = 15)
{
	unsigned int nEvent    =	0;
	unsigned int sigStart  =  100;
	unsigned int sigEnd    = 4000;
	unsigned int boardType =    1;
	unsigned int Nch       =    0;

	int nPulse = 98;
	
	// Inspect board type
	if ( boardType == 0 | boardType == 1 )
	{
		if      ( boardType == 0 ) Nch =  4;
		else if ( boardType == 1 ) Nch = 16;
	}

	// Interpret channel masking
	bool chMask[Nch];
	int enabledChs = 0;

	if ( !intChMask )
	{
		if      ( boardType == 0 ) intChMask =    15;
		else if ( boardType == 1 ) intChMask = 65535;
	}

    if ( intChMask < 1 || intChMask > std::pow(2, Nch) - 1 )
	{
		std::cerr << "Error: " << intChMask << " is invalid for channel mask." << std::endl;
	}
	else
	{
		for ( unsigned short int ch = 0; ch < Nch; ch++ )
		{
			// Shift 1 by ch positions and perform bitwise AND to check if the bit is set
			 chMask[ch] = (intChMask >> ch) & 1;
			 if ( chMask[ch] ) enabledChs++;
		}
	}

	std::cout << "# of enabeld channels are " << enabledChs << std::endl;

	// Check the number of events to be processed
	unsigned long file_size;
	FILE* fp;
	fp = fopen(input, "rb");
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	if ( !nEvent ) // if nEvent is not defined use the maximum number of events recorded
	{
		if      ( boardType == 0 ) nEvent = int(file_size /  32768);
		else if ( boardType == 1 ) nEvent = int(file_size / 131072);
	}

	//------------------------------------------------
	// Print configuration
	//------------------------------------------------
	std::cout << "[ Run Configuration ]"                        << std::endl;
	std::cout << "  Board type                 : " << boardType << std::endl;
	std::cout << "  Input file name            : " << input     << std::endl;
	std::cout << "  # of events to be processed: " << nEvent    << std::endl;
	if      ( boardType == 0 )
	{
		std::cout << "  Channel mask = {"
			      << chMask[0] << ", "
			      << chMask[1] << ", "
			      << chMask[2] << ", "
			      << chMask[3] << "}" << std::endl;
	}
	else if ( boardType == 1 )
	{
		std::cout << "  Channel mask = {"
			      << chMask[ 0] << ", "
			      << chMask[ 1] << ", "
			      << chMask[ 2] << ", "
			      << chMask[ 3] << ", "
			      << chMask[ 4] << ", "
			      << chMask[ 5] << ", "
			      << chMask[ 6] << ", "
			      << chMask[ 7] << ", "
			      << chMask[ 8] << ", "
			      << chMask[ 9] << ", "
			      << chMask[10] << ", "
			      << chMask[11] << ", "
			      << chMask[12] << ", "
			      << chMask[13] << ", "
			      << chMask[14] << ", "
			      << chMask[15] << "}" << std::endl;
	std::cout << "  Region of interest         : [" << sigStart << ", " << sigEnd << "]" << std::endl;
	}

	//------------------------------------------------
	// Multi processing
	//------------------------------------------------
	int ch;
	pid_t pid1, pid2, pid3, pid4;

	pid1 = fork();
	if      ( pid1 >  0 ) ch =  0;
	else if ( pid1 == 0 ) ch =  1;

	pid2 = fork();
	if      ( pid2 == 0)  ch += 2;

	pid3 = fork();
	if ( pid3 == 0 ) ch += 4; 

	pid4 = fork();
	if ( pid4 == 0 ) ch += 8;

	//------------------------------------------------
	// Read data points
	//------------------------------------------------
	// Local variables for data acquisition
	const double eventNorm = sigEnd - sigStart + 1;

	const char* inputf = input;

	if ( !chMask[ch] ) { exit(0); };

	TTree *tree = new TTree(Form("tree_ch%d", ch), Form("tree_ch%d", ch));

	int ch_tree, event_tree, pulseN;
	double timing_tree;

	tree -> Branch("ch", &ch_tree);
	tree -> Branch("ievt", &event_tree);
	tree -> Branch("pulseN", &pulseN);
	tree -> Branch("t", &timing_tree);

	SUBMETDecoder* decoder = new SUBMETDecoder(inputf); 

	decoder -> SetBoard(boardType); // '1' is for fullscale, '0' for proto2
	decoder -> SetMaxN(nEvent);     // Set the maximum number of events

	ch_tree = ch;
	decoder -> SetCh(ch);

	std::vector<unsigned short> times, volts;
	std::vector<int> times_converted;
	std::vector<int> volts_converted;
	for (int ievt = 0; ievt < nEvent; ievt++)
	{

		event_tree = ievt;

		decoder -> GetEvent(times, volts);

		times_converted . reserve(times . size());
		volts_converted . reserve(volts . size());
		for ( unsigned short int i = 0; i < times . size(); i ++ )
		{
			times_converted[i] = times . at(i);
			volts_converted[i] = volts . at(i);
		}
		//cout << ch << " | " <<  ievt << " | " << volts . at(10) << endl;
		//cout << ch << " | " <<  ievt << endl;

		// Graph
		TGraph* gr = new TGraph(times . size(), &times_converted[0], &volts_converted[0]);

		///////////////////////////////////////////////////////
		// Fitting ( sine function )
		///////////////////////////////////////////////////////

		//////////////////////////////////////////////////////
		// Repeatedly Partial Fit
		//////////////////////////////////////////////////////

		for (int i = 0; i < 98; i++) {
			TF1 *fSine = new TF1("f1", "[0] * sin ([1]*x + [2]) + [3]", 110 + 40 * i, 150 + 40 * i);

			fSine -> SetParameter(0, 600);
			fSine -> SetParameter(1, 2 * TMath::Pi() / 40);
			fSine -> SetParameter(3, 3600); 

			fSine -> SetParLimits(0, 400, 800);
			fSine -> SetParLimits(1, 0.1, 0.2);
			gr -> Fit(fSine, "R+Q");

			double minPos = fSine -> GetMinimumX(); 

			timing_tree = minPos;
			pulseN = i;
			tree -> Fill();
			//cout << "ch : " << ch_tree << " | ievt : " << ievt << " | nPulse : "  << i <<   " | val : " << minPos << endl;
			delete fSine;
		}
		delete gr;

		if ( ievt >= 100 && ievt % 100 == 0 ) std::cout << ievt << "-th event processed...\r" << std::flush;
	}

	delete decoder;

	TFile* output = new TFile(Form("timingtree_ch%d.root", ch), "recreate");

	tree -> Write();
	//cout << Form("Ch%i complete", ch) << endl;
	output -> Close();
	delete tree;

	//exit(0);

	//------------------------------------------------
	// Wait for all direct child processes to finish
	//------------------------------------------------
	// Child processes of the fourth fork must be finished firstly for prohibiting being zombie processees.   
	if ( ch > 8 && ch < 16 )
	{
		exit(0);
	}

	// Child processes of the third fork are finished after the fourth child processes.
	if ( ch > 3 && ch < 8 )
	{
		int status;
		waitpid(pid4, &status,  0); //wait for the forth child processes.
		exit(0);
	}

	// Child processes of the second fork are finished after the third child processes and the fourth child processes.
	if ( ch == 2 || ch == 3 )
	{
		int status, status1;
		waitpid(pid3, &status,  0); 
		waitpid(pid4, &status1, 0); 
		exit(0);
	}

	// Child processes of the first fork are finished after the second child processes, the third child processes and the fourth child processes.
	if ( ch == 1 )
	{
		int status, status1, status2;
		waitpid(pid2, &status,  0); 
		waitpid(pid3, &status1, 0); 
		waitpid(pid4, &status2, 0); 
		exit(0);
	}

	// A parent process is finished after all the child processes.  
	if ( ch == 0 )
	{
		int status, status1, status2, status3;
		waitpid(pid1, &status,  0); 
		waitpid(pid2, &status1, 0); 
		waitpid(pid3, &status2, 0); 
		waitpid(pid4, &status3, 0); 
	}

	cout << "Tree is constructed" << endl;

}
