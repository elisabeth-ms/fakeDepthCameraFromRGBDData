# How to use fakeDepthCameraFromRGBDData device?

Installation:

```
git clone https://github.com/elisabeth-ms/fakeDepthCameraFromRGBDData.git  # Download the repository
cd fakeDepthCameraFromRGBDData && mkdir build && cd build && cmake ..
make -j$(nproc)  # compile
sudo make install  # Install 
```

The fakeDepthCameraFromRGBDData device acts as a fake rgbd camera. It gathers color and images from a directory folder and it publishes both images through yarp ports.

Launch a RGBDSensorWrapper with the wrapped subdevice fakeDepthCameraFromRGBDDataDriver.
```
yarpdev --device RGBDSensorWrapper --name /realsense2 --subdevice fakeDepthCameraFromRGBDDataDriver
```
The parameters:
- device: network wrapper device (RGBDSensorWrapper). 
- name:  name of the YARP port opened by the server will start with this prefix. /realsense2 for faking the real camera.
- subdevice:  the low-level device driver reading the images (the fakeDepthCameraFromRGBDDataDriver device in this case)

Open yarpview GUIS to visualize the images received by the fakeDepthCameraFromRGBDDataDriver.
```
yarpview --name /yarpview/realsense2/depthImage:i
yarpview --name /yarpview/realsense2/rgbImage:i
```

Connect the yarp ports from the fakeDepthCameraFromRGBDDataDriver to the viewers.

```
yarp connect /realsense2/rgbImage:o /yarpview/realsense2/rgbImage:i tcp

yarp connect /realsense2/depthImage:o /yarpview/realsense2/depthImage:i udp+recv.portmonitor+type.dll+file.depthimage_to_rgb
```

Expected result:
![alt text](https://github.com/elisabeth-ms/fakeDepthCameraFromRGBDData/blob/master/color_depth_viewers.png?raw=true)

# How to save images from xtion rgbd camera connected with usb? 
Connect the xtion camera to a usb port and run ./getImagesXtionUsb. It stores the color(.jpg) and depth (.png) images in a specific folder.
