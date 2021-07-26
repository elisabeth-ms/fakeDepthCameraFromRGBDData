cmake_minimum_required(VERSION 3.5)
project(fakeDepthCameraFromRGBDData)

set (CMAKE_CXX_STANDARD 11)

find_package(YARP 3.3 REQUIRED COMPONENTS os dev sig)
find_package(YCM)
find_package(OpenCV REQUIRED)
find_package(PCL 1.8 QUIET)


# Focal + PCL 1.10 + OpenNI workaround (idea: use CMake components to find_package(PCL)?).
if(PCL_VERSION VERSION_EQUAL 1.10)
    include_directories(/usr/include/ni)
    link_libraries(/usr/lib/libOpenNI.so)
endif()
list(APPEND CMAKE_MODULE_PATH ${YARP_MODULE_PATH})

include(YarpPlugin)
include(YarpInstallationHelpers)


option(BUILD_SHARED_LIBS ON)
set(YARP_FORCE_DYNAMIC_PLUGINS TRUE)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

yarp_configure_plugins_installation(yarp-device-fake-depth-camera-from-rgbd-data)

add_subdirectory(fakeDepthCameraFromRGBDDataDriver)
add_subdirectory(getImagesXtionUsb)
add_subdirectory(share)
