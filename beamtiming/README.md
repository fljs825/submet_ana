# Requirement
1. ROOT
# Manual (Execute example file)
1. `git clone https://github.com/fljs825/submet_ana.git/beamtiming`
2. `cd beamtiming`
3. `root test_main.root`
# Description
## Methods
### Class Declaration : BeamTiming bt;
- ### `bt.GetBeamTimings(TString dir, const int filetype = 1, const int threshold = 1, const int width = 5, bool figures = false);`

Find beam timings.

  - `TString dir` : path to the directory for a corresponding run

  - `const int filetype` :

    - `const int filetype = 0` : tree structure : `ch0/pul_ch0`. non-istime version.
   
    - `const int filetype = 1` : tree structure : `ch0/pulse_ch0`. istime-valid version.

  - `const int threshold` : adjust beam finding threshold. Recommended : 0.5 < threshold < 1.5.

  - `const int width` : adjust beam duration. default beam duration = mean +- 5 * sigma. This argument adjusts n such that beam duration = mean +- n * sigma.

  - `bool figures` : choose to save beam finding results as a png file. 

    - `bool figures = true` : save figures. 

    - `bool figures = false` : not save figures.

  - `TString runname` : set the title of a png file.

Example : 
```
BeamTiming bt;
bt.GetBeamTimings("/path/to/r00001/", 1, 1, 5, false);
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

