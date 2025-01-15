# Requirement
1. ROOT
# Manual (Execute example file)
1. `git clone https://github.com/fljs825/submet_ana.git/beamtiming`
2. `cd beamtiming`
3. `root test_main.root`
# Description
## Methods
### Class Declaration : BeamTiming bt;
- ### `void bt.GetBeamTimings(TString dir, const int filetype = 1, double threshold = 1, bool figures = false);`

-  `void bt.GetBeamTimings(...) searches the beam timings.

  - `TString dir` : path to the directory for a corresponding run

  - `const int filetype` :

    - `const int filetype = 0` : For the old version trees. (`istime` is invalid so that used `zctime1` to find pulse timings)
   
    - `const int filetype = 1` : For the current version pules-based trees. `istime` is valid.
   
    - `const int filetype = 2` : For the current version event-based trees.

  - `const int threshold` : adjust beam finding threshold. Recommended : 0.5 < threshold < 1.5.

  - `bool figures` : choose to save beam finding results as a png file. 

    - `bool figures = true` : save figures. 

    - `bool figures = false` : not save figures.

Example : 
```
BeamTiming bt;
bt.GetBeamTimings("/path/to/r00001/", 1, 1, false);
```

* GetBeamTimings() operates without some missing arguments.
Example :
```
BeamTiming bt;
bt.GetBeamTimings("/path/to/r00001"); // Default filetype : 1, threshold : 1, width : 5, save figures : false
```

- ### `std::vector<double> bt.GetTimings();`

  - returns peak timings of the beams as a vector.

Example :
```
std::vector<double> timings;
timings = bt.GetTimings();
cout << timings . at(0) << endl;
```
Gives the first beam's peak timing. 

- ### `std::vector<double> bt.GetCounts();`

returns the number of large pulses at a given pulse timing.

Example : 
```
std::vector<double> counts;
counts = bt.GetCounts();
cout << counts . at(0) << endl;
```
Gives the number of the first beam's large pulses. 

- ### `std::vector<double> bt.GetArisings();`

returns timing at $m - 5\sigma$

- ### `std::vector<double> bt.GetFallings();`

returns timing at $m + 5\sigma$

- ### `bool bt.isBeamTiming(time)`

If input time is in the beam time window, returns `true`. Else, returns `false`

Example :

  - Input : 

    ```
    time = 350; // Let t = 350 is in the beam time window
    cout << bt.isBeamTiming(time); << endl;
    ```

  - Output :

    ```
    1
    ```

