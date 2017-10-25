/** The main application for the Muon Ionization Cooling Analysis (MICA) framework */

// std library headers
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

// ROOT headers
#include "TApplication.h"
#include "TGClient.h"
#include "TStyle.h"
#include "TFile.h"
#include "TTree.h"
#include "TVirtualPad.h"

// MAUS headers
#include "src/common_cpp/DataStructure/Spill.hh"
#include "src/common_cpp/DataStructure/Data.hh"
#include "src/common_cpp/DataStructure/ReconEvent.hh"
#include "src/common_cpp/DataStructure/MCEvent.hh"

// MICA headers
#include "mica/AnalyserBase.hh"
#include "mica/AnalyserFactory.hh"
#include "mica/AnalyserTrackerPRSeedResidual.hh"
#include "mica/AnalyserTrackerPREfficiency.hh"

int main(int argc, char *argv[]) {
  // Instantiate the analysers required
  std::vector<std::string> anl_names {"AnalyserTrackerKFMomentum",
                                      "AnalyserTofTracker",
                                      "AnalyserViewerRealSpace"};
  std::vector<mica::AnalyserBase*> analysers = mica::AnalyserFactory::CreateAnalysers(anl_names);

  // Should we pause between events?
  bool bool_pause = true;

  // Set up the input and output files using the programme arguments
  std::string infile = "";
  std::string outfile = "viewer.pdf";
  if (argc > 1) {
    infile = std::string(argv[1]); // 1st arg to code should be input ROOT file name
  } else {
    std::cerr << "Please enter the input file name as the first argument and try again\n";
    return -1;
  }

  TApplication theApp("App", &argc, argv); // Set up the ROOT application for live plotting

  TFile f1(infile.c_str());
  if (!f1.IsOpen()) {
      std::cerr << "Failed to find file: " << infile <<std::endl;
      return -1;
  }
  std::cout << "Input file " << infile << std::endl;

  if (argc > 2) outfile = std::string(argv[2]);
  std::cout << "Output file " << outfile << std::endl;

  // Set up access to ROOT data from input file
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
    for (auto an : analysers) {
      an->Draw();
    }

    int event_counter = 0;
    for (auto revt : (*(spill->GetReconEvents()))) {
      MAUS::MCEvent* mevt = nullptr;
      if (event_counter < static_cast<int>(spill->GetMCEvents()->size()))
        mevt = spill->GetMCEvents()->at(event_counter);
      for (auto an : analysers) {
        an->Analyse(revt, mevt);
        an->Update();
        for (auto pad : an->GetPads()) {
          if (pad) {
            pad->Update();
          }
        }
      }
      if (bool_pause) {
        std::cout << "Press Enter to Continue";
        std::cin.ignore();
      }
      ++event_counter;
      ++events_processed;
    }
    std::cout << "Spills processed: " << spills_processed << " of " << nentries
              << ", events processed: " << events_processed << std::endl;
  } // ~Loop over all spills

  // Wrap up
  for (auto an : analysers) {
    delete an;
  }
  f1.Close();
  theApp.Run();
  return 0;
}
