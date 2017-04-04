
/****************************************************************************\
 * Copyright (C) 2017 Scandy
 *
 * THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 * PURPOSE.
 *
\****************************************************************************/

/**
 * \file basic_usage.cpp
 * \brief Shows to create a ScandyCore isntance and attach a TestViewport to it.
 * This basic_usage example shows to create a ScandyCore isntance and attach
 * a TestViewport to it. It then starts the visualizer and runs until closed
 * by the user.
 */

// include scandycore so we can make 3D magic happen
#include <scandy/core/IScandyCore.h>
#include <scandy_license.h>

#include <thread>
#include <iostream>

using namespace scandy::core;
using namespace std;

int main(int argc, char *argv[]) {
  // Create the ScandyCore instance with a Visualizer window of 400x400
  auto core = IScandyCore::factoryCreate(400,400);
  if(!core) {
    std::cerr << "ERROR creating ScandyCore" << std::endl;
    return (int) scandy::core::Status::EXCEPTION_IN_HANDLER;;
  }

  // We need to set the ScandyCore license to our internal license
  auto status = core->setLicense(scandy_core_license);
  if(status != Status::SUCCESS) {
    std::cerr << "ERROR could not set license" << std::endl;
    return (int) status;
  }

  // read data from the attached the Pico Flex
  status = core->initializeScanner(ScannerType::PICO_FLEXX);
  if(status != Status::SUCCESS) {
    std::cerr << "ERROR could not find attached usb Pico Flexx depth sensor" << std::endl;
    return (int) status;
  }

  // Get a reference to the ScandyCore Visualizer
  auto viz = core->getVisualizer();
  if(!viz) {
    std::cerr << "ERROR getting ScandyCore Visualizer" << std::endl;
    return (int) scandy::core::Status::NO_VISUALIZER_FOUND;
  }

  // Start SLAM to process the depth data in the rrf file.
  status = core->startPreview();

  // The visualizer window must run in the main thread.  Create a background
  // thread to kill the visualizer thread after the entire file has been processed.
  std::thread slam_thread([core, viz]() {

    auto status = core->startScanning();
    if(status != Status::SUCCESS) {
      std::cerr << "ERROR starting scanner" << std::endl;
      abort();
    }

    // continue to scan until user cancels
    std::cout << "Press 'q' to stop scanning: ";
    char ch;
    do {
      std::cin >> ch;
    }
    while(ch != 'q');

    status = core->stopScanning();
    if(status != Status::SUCCESS) {
      std::cerr << "ERROR stopping scanner" << std::endl;
      abort();
    }

    viz->stop();
    viz->join();
  });

  // Start the Visualizer. This will not return return until the window is closed by the user.
  viz->start();

  // The Visualizer will continue running until the window is closed by the user.
  slam_thread.join();

  // create the mesh
  status = core->createVisualizer(400,400, 1, 1);
  if(status != Status::SUCCESS) {
    std::cerr << "ERROR could not create visualizer" << std::endl;
    return (int) status;
  }
  viz = core->getVisualizer();

  // generate the mesh
  status = core->generateMesh();
  if(status != Status::SUCCESS) {
    std::cerr << "ERROR could not generate mesh" << std::endl;
    return (int) status;
  }

  // save mesh
  status = core->saveMesh("temp.ply");
  if(status != Status::SUCCESS) {
    std::cerr << "ERROR could not save mesh" << std::endl;
    return (int) status;
  }

  std::cout << "Saved mesh to temp.ply" << std::endl;

  // view mesh
  viz->start();

  return 0;
}
