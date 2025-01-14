# Requirement
1. ROOT
# Manual (Execute example file)
1. `git clone https://github.com/fljs825/submet_ana.git/beamtiming`
2. `root test_main.root`
# Description
## Methods
### Class Declaration : BeamTiming bt;
### bt.GetBeamTimings(TString dir, bool figures = false, TString runname);
`TString dir` : path to the directory for a corresponding run

`bool figures` : choose to save beam finding results as a png file. 

- `bool figures = true` : save figures. 

- `bool figures false` : not save figures.

`TString runname` : set the title of a png file.

Example : 
```
BeamTiming bt;
bt.GetBeamTimings("/path/to/r00001/", true, "r00001");
```
### `std::vector<double> bt.GetTimings();`
### `std::vector<double> bt.GetCounts();`
### `std::vector<double> bt.GetArisings();`
### `std::vector<double> bt.GetFallings();`


### `std::vector<double> BeamTiming::fallings`
### `std::vector<double> BeamTiming::Arisings`
### 
