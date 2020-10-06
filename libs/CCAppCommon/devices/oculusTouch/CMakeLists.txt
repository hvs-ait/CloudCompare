# Oculus touch support
if ( ${OPTION_USE_OCULUS_SDK} )
    #set(header_list
    #   ${header_list}
    #   ${CMAKE_CURRENT_SOURCE_DIR}/ccOculusControllerManager.h
    #   ${CMAKE_CURRENT_SOURCE_DIR}/ccOculusController.h
    #   ${CMAKE_CURRENT_SOURCE_DIR}/ccOculusTouch.h
    #   PARENT_SCOPE
    #)

#    set(source_list
 #      ${source_list}
  #     ${CMAKE_CURRENT_SOURCE_DIR}/ccOculusControllerManager.cpp
   #    ${CMAKE_CURRENT_SOURCE_DIR}/ccOculusTouch.cpp
    #   PARENT_SCOPE
    #)
    target_sources( ${PROJECT_NAME}
		PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/ccOculusTouch.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ccOculusControllerManager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ccOculusController.h
		${CMAKE_CURRENT_SOURCE_DIR}/ccOculusControllerManager.h
		${CMAKE_CURRENT_SOURCE_DIR}/ccOculusTouch.h
	)

	target_include_directories( ${PROJECT_NAME}
		PUBLIC
			${CMAKE_CURRENT_LIST_DIR}
	)
    
    target_compile_definitions( ${PROJECT_NAME} PUBLIC CC_OCULUS_TOUCH_SUPPORT )
endif()
