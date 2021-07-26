
yarp_prepare_plugin(fakeDepthCameraFromRGBDDataDriver
                    CATEGORY device
                    TYPE yarp::dev::fakeDepthCameraFromRGBDDataDriver
                    INCLUDE fakeDepthCameraFromRGBDDataDriver.h
		    EXTRA_CONFIG WRAPPER=RGBDSensorWrapper
		    DEFAULT ON)

if(ENABLE_fakeDepthCameraFromRGBDDataDriver)


  yarp_add_plugin(fakeDepthCameraFromRGBDDataDriver)

  target_sources(fakeDepthCameraFromRGBDDataDriver PRIVATE fakeDepthCameraFromRGBDDataDriver.cpp
                                              fakeDepthCameraFromRGBDDataDriver.h)
  include_directories( ${OpenCV_INCLUDE_DIRS} )
  target_link_libraries(fakeDepthCameraFromRGBDDataDriver PRIVATE YARP::YARP_os
                                                     YARP::YARP_sig
                                                     YARP::YARP_dev
                                                     ${OpenCV_LIBRARIES})

  list(APPEND YARP_${YARP_PLUGIN_MASTER}_PRIVATE_DEPS YARP_os
                                                      YARP_sig
                                                      YARP_dev ${OpenCV_LIBRARIES})
  yarp_install(TARGETS fakeDepthCameraFromRGBDDataDriver
               EXPORT YARP_${YARP_PLUGIN_MASTER}
               COMPONENT ${YARP_PLUGIN_MASTER}
               LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
               ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR}
	       YARP_INI DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR})


  set(YARP_${YARP_PLUGIN_MASTER}_PRIVATE_DEPS ${YARP_${YARP_PLUGIN_MASTER}_PRIVATE_DEPS} PARENT_SCOPE)
  yarp_install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/fakeDepthCameraFromRGBDDataDriver.ini"  DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR})
  #set_property(TARGET fakeDepthCameraFromRGBDDataDriver PROPERTY FOLDER "Plugins")
endif()
