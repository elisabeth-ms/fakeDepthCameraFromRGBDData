find_library(OPENNI2_LIBRARY
    	NAMES OpenNI
    	PATHS /usr/lib)

find_path (OPENNI2_INCLUDE_DIR OpenNI.h /usr/include/openni2)

add_executable(getImagesXtionUsb getImagesXtionUsb.cpp)

link_directories(${OPENNI2_LIBRARY})
INCLUDE_DIRECTORIES(${OPENNI2_INCLUDE_DIR})
target_link_libraries(getImagesXtionUsb ${OPENNI2_LIBRARY} ${OpenCV_LIBRARIES})



