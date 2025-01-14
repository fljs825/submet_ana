# Requirement
1. ROOT
# Manual (Execute example file)
1. `git clone https://github.com/fljs825/submet_ana.git/beamtiming`
2. `root test_main.root`
# Description
## Methods
### Class Declaration : BeamTiming bt;
### bt.GetBeamTimings(TString dir, bool figures = false, TString runname);
`TString dir` : path to the directory for corresponding run. example : `/path/to/r00001`
`bool figures` : choose to save beam finding results as a png file.
`TString runname` : set the title of a png file.
### `std::vector<double> bt.GetTimings();`
### `std::vector<double> bt.GetCounts();`
### `std::vector<double> bt.GetArisings();`
### `std::vector<double> bt.GetFallings();`


### `std::vector<double> BeamTiming::fallings`
### `std::vector<double> BeamTiming::Arisings`
### 
