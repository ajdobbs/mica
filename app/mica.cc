/** The main application for the Muon Ionization Cooling Analysis (MICA) framework */

// std library headers
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

// ROOT headers
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

/** Analyse one ROOT file, using a given set of MICE analysers */
void analyse_file(TFile& aFile, std::vector<mica::AnalyserBase*>& analysers);

/** Draw and save to pdf the data contained in a given set of MICA analysers */
void make_plots(const std::string& ofname, std::vector<mica::AnalyserBase*>& analysers);

/** The main MICA app function - prepare the input file, analyse, plot, save to pdf */
int main(int argc, char *argv[]) {
  // Instantiate the analysers required
  std::vector<std::string> analyser_names {"AnalyserTrackerChannelHits",
                                           "AnalyserTrackerSpacePoints",
                                           "AnalyserTrackerPRSeedResidual",
                                           "AnalyserTrackerPRSeedNPEResidual",
                                           "AnalyserTrackerPRStats",
                                           "AnalyserTrackerAngularMomentum",
                                           "AnalyserTrackerMCPRResiduals",
                                           "AnalyserTrackerPREfficiency",
                                           "AnalyserTrackerKFStats",
                                           "AnalyserTrackerKFMomentum",
                                           "AnalyserTofTracker"};
  std::vector<mica::AnalyserBase*> analysers
      = mica::AnalyserFactory::CreateAnalysers(analyser_names);

  // Customise a few specific analyser options
  // Use a log scale for patrec seed residual plots
  dynamic_cast<mica::AnalyserTrackerPRSeedResidual*>(analysers[2])->setLogScale(true);
  // Don't restrict efficiency calc to ideal events (false = ideal events only, true = non-ideal ok)
  dynamic_cast<mica::AnalyserTrackerPREfficiency*>(analysers[7])->SetAllowMultiHitStations(false);
  // Only use the TOFs to define an expected good event, not tracker spacepoints
  dynamic_cast<mica::AnalyserTrackerPREfficiency*>(analysers[7])->SetCheckTkU(false);
  dynamic_cast<mica::AnalyserTrackerPREfficiency*>(analysers[7])->SetCheckTkD(false);

  // Set up the input and output files using the programme arguments
  std::string infile = "";
  std::string outfile = "analysis.pdf";
  if (argc > 1) {
    infile = std::string(argv[1]); // 1st arg to code should be input ROOT file name
  } else {
    std::cerr << "Please enter the input file name as the first argument and try again\n";
    return -1;
  }
  TFile f1(infile.c_str());
  if (!f1.IsOpen()) {
      std::cerr << "Failed to find file: " << infile <<std::endl;
      return -1;
  }
  std::cout << "Input file " << infile << std::endl;

  if (argc > 2) outfile = std::string(argv[2]);
  std::cout << "Output file " << outfile << std::endl;

  // Analyse the input ROOT file using the analysers
  analyse_file(f1, analysers);

  // Plot the results contained in the analysers
  make_plots(outfile, analysers);

  // Wrap up
  for (auto an : analysers) {
    delete an;
  }
  return 0;
}

void analyse_file(TFile& aFile, std::vector<mica::AnalyserBase*>& analysers) {
  // Set up access to ROOT data from input file
  TTree* T = static_cast<TTree*>(aFile.Get("Spill"));
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
}

void make_plots(const std::string& ofname, std::vector<mica::AnalyserBase*>& analysers) {
  std::vector<std::shared_ptr<TVirtualPad>> pads;
  std::vector<std::shared_ptr<TStyle> > styles;
  for (auto an : analysers) {
    an->Draw();
    for (auto pad : an->GetPads()) {
      if (pad) {
        pads.push_back(pad);
        styles.push_back(an->GetStyle());
      }
    }
  }
  std::cout << "Found " << pads.size() << " canvases, saving to pdf." << std::endl;
  for (size_t i = 0; i < pads.size(); ++i) {
    if (styles[i]) styles[i]->cd();
    pads[i]->Update();
    if (i == 0) {
      pads[i]->SaveAs((ofname + "(").c_str(), "pdf");
    } else if (i == (pads.size() - 1)) {
      pads[i]->SaveAs((ofname + ")").c_str(), "pdf");
    } else {
      pads[i]->SaveAs(ofname.c_str(), "pdf");
    }
  }
}
