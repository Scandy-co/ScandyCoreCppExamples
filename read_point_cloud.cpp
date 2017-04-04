
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
 * \file basic_usage_file_read.cpp
 * \brief Shows how to create a ScandyCore instance and read a Royale rrf file.
 * This basic_usage example shows to create a ScandyCore isntance and read a
 * Royale file. It then starts the visualizer and runs until closed
 * by the user.
 */

// include scandycore so we can make 3D magic happen
#include <scandy/core/IScandyCore.h>
#include <scandy_license.h>

// Include some vtk pieces for getting PolyData and saving it to a PLY
#include <vtkNew.h>
#include <vtkSmartPointer.h>
#include <vtkPLYWriter.h>
#include <vtkPolyData.h>

#include <thread>
#include <chrono>
#include <cmath>

using namespace scandy::core;
using namespace std;

using namespace std::chrono_literals;

int main(int argc, char *argv[]) {
  scandy::core::Status status;

  // Create the ScandyCore instance with a Visualizer window of 400x400
  auto core = IScandyCore::factoryCreate(400,400);
  if(!core) {
    std::cerr << "ERROR creating ScandyCore" << std::endl;
    return (int) scandy::core::Status::EXCEPTION_IN_HANDLER;;
  }

  // Add some callbacks.
  core->onScannerStart = []() { std::cout << "Starting file playback" << std::endl;};
  core->onScannerStop = []() { std::cout << "File playback stopped." << std::endl;};

  auto last = std::chrono::system_clock::now();
  core->onScanPointCloudDidUpdate = [&](
    const scandy::utilities::float3* point_cloud_data,
    const int width,
    const int height,
    const scandy::utilities::PointCloudMetaData frame_metadata) {

    try {
      auto now = std::chrono::system_clock::now();
      std::chrono::duration<double> elapsed_seconds = now - last;
      if(elapsed_seconds.count() > 0) {

        // Save a ply file every second
        vtkPolyData *new_ply_data = vtkPolyData::New();
        vtkPoints *points = vtkPoints::New();

        // insert the points
        for(int ix=0; ix < width*height; ix++) {
          points->InsertPoint(ix, point_cloud_data[ix].s);
        }
        new_ply_data->SetPoints(points);

        // save the data
        std::stringstream outputFilename;
        outputFilename << "/tmp/output"  << frame_metadata.timestamp << ".ply";

        vtkSmartPointer<vtkPLYWriter> writer = vtkSmartPointer<vtkPLYWriter>::New();
        writer->SetFileName( outputFilename.str().c_str() );
        writer->SetInputData( new_ply_data );
        writer->SetFileTypeToASCII();
        writer->Write();

        points->Delete();
        new_ply_data->Delete();

        last = now;
      }

      last = now;
    }
    catch( std::exception &e ) {
      std::cerr << "Exception in writing the point cloud to file: " << e.what() << std::endl;
    }
  };

  // We need to set the ScandyCore license to our internal license
  status = core->setLicense(scandy_core_license);
  if(status != Status::SUCCESS) {
    std::cerr << "ERROR could not set license" << std::endl;
    return (int) status;
  }

  // get the view window
  auto viz = core->getVisualizer();
  if(!viz) {
    std::cerr << "ERROR getting ScandyCore Visualizer" << std::endl;
    return (int) scandy::core::Status::NO_VISUALIZER_FOUND;
  }

  // Read in a Royale rrf file from the command line
  if(argc >= 2) {
    char *file_path = argv[1];
    if( file_path == nullptr ){
      std::cerr << "ERROR you must provide a path to the file" << std::endl;
      return (int) scandy::core::Status::FILE_NOT_FOUND;;
    }
    status = core->initializeScanner(ScannerType::FILE, file_path);
    if(status != Status::SUCCESS) {
      std::cerr << "ERROR could not read test files" << std::endl;
      return (int) status;
    }
  }
  else {
    // read data from the attached the Pico Flex
    status = core->initializeScanner(ScannerType::PICO_FLEXX);
    if(status != Status::SUCCESS) {
      std::cerr << "ERROR could not find attached usb Pico Flexx depth sensor" << std::endl;
      return (int) status;
    }
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

    std::this_thread::sleep_for(5s);

    status = core->stopScanning();
    if(status != Status::SUCCESS) {
      std::cerr << "ERROR stopping scanner" << std::endl;
      abort();
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
