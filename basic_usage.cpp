
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

// include ScandyCore so we can make 3D magic happen
#include <scandy/core/IScandyCore.h>
#include <scandy/core/visualizer/TestViewport.h>

using namespace scandy::core;
using namespace std;

int main(){
  // Create the ScandyCore instance with a Visualizer window of 400x400
  auto core = IScandyCore::factoryCreate(400,400);

  // Get a reference to the ScandyCore Visualizer
  auto viz = core->getVisualizer();;

  // Create a scandy::core::TestViewport
  auto viewport = std::make_shared<TestViewport>();

  // Add the new TestViewport to the ScandyCore Visualizer
  viz->addViewport(*viewport);

  // Start the Visualizer. This will not return return until the window is closed by the user.
  viz->start();

  // The Visualizer will continue running until the window is closed by the user.
  viz->join();

  return 0;
}
