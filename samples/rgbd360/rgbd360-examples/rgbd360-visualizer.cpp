/* +---------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)               |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2014, Individual contributors, see AUTHORS file        |
   | See: http://www.mrpt.org/Authors - All rights reserved.                   |
   | Released under BSD License. See details in http://www.mrpt.org/License    |
   +---------------------------------------------------------------------------+ */

#include <mrpt/base.h>
#include <mrpt/gui.h>
#include <mrpt/opengl.h>
//#include <mrpt/maps.h>
#include <mrpt/utils/CFileGZInputStream.h>
#include <mrpt/slam/CObservationRGBD360.h>
#include <mrpt/slam/CObservationIMU.h>
#include <mrpt/obs.h>

#ifndef PI
  #define PI 3.14159265
#endif

using namespace std;
using namespace mrpt;
using namespace mrpt::slam;
using namespace mrpt::utils;
//using namespace mrpt::hwdrivers;

// This simple demo opens a rawlog dataset containing the data recorded
// by several rgbd sensors and build the calibrated rgbd images from it


#include <pcl/visualization/cloud_viewer.h>
#include <boost/thread/thread.hpp>

///*! This class creates a visualizer to display a Frame360 object. The visualizer runs in a separate thread,
// *  and can be sychronized using the visualizationMutex.
// */
//class Frame360_Visualizer
//{
// public:
//  /*! Omnidirectional RGB-D image to be displayed */
//  //Frame360 *frame360;
//    Rgbd360_ext *frame360;

//  /*! Frame Index */
//  unsigned frameIdx;

//  /*! Visualizer object */
//  pcl::visualization::CloudViewer viewer;

//  /*! Constructor. It starts the visualization in a separate thread */
//  Frame360_Visualizer(Frame360 *frame = NULL) :
//    frame360(frame),
//    viewer("PbMap"),
//    bShowPlanes(false),
//    bColoredPlanes(false)
////    numScreenshot(0)
//  {
//    viewer.runOnVisualizationThread (boost::bind(&Frame360_Visualizer::viz_cb, this, _1), "viz_cb");
//    viewer.registerKeyboardCallback (&Frame360_Visualizer::keyboardEventOccurred, *this);
//  }

//  /*! Mutex to syncrhronize eventual changes in frame360 */
//  boost::mutex visualizationMutex;

//  #if RECORD_VIDEO
//    /*! Index of visualizer screenshot. This is used to create videos with the results */
//    int numScreenshot;
//  #endif

// private:
//  /*! Show the PbMap's planes. It is control through keyboard event. */
//  bool bShowPlanes;

//  /*! Show the PbMap's planes filled with different colors. It is control through keyboard event. */
//  bool bColoredPlanes;

//  /*! Visualization callback */
//  void viz_cb(pcl::visualization::PCLVisualizer& viz)
//  {
////    {
////      boost::mutex::scoped_lock updateLock(visualizationMutex);
////
////      std::cout << "viz_cb\n";
////    if (frame360 != NULL)
////      std::cout << frame360->sphereCloud->empty() << "\n";

//    if (frame360 == NULL || frame360->sphereCloud->empty())
//    {
//      boost::this_thread::sleep (boost::posix_time::milliseconds (10));
//      return;
//    }

//    {
//      boost::mutex::scoped_lock updateLock(visualizationMutex);

//      // Render the data
//      viz.removeAllPointClouds();
//      viz.removeAllShapes();
//      viz.setSize(800,600); // Set the window size

//      if (!viz.updatePointCloud (frame360->cloud_[0], "sphereCloud"))
//        viz.addPointCloud (frame360->cloud_[0], "sphereCloud");

////      if (!viz.updatePointCloud (frame360->sphereCloud, "sphereCloud"))
////        viz.addPointCloud (frame360->sphereCloud, "sphereCloud");

//      char name[1024];

//      sprintf (name, "Frame %u", frameIdx);
//      viz.addText (name, 20, 20, "info");

//      if(bShowPlanes)
//      {
//        // Draw planes
//        for(size_t i=0; i < frame360->local_planes_[0].vPlanes.size(); i++)
//        {
//          mrpt::pbmap::Plane &plane_i = frame360->planes.vPlanes[i];
//          sprintf (name, "normal_%u", static_cast<unsigned>(i));
//          pcl::PointXYZ pt1, pt2; // Begin and end points of normal's arrow for visualization
//          pt1 = pcl::PointXYZ(plane_i.v3center[0], plane_i.v3center[1], plane_i.v3center[2]);
//          pt2 = pcl::PointXYZ(plane_i.v3center[0] + (0.5f * plane_i.v3normal[0]),
//                              plane_i.v3center[1] + (0.5f * plane_i.v3normal[1]),
//                              plane_i.v3center[2] + (0.5f * plane_i.v3normal[2]));
//          viz.addArrow (pt2, pt1, ared[i%10], agrn[i%10], ablu[i%10], false, name);

//          {
//            sprintf (name, "n%u %s", static_cast<unsigned>(i), plane_i.label.c_str());
////            sprintf (name, "n%u %.1f %.2f", static_cast<unsigned>(i), plane_i.curvature*1000, plane_i.areaHull);
//            viz.addText3D (name, pt2, 0.1, ared[i%10], agrn[i%10], ablu[i%10], name);
//          }

//          sprintf (name, "approx_plane_%02d", int (i));
//          viz.addPolygon<PointT> (plane_i.polygonContourPtr, 0.5 * red[i%10], 0.5 * grn[i%10], 0.5 * blu[i%10], name);

//          if(bColoredPlanes)
//          {
//            sprintf (name, "plane_%02u", static_cast<unsigned>(i));
//            pcl::visualization::PointCloudColorHandlerCustom <PointT> color (plane_i.planePointCloudPtr, red[i%10], grn[i%10], blu[i%10]);
//            viz.addPointCloud (plane_i.planePointCloudPtr, color, name);
//            viz.setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, name);
//          }

//        }
//      }

//      #if RECORD_VIDEO
//        std::string screenshotFile = mrpt::format("im_%04u.png", ++numScreenshot);
//        viz.saveScreenshot (screenshotFile);
//      #endif

//    }
//  }

//  /*! Get events from the keyboard */
//  void keyboardEventOccurred (const pcl::visualization::KeyboardEvent &event, void* viewer_void)
//  {
//    if ( event.keyDown() )
//    {
////      cout << "Key pressed " << event.getKeySym () << endl;
//      if(event.getKeySym() == "k" || event.getKeySym() == "K")
//        bShowPlanes = !bShowPlanes;
//      else if(event.getKeySym() == "l" || event.getKeySym() == "L"){//cout << " Press L\n";
//        bColoredPlanes = !bColoredPlanes;}
//      else if(event.getKeySym () == "a" || event.getKeySym () == "A"){
//        if(frameIdx <= 50)
//          frameIdx = 0;
//        else
//          frameIdx -= 50;
//        }
//      else if(event.getKeySym () == "s" || event.getKeySym () == "S"){
//          frameIdx += 50;}
////      else
////      {
////        cout << "\n\tASCII key: " << unsigned(event.getKeyCode()) << endl;
//////        string key_input = event.getKeySym();
////      }
//    }
//  }

//};


int main ( int argc, char** argv )
{
  try
  {
    if (argc != 2)
    {
      cerr << "Usage: " << argv[0] << " <path_to_rawlog_dataset\n";
      return 1;
    }

    const string RAWLOG_FILENAME = string( argv[1] );
    const unsigned num_sensors = 4;

    unsigned SensorArrangement[] = {1,3,2,0};

    // Set the sensor poses (Extrinsic calibration)
    mrpt::poses::CPose3D sensorPoses[num_sensors];
    math::CMatrixDouble44 pose_sensor_mat[num_sensors];
    Eigen::Matrix4d pose_sensor_mat0 = Eigen::Matrix4d::Identity(); pose_sensor_mat0.block(0,3,3,1) << 0.055, 0, 0;
    pose_sensor_mat[0] = math::CMatrixDouble44(pose_sensor_mat0);

    //TODO: Load proper calibration of the ominidirectional RGBD device
    Eigen::Matrix4d Rt_45 = Eigen::Matrix4d::Identity();
    Rt_45(0,0) = Rt_45(2,2) = cos(45*PI/180);
    Rt_45(0,2) = sin(45*PI/180);
    Rt_45(2,0) = -Rt_45(0,2);

    for(unsigned i=1; i < num_sensors; i++){
      pose_sensor_mat[i] = math::CMatrixDouble44(Rt_45) * pose_sensor_mat[i-1];
      cout << "Sensor pose \n" << pose_sensor_mat[i].getEigenBase() << endl;
    }

    for(unsigned i=0; i < num_sensors; i++)
      sensorPoses[i] = mrpt::poses::CPose3D(pose_sensor_mat[SensorArrangement[i]]);

    CFileGZInputStream rawlogFile(RAWLOG_FILENAME);
    CActionCollectionPtr action;
    CSensoryFramePtr observations;
    CObservationPtr observation;
    size_t rawlogEntry=0;
    //bool end = false;

    CObservation3DRangeScanPtr obsRGBD[4];  // Pointers to the 4 images that compose an observation
    bool rgbd1 = false, rgbd2 = false, rgbd3 = false, rgbd4 = false;
    CObservation2DRangeScanPtr laserObs;    // Pointer to the laser observation
    const int decimation = 1;
    int num_observations = 0, num_rgbd360_obs = 0;

    // Create window and prepare OpenGL object in the scene:
    // --------------------------------------------------------
    bool bVisualize = true;
    mrpt::gui::CDisplayWindow3D  win3D("OpenNI2 3D view",800,600);

    win3D.setCameraAzimuthDeg(140);
    win3D.setCameraElevationDeg(20);
    win3D.setCameraZoom(8.0);
    win3D.setFOV(90);
    win3D.setCameraPointingToPoint(2.5,0,0);

//    mrpt::opengl::CPointCloudColouredPtr gl_points = mrpt::opengl::CPointCloudColoured::Create();
//    gl_points->setPointSize(2.5);
    mrpt::opengl::CPointCloudColouredPtr gl_points[num_sensors];
    for(unsigned i=0; i < num_sensors; i++)
    {
      gl_points[i] = mrpt::opengl::CPointCloudColoured::Create();
      gl_points[i]->setPointSize(2.5);
    }

    opengl::COpenGLViewportPtr viewInt; // Extra viewports for the RGB images.
    {
      mrpt::opengl::COpenGLScenePtr &scene = win3D.get3DSceneAndLock();

      // Create the Opengl object for the point cloud:
//      scene->insert( gl_points );
      for(unsigned i=0; i < num_sensors; i++)
        scene->insert( gl_points[i] );

      scene->insert( mrpt::opengl::CGridPlaneXY::Create() );
      scene->insert( mrpt::opengl::stock_objects::CornerXYZ() );

      const double aspect_ratio =  480.0 / 640.0;
      const int VW_WIDTH = 400;	// Size of the viewport into the window, in pixel units.
      const int VW_HEIGHT = aspect_ratio*VW_WIDTH;

      // Create an extra opengl viewport for the RGB image:
//      viewInt = scene->createViewport("view2d_int");
//      viewInt->setViewportPosition(5, 30, VW_WIDTH,VW_HEIGHT );
      win3D.addTextMessage(10, 30+VW_HEIGHT+10,"Intensity data",TColorf(1,1,1), 2, MRPT_GLUT_BITMAP_HELVETICA_12 );

      win3D.addTextMessage(5,5,
        format("'o'/'i'-zoom out/in, ESC: quit"),
          TColorf(0,0,1), 110, MRPT_GLUT_BITMAP_HELVETICA_18 );

      win3D.unlockAccess3DScene();
      win3D.repaint();
    }


    while ( CRawlog::getActionObservationPairOrObservation(
                                                 rawlogFile,      // Input file
                                                 action,            // Possible out var: action of a pair action/obs
                                                 observations,  // Possible out var: obs's of a pair action/obs
                                                 observation,    // Possible out var: a single obs.
                                                 rawlogEntry    // Just an I/O counter
                                                 ) )
    {
      // Process action & observations
      if (observation)
      {
        // assert(IS_CLASS(observation, CObservation2DRangeScan) || IS_CLASS(observation, CObservation3DRangeScan));
        cout << "Observation " << num_observations++ << " timestamp " << observation->timestamp << endl;

        // TODO: Get closest frames in time (more tight synchronization)

        if(observation->sensorLabel == "RGBD1")
        {
          obsRGBD[0] = CObservation3DRangeScanPtr(observation);
          rgbd1 = true;
        }
        if(observation->sensorLabel == "RGBD2")
        {
          obsRGBD[1] = CObservation3DRangeScanPtr(observation);
          rgbd2 = true;
        }
        if(observation->sensorLabel == "RGBD3")
        {
          obsRGBD[2] = CObservation3DRangeScanPtr(observation);
          rgbd3 = true;
        }
        if(observation->sensorLabel == "RGBD4")
        {
          obsRGBD[3] = CObservation3DRangeScanPtr(observation);
          rgbd4 = true;
        }
        else if(observation->sensorLabel == "LASER")
        {
          laserObs = CObservation2DRangeScanPtr(observation);
        }
      }
      else
      {
      // action, observations should contain a pair of valid data (Format #1 rawlog file)
        THROW_EXCEPTION("Not a valid observation");
      }

      if(!(rgbd1 && rgbd2 && rgbd3 && rgbd4))
        continue;

      rgbd1 = rgbd2 = rgbd3 = rgbd4 = false; // Reset the counter of simultaneous observations

      // Apply decimation
      num_rgbd360_obs++;
      if(num_rgbd360_obs%decimation != 0)
        continue;

      // Fill the frame180 structure
      CObservationRGBD360 obs360;
      for(unsigned i=0; i<obs360.NUM_SENSORS; i++)
      {
        obs360.rgbd[i] = *obsRGBD[SensorArrangement[i]];
        obs360.rgbd[i].sensorPose = sensorPoses[SensorArrangement[i]];
      }

      // Segment surfaces (planes and curve regions)
//      obs360.getPlanes();

      // Visualize the data
      if(bVisualize)
      {
        // It IS a new observation:
        mrpt::system::TTimeStamp last_obs_tim = observation->timestamp;

        // Update visualization ---------------------------------------

        win3D.get3DSceneAndLock();

        // Estimated grabbing rate:
        win3D.addTextMessage(-350,-13, format("Timestamp: %s", mrpt::system::dateTimeLocalToString(last_obs_tim).c_str()), TColorf(0.6,0.6,0.6),"mono",10,mrpt::opengl::FILL, 100);

//        // Show intensity image:
//        if (obsRGBD[0]->hasIntensityImage )
//        {
//          viewInt->setImageView(obsRGBD[0]->intensityImage); // This is not "_fast" since the intensity image may be needed later on.
//        }
        win3D.unlockAccess3DScene();

        // -------------------------------------------------------
        //           Create 3D points from RGB+D data
        //
        // There are several methods to do this.
        //  Switch the #if's to select among the options:
        // See also: http://www.mrpt.org/Generating_3D_point_clouds_from_RGB_D_observations
        // -------------------------------------------------------
        {
          win3D.get3DSceneAndLock();
//            obsRGBD[0]->project3DPointsFromDepthImageInto(*gl_points, false /* without obs.sensorPose */);
            for(unsigned i=0; i < num_sensors; i++)
              obs360.rgbd[i].project3DPointsFromDepthImageInto(*gl_points[i], true);
//              obsRGBD[i]->project3DPointsFromDepthImageInto(*gl_points[i], true, &sensorPoses[i]);

          win3D.unlockAccess3DScene();
        }

        win3D.repaint();

        mrpt::system::pause();
      }

    };

    cout << "\n ... END rgbd360-visualizer ...\n";

    return 0;

	} catch (std::exception &e)
	{
		std::cout << "MRPT exception caught: " << e.what() << std::endl;
		return -1;
	}
	catch (...)
	{
		printf("Untyped exception!!");
		return -1;
	}
}
