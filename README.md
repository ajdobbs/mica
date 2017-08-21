MICA - Muon Ionization Cooling Analysis
***************************************

Author - A. J. Dobbs

MICA is an analysis framework for the Muon Ionization Cooling Experiment. It is dependent on the
MICE reconstruction software, MAUS.

Installation instructions
-------------------------

* Make a directory for the project

mkdir mica
cd mica

* Obtain the source code by cloning the MICA repository with git into a subdirectory:

git clone git@github.com:ajdobbs/mica.git source

* Source an existing MAUS installation (see http://micewww.pp.rl.ac.uk/projects/maus/wiki/Install)

source /path/to/maus/env.sh

* Make a build directory

mkdir build
cd build

* Build using CMake

cmake ../source/
make
