# VBF Invisible Higgs Analysis Code

*This is a rather quick README, designed to get you off the ground quickly, but could do with some more explanation in areas!*

## Setup

You will need a github account, and link it to CMS. [FAQ here] (http://cms-sw.github.io/cmssw/faq.html)

Set some global config parameters:

    git config --global user.name <First Name> <Last Name>
    git config --global user.email <Your-Email-Address>
    git config --global user.github <Your-Just-Created-GitHub-Account>

Now clone the git repository. Note, you will need a machine that can run CMSSW (e.g. lxplus)

    scram p CMSSW CMSSW_5_3_11_patch5
    cd CMSSW_5_3_11_patch5/src
    cmsenv
    git init
    git clone https://github.com/jimbrooke/InvisibleHiggs
    scram b

This will check out and compile the complete Analysis and PAT/ntuple code. For now, we're only interested in the Analysis code.

## Running analysis programs

Running programs is easy:

    <program name> -i <location of ntuples> -o <output folder> -l <luminosity> -f <location of dataset list>

The program source files are stored in `Analysis/bin`. If you edit a program file, make sure you run `scram b` (in /Analysis) to re-compile.

## Contributing to the repository

Always check for the latest pushes by other people by updating your local version:
    
    git status
    git pull

If you want to commit your changes to the remote repository (not just your local one!) then do:
    
    git add <folder/filename> # You need to tell git to stage a file before you can commit
    git commit -m "Your message here!" # This will commit changes in **staged** files to your *local* branch
    git push origin master # This will push your changes to the remote master branch

To understand git's workflow (which is somewhat different to CVS!), read the first few chapters of the [ProGit book](http://git-scm.com/book). Also handy are these cheatsheets: [PDF cheatsheet](https://na1.salesforce.com/help/doc/en/salesforce_git_developer_cheatsheet.pdf) and [interactive reminder](http://ndpsoftware.com/git-cheatsheet.html). The git website offers lots of help and tutorials.

## Folder structure/purpose

* `bin` - contains program source code. `bin/BuildFile.xml` is the list of programs that will be compiled when you run `scram b`. If you write a new program, you will need to add it to this file if you want it to compile.

* `data` - stores files with lists of datasets. Currently use v11.

* `interface` - header files for helper objects

* `scripts` - contains scripts to run complete analysis and limits

* `src` - source code for helper objects

## General program structure

Although this varies from program to program, typically we do something like (see `bin/example.cpp`):

- get command line options (input dir, output dir, lumi, etc)
- load in datasets file
- declare some histograms and general cuts 
- loop over datasets in dataset file
- in the loop, create some temp histograms, setup some specific cuts, fill hists using `tree->Draw("variableName>>histName",someCuts)`
- add temp hists together
- after loop, calculate and print out important values, save plots to PDF, and save hists to ROOT file

## A brief explanation of helper objects

* `Constants` - this stores constants like BG systematics, so that a consistent value is used in various programs
* `Cuts` - this contains signal, Z, W and WTau cuts, along with pre-defined sets of cuts and cutflow methods
* `Datasets` - used to read in file with list of datasets, load a given dataset file. Note that dataset files typically have the structure:


        <ntuple file name>    <number of evts>    <cross-section in pb>    <0 if MC, 1 if data>


 The number of events, and the appropriate cross-section, can be found on the [PAT v11 twiki page](https://twiki.cern.ch/twiki/bin/viewauth/CMS/InvHiggsPATv11). The number of events is the number after PAT processing (from `crab -report`)

* `ProgramOptions` - takes care of command-line options like `-i`
* `StackPlot` - used for making N-1 and control plots. Please see `nMinusOne.cpp` for a clear example of plotting. When creating a `StackPlot` object, you can set its name, axes labels, whether it should be lin or log, and if a ratio plot should be drawn beneath it. By default, the plots are log with no ratio plot. If using a log axis, the y axis will be scaled automatically, however it may require some fine tuning to avoid overlap with legend. Legend is generated automatically, and there is a method to set its position on the canvas
* `SumDatasets` - used to sum together individual dataset files into one, eg all the `QCD_PtXtoY` into just `QCD` 
