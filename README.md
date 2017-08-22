# MICA - Muon Ionization Cooling Analysis

Author - A. J. Dobbs

MICA is an analysis framework for the Muon Ionization Cooling Experiment. It is dependent on the MICE reconstruction software, MAUS.

## Installation instructions


* Make a directory for the project

```bash
mkdir mica
cd mica
```

* Obtain the source code by cloning the MICA repository with git into a subdirectory:

```bash
git clone git@github.com:ajdobbs/mica.git source
```

or, if you do not have a github login, clone anonymously with:

```bash
git clone https://github.com/ajdobbs/mica.git source
```

* Source an existing MAUS installation (see http://micewww.pp.rl.ac.uk/projects/maus/wiki/Install)

```bash
source /path/to/maus/env.sh
```

* Make a build directory

```bash
mkdir build
cd build
```

* Build using CMake

```bash
cmake ../source/ -DCMAKE_INSTALL_PREFIX:PATH=/path/to/install/directory
make -jN
```

where the last argument to cmake is optional, specifying a custom installation directory, and N is the
number of cores to build with.

* To clean a build, simply delete everything in your build directory

* Installation

If you wish you to install MICA rather than running from the build directory, do:

```bash
make install
```

Note: if using a custom installation directory, you may need to update LD_LIBRARY_PATH and PATH:

```bash
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/path/to/install/directory/lib
export PATH=${PATH}:/path/to/install/directory/bin
```

## Usage

Run an analysis with:

```bash
./bin/mica /path/to/maus/recon/data/maus_output.root
```

The output can then be found in ```analysis.pdf```.
