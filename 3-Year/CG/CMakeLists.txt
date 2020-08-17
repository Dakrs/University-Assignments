cmake_minimum_required(VERSION 3.5)

# Project Name

project(engine)

set(ENGINE_FILES ./src/engine.cpp ./src/tinyxml2.cpp ./src/Point.cpp ./src/Figure.cpp ./src/Transformation.cpp src/Translate.cpp src/Rotate.cpp src/Scale.cpp src/Camera.cpp headers/Camera.h src/parser.cpp headers/parser.h src/Materials/Material.cpp headers/Material.h src/Materials/Diffuse.cpp headers/Diffuse.h src/Materials/Ambient.cpp headers/Ambient.h src/Materials/Specular.cpp headers/Specular.h src/Materials/Emissive.cpp headers/Emissive.h src/Light.cpp headers/Light.h)

add_executable(engine ${ENGINE_FILES})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)




if  (WIN32)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OPENGL_FOUND)
	message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

	set(TOOLKITS_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/toolkit")
	message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})

	if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
		message(ERROR ": GLUT not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")

	if (NOT EXISTS "${TOOLKITS_FOLDER}/glew/GL/glew.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glew/glew32.lib")
		message(ERROR ": GLEW not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/glew/GL/glew.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glew/glew32.lib")

	if (NOT EXISTS "${TOOLKITS_FOLDER}/devil/IL/il.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/devil/devIL.lib")
		message(ERROR ": DevIL not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/devil/IL/il.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/devil/devIL.lib")

	include_directories(${TOOLKITS_FOLDER}/glut ${TOOLKITS_FOLDER}/glew ${TOOLKITS_FOLDER}/devil)
	target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES}
			${TOOLKITS_FOLDER}/glut/glut32.lib
			${TOOLKITS_FOLDER}/devil/devIL.lib
			${TOOLKITS_FOLDER}/glew/glew32.lib )

	if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" AND EXISTS "${TOOLKITS_FOLDER}/glew/glew32.dll"
			AND EXISTS "${TOOLKITS_FOLDER}/devil/devil.dll")
		file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
		file(COPY ${TOOLKITS_FOLDER}/devil/devil.dll DESTINATION ${CMAKE_BINARY_DIR})
		file(COPY ${TOOLKITS_FOLDER}/glew/glew32.dll DESTINATION ${CMAKE_BINARY_DIR})
	endif(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" AND EXISTS "${TOOLKITS_FOLDER}/glew/glew32.dll"
			AND EXISTS "${TOOLKITS_FOLDER}/devil/devil.dll")

	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

else (WIN32) #Linux and Mac


	find_package(GLUT REQUIRED)
	include_directories(${GLUT_INCLUDE_DIRS})
	link_directories(${GLUT_LIBRARY_DIRS})
	add_definitions(${GLUT_DEFINITIONS})

	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-deprecated")
	set_source_files_properties(${sources} PROPERTIES COMPILE_FLAGS
		"-xobjective-c -Wno-deprecated-declarations")
	find_package(OpenGL REQUIRED)
	include_directories(${OpenGL_INCLUDE_DIRS})
	link_directories(${OpenGL_LIBRARY_DIRS})
	add_definitions(${OpenGL_DEFINITIONS})



	find_package(DevIL REQUIRED)
	link_libraries(${IL_LIBRARIES} )
	include_directories(${IL_INCLUDE_DIR})


	if (NOT APPLE)
		find_package(GLEW REQUIRED)
		include_directories(${GLEW_INCLUDE_DIRS})
		link_libraries(${GLEW_LIBRARIES})
		target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARY} ${IL_LIBRARIES})
	else (NOT APPLE)
		target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${IL_LIBRARIES})
	endif(NOT APPLE)


	if(NOT OPENGL_FOUND)
		message(ERROR " OPENGL not found!")
	endif(NOT OPENGL_FOUND)
endif(WIN32)


project(generator)
set(GENERATOR_FILES ./src/generator.cpp ./src/Point.cpp ./src/Polygon.cpp ./src/Transformation.cpp ./src/Patch.cpp ./src/Translate.cpp)

add_executable(generator ${GENERATOR_FILES} )
target_link_libraries(generator ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

set_property(GLOBAL PROPERTY USE_FOLDERS ON)




if  (WIN32)

	find_package(OpenGL REQUIRED)
	include_directories(${OpenGL_INCLUDE_DIRS})
	link_directories(${OpenGL_LIBRARY_DIRS})
	add_definitions(${OpenGL_DEFINITIONS})

	if(NOT OPENGL_FOUND)
		message(ERROR " OPENGL not found!")
	endif(NOT OPENGL_FOUND)

	set(TOOLKITS_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/toolkit")
	message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})

	if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
		message(ERROR ": GLUT not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")

	if (NOT EXISTS "${TOOLKITS_FOLDER}/glew/GL/glew.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glew/glew32.lib")
		message(ERROR ": GLEW not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/glew/GL/glew.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glew/glew32.lib")

	if (NOT EXISTS "${TOOLKITS_FOLDER}/devil/IL/il.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/devil/devIL.lib")
		message(ERROR ": DevIL not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/devil/IL/il.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/devil/devIL.lib")

	include_directories(${TOOLKITS_FOLDER}/glut ${TOOLKITS_FOLDER}/glew ${TOOLKITS_FOLDER}/devil)
	target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES}
			${TOOLKITS_FOLDER}/glut/glut32.lib
			${TOOLKITS_FOLDER}/devil/devIL.lib
			${TOOLKITS_FOLDER}/glew/glew32.lib )

	if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" AND EXISTS "${TOOLKITS_FOLDER}/glew/glew32.dll"
			AND EXISTS "${TOOLKITS_FOLDER}/devil/devil.dll")
		file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
		file(COPY ${TOOLKITS_FOLDER}/devil/devil.dll DESTINATION ${CMAKE_BINARY_DIR})
		file(COPY ${TOOLKITS_FOLDER}/glew/glew32.dll DESTINATION ${CMAKE_BINARY_DIR})
	endif(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" AND EXISTS "${TOOLKITS_FOLDER}/glew/glew32.dll"
			AND EXISTS "${TOOLKITS_FOLDER}/devil/devil.dll")

	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif( WIN32)