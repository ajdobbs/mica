/* This file is part of MAUS: http://micewww.pp.rl.ac.uk/projects/maus
 *
 * MAUS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MAUS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MAUS.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// C / C++ headers
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// ROOT headers
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TVirtualPad.h"
#include "TCanvas.h"

#include "src/common_cpp/JsonCppStreamer/IRStream.hh"
#include "src/common_cpp/DataStructure/Spill.hh"
#include "src/common_cpp/DataStructure/Data.hh"
#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"

#include "src/common_cpp/Analysis/AnalyserBase.hh"
#include "src/common_cpp/Analysis/AnalyserTrackerSpacePoints.hh"
#include "src/common_cpp/Analysis/AnalyserTrackerPRSeedResidual.hh"
#include "src/common_cpp/Analysis/AnalyserTrackerPRSeedNPEResidual.hh"
#include "src/common_cpp/Analysis/AnalyserTrackerPRStats.hh"
#include "src/common_cpp/Analysis/AnalyserTrackerAngularMomentum.hh"
#include "src/common_cpp/Analysis/AnalyserTrackerPREfficiency.hh"
#include "src/common_cpp/Analysis/AnalyserTrackerMCPurity.hh"

/** Access Tracker data using ROOT */

int main(int argc, char *argv[]) {
  // Instantiate the analysers
  std::vector<MAUS::Analysis::AnalyserBase*> analysers;

  MAUS::Analysis::AnalyserTrackerSpacePoints* anlTSP =
    new MAUS::Analysis::AnalyserTrackerSpacePoints();
  analysers.push_back(anlTSP);

  MAUS::Analysis::AnalyserTrackerPRSeedResidual* anlTPSR =
    new MAUS::Analysis::AnalyserTrackerPRSeedResidual();
  anlTPSR->setLogScale(true);
  analysers.push_back(anlTPSR);

  MAUS::Analysis::AnalyserTrackerPRSeedNPEResidual* anlTPSNR =
    new MAUS::Analysis::AnalyserTrackerPRSeedNPEResidual();
  analysers.push_back(anlTPSNR);

  MAUS::Analysis::AnalyserTrackerPRStats* anlTPRS = new MAUS::Analysis::AnalyserTrackerPRStats();
  analysers.push_back(anlTPRS);

  MAUS::Analysis::AnalyserTrackerAngularMomentum* anlTAM =
    new MAUS::Analysis::AnalyserTrackerAngularMomentum();
  anlTAM->SetAnalysisStation(1);
  anlTAM->SetAnalysisPlane(0);
  analysers.push_back(anlTAM);

  MAUS::Analysis::AnalyserTrackerPREfficiency* anlPRE =
    new MAUS::Analysis::AnalyserTrackerPREfficiency();
  anlPRE->SetAllowMultiHitStations(false); // false -> ideal events only, true -> non-ideal allowed
  analysers.push_back(anlPRE);

  // MAUS::Analysis::AnalyserTrackerMCPurity* anlMP = new MAUS::Analysis::AnalyserTrackerMCPurity();
  // analysers.push_back(anlMP);

  // Set up ROOT app, input file, and MAUS data class
  std::string infile = "";
  std::string outfile = "analysis.pdf";
  if (argc > 1) {
    infile = std::string(argv[1]); // 1st arg to code should be input ROOT file name
  } else {
    std::cerr << "Please enter the input file name as the first argument and try again\n";
    return 0;
  }
  if (argc > 2)
    outfile = std::string(argv[2]);
  std::cout << "Input file " << infile << std::endl;
  std::cout << "Output file " << outfile << std::endl;
  TFile f1(infile.c_str());
  TTree* T = static_cast<TTree*>(f1.Get("Spill"));
  MAUS::Data* data = nullptr;  // Don't forget = nullptr or you get a seg fault
  T->SetBranchAddress("data", &data); // Yes, this is the *address* of a *pointer*
  int nentries = T->GetEntries();
  std::cerr << "Found " << nentries << " spills\n";

  // Loop over all spills
  for (int i = 0; i < nentries; ++i) {
    T->GetEntry(i);
    if (!data) {
      std::cout << "Data is NULL\n";
      continue;
    }
    MAUS::Spill* spill = data->GetSpill();
    if (spill == nullptr) {
      std::cout << "Spill is NULL\n";
      continue;
    }
    if (spill->GetDaqEventType() != "physics_event") {
      // std::cout << "Spill is of type " << spill->GetDaqEventType() << ", not a usable spill\n";
      continue;
    }
    std::cout << "Spill: " << spill->GetSpillNumber() << std::endl;

    // Call the analysers
    int event_counter = 0;
    for (auto revt : (*(spill->GetReconEvents()))) {
      MAUS::MCEvent* mevt = nullptr;
      if (event_counter < static_cast<int>(spill->GetMCEvents()->size()))
        mevt = spill->GetMCEvents()->at(event_counter);
      for (auto an : analysers) {
        an->Analyse(revt, mevt);
      }
      ++event_counter;
    }
  } // ~Loop over all spills

  // Plot the results
  for (size_t i = 0; i < analysers.size(); ++i) {
    TVirtualPad* pad1 = analysers[i]->Draw();
    pad1->Update();
    if (i == 0) {
      pad1->SaveAs((outfile + "(").c_str(), "pdf");
    } else if (i == (analysers.size() - 1)) {
      pad1->SaveAs((outfile + ")").c_str(), "pdf");
    } else {
      pad1->SaveAs(outfile.c_str(), "pdf");
    }
  }

  std::cout << "File done" << std::endl;
  for (auto an : analysers) {
    delete an;
  }
  return 0;
}
