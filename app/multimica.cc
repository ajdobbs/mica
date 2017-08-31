/** The main application for the Muon Ionization Cooling Analysis (MICA) framework */

// C / C++ headers
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>

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
  int nthreads = 4;

  // Set up identical groups of analysers, one group for each thread
  std::vector<mica::AnalyserGroup*> groups;
  for (size_t i = 0; i < nthreads; ++i) {
    mica::AnalyserGroup* ag = new mica::AnalyserGroup();

    mica::AnalyserTrackerSpacePoints* anlTSP = new mica::AnalyserTrackerSpacePoints();
    ag->AddAnalyser(anlTSP);

    mica::AnalyserTrackerAngularMomentum* anlTAM = new mica::AnalyserTrackerAngularMomentum();
    anlTAM->SetAnalysisStation(1);
    anlTAM->SetAnalysisPlane(0);
    ag->AddAnalyser(anlTAM);

    mica::AnalyserTrackerMCPRResiduals* anlMCPRR = new mica::AnalyserTrackerMCPRResiduals();
    ag->AddAnalyser(anlMCPRR);

    groups.push_back(ag);
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

  // Create a buffer for each thread
  int buffersize = 50;
  std::vector<std::vector<MAUS::Spill*> > buffers;
  for (size_t ithread = 0; ithread < nthreads; ++ithread) {
    std::vector<MAUS::Spill*> lSpills(buffersize);
    buffers.push_back(lSpills);
  }

  // Set up some inter-thread communication variables
  bool* tDataLeft = new bool(true); //  Tells all threads whether there is still any data left to process
  std::vector<bool*> tDataRequests; // Data requestor flag for each thread
  for (int ithread = 0; ithread < nthreads; ++ithread) {
    tDataRequests.push_back(new bool(true));
  }

  // Define a lambda function to run the analysers in each thread
  auto paraFunc = [](mica::AnalyserGroup* ag, std::vector<MAUS::Spill*>& buffer,
                     bool* dataneeded, bool* dataleft) {
    std::cout << "Thread " << std::this_thread::get_id() << " launching" << std::endl;
    while (*dataleft) {
      // Communicate to main thread that this thread is ready for more data
      *dataneeded = true;
      std::cout << "Thread " << std::this_thread::get_id() << " requesting data " << std::endl;
      std::cout << "Thread " << std::this_thread::get_id() << " data needed: " << *dataneeded << ", dataleft: " << *dataleft << std::endl;
      std::cout << "Thread " << std::this_thread::get_id() << " data needed: " << dataneeded << ", dataleft: " << dataleft << std::endl;
      while (*dataneeded) {
        // Loop until more data is ready - ideally add condition variable here
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
        // std::cout << "Thread " << std::this_thread::get_id() << ", in while loop, data needed: " << *dataneeded << std::endl;
        if (!(*dataleft)) break;
      }
      if (*dataneeded) break;
      std::cout << "Thread " << std::this_thread::get_id() << " data acquired" << std::endl;

      // Loop over spills in the buffer
      for (auto spill : buffer) {
        if (spill == nullptr) {
          std::cout << "Spill is NULL\n";
          continue;
        }
        if (spill->GetDaqEventType() != "physics_event") {
          continue;
        }
        std::cout << "Thread " << std::this_thread::get_id() << ", processing spill "
                  << spill->GetSpillNumber() << std::endl;

        // Loop over events in the spill
        int event_counter = 0;
        for (auto revt : (*(spill->GetReconEvents()))) {
          MAUS::MCEvent* mevt = nullptr;
          if (event_counter < static_cast<int>(spill->GetMCEvents()->size()))
            mevt = spill->GetMCEvents()->at(event_counter);
          ag->Analyse(revt, mevt);
          ++event_counter;
        }
      }
    }
    std::cout << "Thread " << std::this_thread::get_id() << " terminating" << std::endl;
  };

  // Launch the threads
  std::vector<std::thread> threads;
  for (int i = 0; i < nthreads; ++i) {
    threads.push_back(std::thread(paraFunc, groups[i], std::ref(buffers[i]),
                                  tDataRequests[i], tDataLeft));
  }


  // Continuously fill the buffers until all the spills are done
  int currententry = 0;
  bool lDataLeft = true; // Is there data left - version local to main thread
  while (lDataLeft) {
    // Loop over the buffers
    for (int ithread = 0; ithread < nthreads; ++ithread) {
      // std::cout << "Main: thread " << ithread << " requesting data: "
      //           << *tDataRequests[ithread] << std::endl;
      if (!lDataLeft) break;
      if (!(*tDataRequests[ithread])) {
        // std::cout << "Main: thread " << ithread << ", still processing buffer, looping" << std::endl;
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
        continue; // This buffer is still being processed, move on
      }

      // Buffer needs refilling
      std::cout << "Main: thread " << ithread << " refilling buffer " << std::endl;

      // Clear this buffer's memory first
      for (auto& sp : buffers[ithread]) {
        if (sp) delete sp;
        sp = nullptr;
      }

      for (int ispill = 0; ispill < buffersize; ++ispill) {
        if (!lDataLeft) break;
        if (currententry == (nentries - 1)) { // Reached the last entry, process it and then stop
          lDataLeft = false;
        }
        T->GetEntry(currententry);
        std::cout << "Processing entry " << currententry << std::endl;
        if (!data) {
          std::cout << "Data is NULL\n";
          continue;
        }
        MAUS::Spill* spill = data->GetSpill();
        if (spill == nullptr) {
          std::cout << "Spill is NULL\n";
          continue;
        }
        // std::cerr << buffers.size() << " " << buffers[ithread].size() << " " << ithread << " " << ispill << std::endl;
        buffers[ithread][ispill] = new MAUS::Spill(*spill);
        // buffers[ithread][ispill] = spill;
        ++currententry;
      }
      // Finished filling the buffer, tell the requesting thread
      *(tDataRequests[ithread]) = false;
      std::cout << "Main: telling thread " << ithread << " it has data: " << *(tDataRequests[ithread]) << std::endl;
      std::cout << "Main: thread " << ithread << ", data needed pointer " << (tDataRequests[ithread]) << std::endl;

      // Move on to the next buffer, or break out if that was the last spill
      if (!lDataLeft) break;
    }
    if (!lDataLeft) break;
  }

  // Tell the threads we have no more data left and wait for them to finish
  *tDataLeft = false;
  for (auto& th : threads) {
    th.join();
  }

  // Merge the analysers (merge the rest into the first AnalyserGroup)
  for (int ithread = 1; ithread < nthreads; ++ithread) {
    groups[0]->Merge(groups[ithread]);
  }

  // Plot the results and save to file
  std::vector<TVirtualPad*> pads = groups[0]->Draw(); // Draws for each internal analyser
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

  // Wrap up
  std::cout << "File done" << std::endl;
  for (auto& gp : groups) {
    for (auto&  an : gp->GetAnalysers()) {
      if (an) delete an;
      an = nullptr;
    }
    delete gp;
    gp = nullptr;
  }
  return 0;
}
