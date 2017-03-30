
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
 * \file basic_usage.cpp
 * \brief Shows to create a ScandyCore isntance and attach a TestViewport to it.
 * This basic_usage example shows to create a ScandyCore isntance and attach
 * a TestViewport to it. It then starts the visualizer and runs until closed
 * by the user.
 */

// include scandycore so we can make 3D magic happen
#include <scandy/core/IScandyCore.h>

using namespace scandy::core;
using namespace std;

int main(){
  // Create the ScandyCore instance with a Visualizer window of 400x400
  auto core = IScandyCore::factoryCreate(400,400);
  if(!core) {
    throw std::runtime_error("ERROR creating ScandyCore");
  }

  // load mesh
  auto status = core->loadMesh("scandy.ply", "scandy-texture.png");
  if(status != Status::SUCCESS) {
    throw std::runtime_error("ERROR could not read test files");
  }

  // Get a reference to the ScandyCore Visualizer
  auto viz = core->getVisualizer();
  if(!viz) {
    throw std::runtime_error("ERROR creating ScandyCore");
  }

  // Start the Visualizer. This will not return return until the window is closed by the user.
  viz->start();

  // The Visualizer will continue running until the window is closed by the user.
  viz->join();

  return 0;
}
