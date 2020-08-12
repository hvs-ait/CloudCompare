set(header_list
   ${header_list}
   ${CMAKE_CURRENT_SOURCE_DIR}/ccOculusControllerManager.h
   ${CMAKE_CURRENT_SOURCE_DIR}/ccOculusController.h
   ${CMAKE_CURRENT_SOURCE_DIR}/ccOculusTouch.h
   PARENT_SCOPE
)

set(source_list
   ${source_list}
   ${CMAKE_CURRENT_SOURCE_DIR}/ccOculusControllerManager.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/ccOculusController.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/ccOculusTouch.cpp
   PARENT_SCOPE
)