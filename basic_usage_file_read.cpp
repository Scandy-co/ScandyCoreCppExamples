
/****************************************************************************\
 * Copyright (C) 2016 Scandy
 *
 * THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
\****************************************************************************/

/**
 * \file basic_usage_file_read.cpp
 * \brief Shows how to create a ScandyCore instance and read a Royale rrf file.
 * This basic_usage example shows to create a ScandyCore isntance and read a
 * Royale file. It then starts the visualizer and runs until closed
 * by the user.
 */

// include scandycore so we can make 3D magic happen
#include <scandy/core/IScandyCore.h>

#include <thread>

using namespace scandy::core;
using namespace std;

using namespace std::chrono_literals;

int main(int argc, char *argv[]) {
  scandy::core::Status status;

  // Create the ScandyCore instance with a Visualizer window of 400x400
  auto core = IScandyCore::factoryCreate(400,400);
  if(!core) {
    throw std::runtime_error("ERROR creating ScandyCore");
  }

  // Add some callbacks.
  core->onScannerStart = []() { std::cout << "Starting file playback";};
  core->onScannerStop = []() { std::cout << "File playback stopped."; };

  // get the view window
  auto viz = core->getVisualizer();
  if(!viz) {
    throw std::runtime_error("ERROR creating ScandyCore");
  }

  // Read in a Royale rrf file from the command line
  status = core->initializeScanner(ScannerType::FILE, argv[1]);
  if(status != Status::SUCCESS) {
    throw std::runtime_error("ERROR could not read test file");
  }

  // Start SLAM to process the depth data in the rrf file.
  status = core->startPreview();

  // The visualizer window must run in the main thread.  Create a background
  // thread to kill the visualizer thread after the entire file has been processed.
  std::thread slam_thread([core, viz]() {

    auto status = core->startScanning();
    if(status != Status::SUCCESS) {
      throw std::runtime_error("ERROR starting scanner");
    }

    std::this_thread::sleep_for(5s);

    status = core->stopScanning();
    if(status != Status::SUCCESS) {
      throw std::runtime_error("ERROR stopping scanner");
    }

    viz->stop();
    viz->join();
  });

  // Start the visualizer.
  viz->start();

  // Release the slam_thread.
  slam_thread.join();

  return 0;
}
