#!/bin/bash

# Source Geant4 environment setup (modify path as needed)
source /home/arnon/programs/Geant4-11.2.2-Linux/bin/geant4.sh

# Run your Geant4 application with arguments
cd /home/arnon/projects/kcmh-pct-geant4/build/
./pctKCMH "$@"
