project( CCOculus )

add_library( ${PROJECT_NAME} SHARED ) 

add_subdirectory( include )
add_subdirectory( src )

target_compile_definitions( ${PROJECT_NAME} PRIVATE CCOCULUS_LIB_BUILD )

target_link_libraries( ${PROJECT_NAME}
    CCCoreLib
)

target_link_OCULUS_SDK( ${PROJECT_NAME} )
InstallSharedLibrary( TARGET ${PROJECT_NAME} )
