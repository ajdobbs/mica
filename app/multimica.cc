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
#include "TROOT.h"
#include "TStyle.h"
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
#include "mica/AnalyserGroup.hh"
#include "mica/AnalyserTrackerSpacePoints.hh"
#include "mica/AnalyserTrackerPRSeedResidual.hh"
#include "mica/AnalyserTrackerPRSeedNPEResidual.hh"
#include "mica/AnalyserTrackerPRStats.hh"
#include "mica/AnalyserTrackerAngularMomentum.hh"
#include "mica/AnalyserTrackerPREfficiency.hh"
#include "mica/AnalyserTrackerMCPurity.hh"
#include "mica/AnalyserTrackerMCPRResiduals.hh"

int main(int argc, char *argv[]) {
  int nthreads = 2;

  std::vector<mica::AnalyserGroup*> groups;
  for (size_t i = 0; i < nthreads; ++i) {
    mica::AnalyserGroup* ag = new mica::AnalyserGroup();

    mica::AnalyserTrackerAngularMomentum* anlTAM = new mica::AnalyserTrackerAngularMomentum();
    anlTAM->SetAnalysisStation(1);
    anlTAM->SetAnalysisPlane(0);
    ag->AddAnalyser(anlTAM);

    mica::AnalyserTrackerMCPRResiduals* anlMCPRR = new mica::AnalyserTrackerMCPRResiduals();
    ag->AddAnalyser((anlMCPRR);

    groups.push_back(ag)
  }

  // Set up input file and output file
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

  // Get the ROOT TTree and extract the top level data
  TTree* T = static_cast<TTree*>(f1.Get("Spill"));
  MAUS::Data* data = nullptr;  // Don't forget = nullptr or you get a seg fault
  T->SetBranchAddress("data", &data); // Yes, this is the *address* of a *pointer*
  int nentries = T->GetEntries();
  std::cerr << "Found " << nentries << " spills\n";


  // Define a lambda function to run the analysers in each thread

  auto paraFunc = [](mica::AnalyserGroup* ag, std::vector<MAUS::Spill*>* buffer) {
    while (true) {
      // Loop over spills in the buffer
      for (auto spill : buffer) {
        if (spill == nullptr) {
          std::cout << "Spill is NULL\n";
          continue;
        }
        if (spill->GetDaqEventType() != "physics_event") {
          delete spill;
          continue;
        }

        // Loop over events in the spill
        for (auto revt : (*(spill->GetReconEvents()))) {
          MAUS::MCEvent* mevt = nullptr;
          if (event_counter < static_cast<int>(spill->GetMCEvents()->size()))
            mevt = spill->GetMCEvents()->at(event_counter);
          ag->Analyse(revt, mevt);
        }
        delete spill;
      }

      // Set the buffer size to zero to signal more data is needed
      buffer->resize(0);
      while (buffer->size() == 0) {
        // Loop until more data is ready - ideally add condition variable here
      }
    }
  }

  // Create a buffer for each thread
  int buffersize = 10;
  int currententry = 0;
  std::vector< std::vector<MAUS::Spill*>*> buffers
  for (size_t i = 0; i < nthreads; ++i) {
    buffers.push_back(new std::vector<MAUS::Spill*>())
  }

  // Launch the threads


  // Continuously fill the buffers until all the spills are done
  dataleft = true;
  while (dataleft) {
    for (auto buffer : buffers) {
      if (!dataleft) break;
      for (int i = currententry; i < buffersize; ++i) {
        if (currententry >= nentries) {
          dataleft = false
          break;
        }
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
        buffer->push_back(new Spill(*spill));
        ++currententry
      }
    }
  }

  // Kill the threads


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
