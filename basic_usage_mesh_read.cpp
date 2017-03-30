
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

  // load mesh from command line arguements
  char *mesh_path = argv[1];
  if( mesh_path == nullptr ){
    std::cerr << "ERROR you must provide a path to the mesh" << std::endl;
    return (int) scandy::core::Status::FILE_NOT_FOUND;;
  }
  char *texture_path = argv[2];
  if( texture_path == nullptr ){
    texture_path = "";
  }
  auto status = core->loadMesh(mesh_path, texture_path);
  if(status != Status::SUCCESS) {
    std::cerr << "ERROR could not read test files" << std::endl;
    return (int) status;
  }

  // Get a reference to the ScandyCore Visualizer
  auto viz = core->getVisualizer();
  if(!viz) {
    std::cerr << "ERROR getting ScandyCore Visualizer" << std::endl;
    return (int) scandy::core::Status::NO_VISUALIZER_FOUND;
  }

  // Start the Visualizer. This will not return return until the window is closed by the user.
  viz->start();

  // The Visualizer will continue running until the window is closed by the user.
  viz->join();

  return 0;
}
