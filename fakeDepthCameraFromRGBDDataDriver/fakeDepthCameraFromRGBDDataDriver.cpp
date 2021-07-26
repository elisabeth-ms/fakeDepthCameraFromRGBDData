#include "fakeDepthCameraFromRGBDDataDriver.h"

#include <yarp/os/LogComponent.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Value.h>

#include <algorithm>
#include <map>
#include <cmath>

#include <opencv2/opencv.hpp>

//using namespace yarp::dev;
//using namespace yarp::sig;
//using namespace yarp::os;
using namespace cv;
using namespace std;

namespace {
YARP_LOG_COMPONENT(FAKEDEPTHCAMERAFROMRGBDDATA, "yarp.device.fakeDepthCameraFromRGBDDataDriver")
}

yarp::dev::fakeDepthCameraFromRGBDDataDriver::fakeDepthCameraFromRGBDDataDriver():
    rgb_h(480),
    rgb_w(640),
    dep_h(480),
    dep_w(640),
    rgb_Vfov(36),
    rgb_Hfov(50),
    dep_Vfov(36),
    dep_Hfov(50),
    dep_far(6),
    first(true),
    image(nullptr)
{}

yarp::dev::fakeDepthCameraFromRGBDDataDriver::~fakeDepthCameraFromRGBDDataDriver(){
 close();
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::open(yarp::os::Searchable& config)
{
    Property cfg;
    cfg.fromString(config.toString());
    yInfo()<<"Lets see the configuration";
    yInfo()<<config.toString();
    cfg.unput("device");
    cfg.put("device", "fakeFrameGrabber");

    std::string rootName;
    rootName = config.check("name",yarp::os::Value("/"), "starting '/' if needed.").asString();

    if (!config.check("name", "Prefix name of the ports opened by the RGBD wrapper.")) {
            yError() << "Missing 'name' parameter. Check you configuration file; it must be like:";
            yError() << "   name:         Prefix name of the ports opened by the RGBD wrapper, e.g. /robotName/RGBD";
            return false;
    }

    rootName = config.find("name").asString();

    yInfo()<<"Lets see the configuration";
	
    testgrabber.open(cfg);
    testgrabber.view(image);
    std::string startFrame ="0";
    frame.insert(0,startFrame);


    //portPrefix += strRGBDRemote;
    if(!imageNamePort.open(rootName + "/imageName"))
    {
        yError("Bad imageNamePort.open\n");
        return false;
    }


//    vector<tuple<double*, string, double> > param;
//    param.emplace_back(&rgb_h,    "rgb_h",    480.0);
//    param.emplace_back(&rgb_w,    "rgb_w",    640.0);
//    param.emplace_back(&dep_h,    "rgb_h",    480.0);
//    param.emplace_back(&dep_w,    "rgb_w",    640.0);
//    param.emplace_back(&accuracy, "accuracy", 0.001);
//    param.emplace_back(&rgb_Vfov, "rgb_Vfov", 50.0);
//    param.emplace_back(&rgb_Hfov, "rgb_Hfov", 36.0);
//    param.emplace_back(&dep_Vfov, "dep_Vfov", 50.0);
//    param.emplace_back(&dep_Hfov, "dep_Hfov", 36.0);
//    param.emplace_back(&dep_near, "dep_near", 0.2);
//    param.emplace_back(&dep_far,  "dep_far",  6.0);
//    for (auto p : param)
//    {
//        if (config.check(get<1>(p)))
//        {
//            *get<0>(p) = config.find(get<1>(p)).asFloat64();
//        }
//        else
//        {
//            *get<0>(p) = get<2>(p);
//        }

//    }

    return true;
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::close()
{
    return true;
}

int yarp::dev::fakeDepthCameraFromRGBDDataDriver::getRgbHeight()
{
    return image->height();
}

int yarp::dev::fakeDepthCameraFromRGBDDataDriver::getRgbWidth()
{
    return image->width();
}


bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::getRgbSupportedConfigurations(yarp::sig::VectorOf<CameraConfig> &configurations)
{
    yCWarning(FAKEDEPTHCAMERAFROMRGBDDATA) << "getRgbSupportedConfigurations not implemented yet";
    return false;
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::getRgbResolution(int &width, int &height)
{
    width  = image->width();
    height = image->height();
    return true;
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::setRgbResolution(int width, int height)
{
    return false;
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::setDepthResolution(int width, int height)
{
    return false;
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::setRgbFOV(double horizontalFov, double verticalFov)
{
    rgb_Hfov = horizontalFov;
    rgb_Vfov = verticalFov;
    return true;
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::setDepthFOV(double horizontalFov, double verticalFov)
{
    dep_Hfov = horizontalFov;
    dep_Vfov = verticalFov;
    return true;
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::setDepthAccuracy(double in_accuracy)
{
    accuracy = in_accuracy;
    return true;
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::getRgbFOV(double &horizontalFov, double &verticalFov)
{
    horizontalFov = rgb_Hfov;
    verticalFov   = rgb_Vfov;
    return false;
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::getRgbMirroring(bool& mirror)
{
    mirror = false;
    return true;
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::setRgbMirroring(bool mirror)
{
    return false;
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::getRgbIntrinsicParam(Property& intrinsic)
{
    intrinsic.put("physFocalLength", 0.5);
    intrinsic.put("focalLengthX",    512);
    intrinsic.put("focalLengthY",    512);
    intrinsic.put("principalPointX", 235);
    intrinsic.put("principalPointY", 231);
    intrinsic.put("distortionModel", "plumb_bob");
    intrinsic.put("k1", 0);
    intrinsic.put("k2", 0);
    intrinsic.put("t1", 0);
    intrinsic.put("t2", 0);
    intrinsic.put("k3", 0);

    intrinsic.put("stamp", yarp::os::Time::now());
    return true;
}

int  yarp::dev::fakeDepthCameraFromRGBDDataDriver::getDepthHeight()
{
    return image->height();
}

int  yarp::dev::fakeDepthCameraFromRGBDDataDriver::getDepthWidth()
{
    return image->width();
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::getDepthFOV(double& horizontalFov, double& verticalFov)
{
    horizontalFov = dep_Hfov;
    verticalFov   = dep_Vfov;
    return false;
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::getDepthIntrinsicParam(Property& intrinsic)
{
    intrinsic.put("physFocalLength", 0.5);
    intrinsic.put("focalLengthX",    525);
    intrinsic.put("focalLengthY",    525);
    intrinsic.put("principalPointX", 319.5);
    intrinsic.put("principalPointY", 239.5);
    intrinsic.put("distortionModel", "depth_distortion");
    intrinsic.put("k1", 0);
    intrinsic.put("k2", 0);
    intrinsic.put("t1", 0);
    intrinsic.put("t2", 0);
    intrinsic.put("k3", 0);

    intrinsic.put("stamp", yarp::os::Time::now());
    return true;
}

double yarp::dev::fakeDepthCameraFromRGBDDataDriver::getDepthAccuracy()
{
    return accuracy;
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::getDepthClipPlanes(double& nearPlane, double& farPlane)
{
    nearPlane = dep_near;
    farPlane  = dep_far;
    return true;
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::setDepthClipPlanes(double nearPlane, double farPlane)
{
    dep_near = nearPlane;
    dep_far  = farPlane;
    return true;

}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::getDepthMirroring(bool& mirror)
{
    mirror = false;
    return true;
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::setDepthMirroring(bool mirror)
{
    return false;
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::getExtrinsicParam(yarp::sig::Matrix& extrinsic)
{
    extrinsic.resize(4, 4);
    extrinsic.zero();

    extrinsic[0][0] = 1;
    extrinsic[1][1] = 1;
    extrinsic[2][2] = 1;
    extrinsic[3][3] = 1;
    return true;
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::getRgbImage(FlexImage& rgbImage, Stamp* timeStamp)
{
    if(first){
            frame.clear();
            frame.insert(0,"1");
            currentTime = yarp::os::Time::now();
            previousTime = yarp::os::Time::now();
            first = false;
    }

    // Check current time
    currentTime = yarp::os::Time::now();

    if(currentTime-previousTime>4.0)
    {   int iFrame = std::stoi(frame,nullptr,10);
        iFrame++;
        yInfo()<<"iFrame: "<<iFrame;
        std::string strFrame = std::to_string(iFrame);
        frame.clear();
        frame.insert(0,strFrame);
        previousTime = currentTime;
    }

    std::string rgbImageDirectory = "/home/elisabeth/data/teoImages/color/";
    std::string imageTest;
    imageTest.insert(0,rgbImageDirectory);
    imageTest.append(frame);
    imageTest.append(".jpg");
    //yInfo()<<frame;
    //yInfo()<<imageTest;
    Mat inputImage = imread(imageTest, IMREAD_COLOR);
    if(!inputImage.data)
    {
       yError()<<"Could not open or find the rgb image " << imageTest;
       return false;
    }
    auto yarpReturnImage= yarp::cv::fromCvMat<yarp::sig::PixelRgb>(inputImage);

    rgbImage.setPixelCode(yarpReturnImage.getPixelCode());
    rgbImage.resize(yarpReturnImage.width(),yarpReturnImage.height());
    rgbImage.setQuantum(yarpReturnImage.getQuantum());
    // Without setQuantum this the image appears with weird colors (grey mostly), and the image is also streched.
    // This is because, with the quantum value we fix the size of a row.
    // From the getCuantum documentation: the size of a row is constrained to be a multiple of the "quantum".
    memcpy((void*)rgbImage.getRawImage(), (void*)yarpReturnImage.getRawImage(), yarpReturnImage.getRawImageSize());
    if(timeStamp) timeStamp->update(yarp::os::Time::now());
    yarp::os::Bottle bot;
    bot.addString(frame+".jpg");
    imageNamePort.write(bot);

    return true;
}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::getDepthImage(yarp::sig::ImageOf<yarp::sig::PixelFloat>& depthImage, Stamp* timeStamp)
{

    std::string depthImageDirectory = "/home/elisabeth/data/teoImages/depth/";
    std::string imageTest;
    imageTest.insert(0,depthImageDirectory);
    imageTest.append(frame);
    imageTest.append(".png");

    Mat inputImage = imread(imageTest, IMREAD_ANYDEPTH);
    if(!inputImage.data)
    {
       yError()<<"Could not open or find the depth image " << imageTest;
       return false;
    }

    float  factor = 0.001;

    short* srcRawImage;
    srcRawImage = inputImage.ptr<short>();


    depthImage.resize(640,480);
    
    float* rawImage;
    rawImage = reinterpret_cast<float*> (depthImage.getRawImage());
    for(int i = 0; i < depthImage.width()*depthImage.height(); i++)
    {
        rawImage[i] = srcRawImage[i] * factor;
    }


    /**
    yarp::sig::ImageOf<yarp::sig::PixelFloat> depth_image;
    if (!yarp::sig::file::read(depthImage,imageTest))
    {
         yError()<<"Could not open or find depth image";
         return false;
     }
    **/

    //yInfo()<<"Depth image found";
    if(timeStamp)
        timeStamp->update(yarp::os::Time::now());
    return true;

}

bool yarp::dev::fakeDepthCameraFromRGBDDataDriver::getImages(FlexImage& colorFrame, yarp::sig::ImageOf<yarp::sig::PixelFloat>& depthFrame, Stamp* colorStamp, Stamp* depthStamp)
{
    return getRgbImage(colorFrame, colorStamp) & getDepthImage(depthFrame, depthStamp);
}

yarp::dev::IRGBDSensor::RGBDSensor_status yarp::dev::fakeDepthCameraFromRGBDDataDriver::getSensorStatus()
{
    return RGBD_SENSOR_OK_IN_USE;
}

std::string yarp::dev::fakeDepthCameraFromRGBDDataDriver::getLastErrorMsg(Stamp* timeStamp)
{
    return "no error";
}
