#include "opencv2/opencv.hpp"
#include <OpenNI.h>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{

  if(argc == 2){
    int index = atoi(argv[1]); 
    string imagesFolder = "/home/elisabeth/data/teoImages/";
    openni::Status rc;


    openni::Device device;
    openni::VideoStream depth, color,ir;

    openni::OpenNI::initialize();

    rc=device.open(openni::ANY_DEVICE);
    if(rc!=openni::STATUS_OK){
      printf("Error! %d\n\n",rc);
      openni::OpenNI::shutdown();
      exit(0);
    }
    cout<<"Asus Xtion opened"<<endl;
    ir.create(device,openni::SENSOR_IR);
    depth.create(device, openni::SENSOR_DEPTH);
    color.create(device, openni::SENSOR_COLOR);



  	rc = depth.create(device, openni::SENSOR_DEPTH);
	  if (rc == openni::STATUS_OK)
	  {
      openni::VideoMode depthVideoMode  = depth.getVideoMode();
      depthVideoMode.setResolution(640,480);
      depthVideoMode.setFps(30);
      rc = depth.setVideoMode(depthVideoMode);
		  rc = depth.start();
		  if (rc != openni::STATUS_OK)
		  {
			  printf("SimpleViewer: Couldn't start depth stream:\n%s\n", openni::OpenNI::getExtendedError());
			  depth.destroy();
		  }
	  }
	  else
	  {
		  printf("SimpleViewer: Couldn't find depth stream:\n%s\n", openni::OpenNI::getExtendedError());
	  }


	  rc = color.create(device, openni::SENSOR_COLOR);
	  if (rc == openni::STATUS_OK)
  	{
      openni::VideoMode colorVideoMode  = color.getVideoMode();
      colorVideoMode.setResolution(640,480);
      colorVideoMode.setFps(30);
      rc = color.setVideoMode(colorVideoMode);

      if (openni::STATUS_OK != rc)
      {
        std::cout << "error: color format not supported..." << std::endl;
      }

		  rc = color.start();
		  if (rc != openni::STATUS_OK)
		  {
		  	printf("Couldn't get color stream:\n%s\n", openni::OpenNI::getExtendedError());
			  color.destroy();
		  }
	  }
	  else
	  {
		  printf("Couldn't get color stream:\n%s\n", openni::OpenNI::getExtendedError());
	  }

    openni::VideoFrameRef colorFrame, depthFrame;
    cv::Mat framecolor, framedepth;
    while (true)
    {
        color.readFrame(&colorFrame);
        
        const openni::RGB888Pixel* imageBuffer = (const openni::RGB888Pixel*)colorFrame.getData();

        framecolor.create(colorFrame.getHeight(), colorFrame.getWidth(), CV_8UC3);
        memcpy(framecolor.data, imageBuffer, 3 * colorFrame.getHeight()*colorFrame.getWidth() * sizeof(uint8_t));

        cv::cvtColor(framecolor, framecolor, cv::COLOR_BGR2RGB); //this will put colors right
        cv::imshow("framecolor", framecolor);

        depth.readFrame(&depthFrame);

        const openni::DepthPixel* imageBuffer2 = (const openni::DepthPixel*)depthFrame.getData();

        framedepth.create(depthFrame.getHeight(), depthFrame.getWidth(), CV_16U);

        memcpy(framedepth.data, imageBuffer2, depthFrame.getHeight()*depthFrame.getWidth() * sizeof(uint16_t));

        // framedepth.convertTo(framedepth, CV_8U);

        cv::imshow("framedepth", framedepth);

        if (cv::waitKey(30)>=0)
        {
          string colorImageFile = imagesFolder+"color/"+to_string(index)+".jpg";
          string depthImageFile = imagesFolder+"depth/"+to_string(index)+".png";
          std::cout<<"Saving color image "<<colorImageFile<<std::endl;
          imwrite(colorImageFile, framecolor); // A JPG FILE IS BEING SAVED
          std::cout<<"Saving depth image "<<depthImageFile<<std::endl;
          imwrite(depthImageFile, framedepth); // A JPG FILE IS BEING SAVED
          index++;
        }
    }
  
  }
    cv::destroyAllWindows();
    return 0;

}
