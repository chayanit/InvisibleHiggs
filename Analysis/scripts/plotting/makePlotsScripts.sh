#!/bin/bash

# This takes src/StackPlot.cc and interface/Stackplot.h
# and converts them into a standalone library for you to use with plotScript.C
# only some minor differences are required

# interface/Stackplot.h:
# -   // ClassDef(StackPlot,1) -> ClassDef(StackPlot,1)
# 
# src/StackPlot.h:
# -  #include "InvisibleHiggs/Analysis/interface/StackPlot.h" -> #include "StackPlot.h"
# - // ClassImp(StackPlot) -> ClassImp(StackPlot)

# Do interface/StackPlot.h
cp ../../interface/StackPlot.h StackPlot.h
sed -i 's@// ClassDef(StackPlot,1)@ClassDef(StackPlot,1)@g' StackPlot.h

# Do src/StackPlot.cc
cp ../../src/StackPlot.cc StackPlot.cc
sed -i 's@#include "InvisibleHiggs/Analysis/interface/StackPlot.h"@#include "StackPlot.h"@g' StackPlot.cc
sed -i 's@// ClassImp(StackPlot)@ClassImp(StackPlot)@g' StackPlot.cc

root -l -q -L StackPlot.cc++

# Now you can do root -q -b plotScript.C (edit it first though!)