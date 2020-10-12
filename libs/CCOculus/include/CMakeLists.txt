
target_sources( ${PROJECT_NAME}
	PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/CCOculus.h
		${CMAKE_CURRENT_LIST_DIR}/ccOculusWrapper.h
)


target_include_directories( ${PROJECT_NAME}
	PUBLIC
		${CMAKE_CURRENT_LIST_DIR}
)
