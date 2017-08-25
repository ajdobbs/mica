/** The main application for the Muon Ionization Cooling Analysis (MICA) framework */

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

#include "src/common_cpp/DataStructure/Spill.hh"
#include "src/common_cpp/DataStructure/Data.hh"
#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"

#include "mica/AnalyserBase.hh"
#include "mica/AnalyserTrackerSpacePoints.hh"
#include "mica/AnalyserTrackerPRSeedResidual.hh"
#include "mica/AnalyserTrackerPRSeedNPEResidual.hh"
#include "mica/AnalyserTrackerPRStats.hh"
#include "mica/AnalyserTrackerAngularMomentum.hh"
#include "mica/AnalyserTrackerPREfficiency.hh"
#include "mica/AnalyserTrackerMCPurity.hh"
#include "mica/AnalyserTrackerMCPRResiduals.hh"

int main(int argc, char *argv[]) {
  // Instantiate the analysers
  std::vector<mica::AnalyserBase*> analysers;

  mica::AnalyserTrackerSpacePoints* anlTSP = new mica::AnalyserTrackerSpacePoints();
  analysers.push_back(anlTSP);

  mica::AnalyserTrackerPRSeedResidual* anlTPSR = new mica::AnalyserTrackerPRSeedResidual();
  anlTPSR->setLogScale(true);
  analysers.push_back(anlTPSR);

  mica::AnalyserTrackerPRSeedNPEResidual* anlTPSNR = new mica::AnalyserTrackerPRSeedNPEResidual();
  analysers.push_back(anlTPSNR);

  mica::AnalyserTrackerPRStats* anlTPRS = new mica::AnalyserTrackerPRStats();
  analysers.push_back(anlTPRS);

  mica::AnalyserTrackerAngularMomentum* anlTAM = new mica::AnalyserTrackerAngularMomentum();
  anlTAM->SetAnalysisStation(1);
  anlTAM->SetAnalysisPlane(0);
  analysers.push_back(anlTAM);

  mica::AnalyserTrackerMCPRResiduals* anlMCPRR = new mica::AnalyserTrackerMCPRResiduals();
  analysers.push_back(anlMCPRR);

  mica::AnalyserTrackerPREfficiency* anlPRE = new mica::AnalyserTrackerPREfficiency();
  anlPRE->SetAllowMultiHitStations(false); // false -> ideal events only, true -> non-ideal allowed
  analysers.push_back(anlPRE);

  // AnalyserTrackerMCPurity* anlMP = new AnalyserTrackerMCPurity();
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
  int spills_processed = 0;
  int events_processed = 0;
  for (int i = 0; i < nentries; ++i) {
    ++spills_processed;
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
      ++events_processed;
    }
    std::cout << "Spills processed: " << spills_processed << " of " << nentries
              << ", events processed: " << events_processed << std::endl;
  } // ~Loop over all spills

  // Plot the results
  std::vector<TVirtualPad*> pads;
  for (auto an : analysers) {
    an->Draw();
    auto new_pads = an->GetPads();
    pads.insert(std::end(pads), std::begin(new_pads), std::end(new_pads));
  }
  std::cout << "Found " << pads.size() << " canvases, saving to pdf." << std::endl;
  for (size_t i = 0; i < pads.size(); ++i) {
    pads[i]->Update();
    if (i == 0) {
      pads[i]->SaveAs((outfile + "(").c_str(), "pdf");
    } else if (i == (pads.size() - 1)) {
      pads[i]->SaveAs((outfile + ")").c_str(), "pdf");
    } else {
      pads[i]->SaveAs(outfile.c_str(), "pdf");
    }
  }

  std::cout << "File done" << std::endl;
  for (auto an : analysers) {
    delete an;
  }
  return 0;
}
