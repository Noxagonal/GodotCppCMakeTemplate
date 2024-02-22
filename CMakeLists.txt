
cmake_minimum_required(VERSION 3.20)



function(get_last_folder_name PATH RESULT_VARIABLE)

	# Extract the last folder name from GODOT_PROJECT_FOLDER_PATH
	# Use file(TO_CMAKE_PATH ...) if the path might contain mixed separators (/, \)
	file(TO_CMAKE_PATH "${PATH}" NORMALIZED_PATH)

	# Convert the path into a list
	string(REPLACE "/" ";" PATH_LIST ${NORMALIZED_PATH})

	# Get the last element of the list, which is the project name
	list(GET PATH_LIST -1 FOLDER_NAME)

	set(${RESULT_VARIABLE} ${FOLDER_NAME} PARENT_SCOPE)

endfunction()

function(get_godot_version GODOT_PATH RESULT_VARIABLE)

	execute_process(
		COMMAND ${GODOT_PATH} --version
		RESULT_VARIABLE GODOT_EXIT_CODE
		OUTPUT_VARIABLE GODOT_VERSION_OUTPUT
	)

	if(GODOT_EXIT_CODE EQUAL "0")
		message(STATUS "Found Godot version: ${GODOT_VERSION_OUTPUT}")
	else()
		message(FATAL_ERROR "Failed to get Godot version: ${GODOT_EXIT_CODE}")
	endif()

	set(${RESULT_VARIABLE} ${GODOT_VERSION_OUTPUT} PARENT_SCOPE)

endfunction()

function(get_godot_major_minor_version GODOT_PATH RESULT_VARIABLE)

	get_godot_version(${GODOT_PATH} GODOT_VERSION_FULL)

	# Replace periods with semicolons to create a list
	string(REPLACE "." ";" VERSION_LIST ${GODOT_VERSION_FULL})

	# Get the major and minor version numbers
	list(GET VERSION_LIST 0 MAJOR_VERSION)
	list(GET VERSION_LIST 1 MINOR_VERSION)

	# Concatenate the major and minor versions
	set(GODOT_MAJOR_MINOR_VERSION "${MAJOR_VERSION}.${MINOR_VERSION}")

	set(${RESULT_VARIABLE} ${GODOT_MAJOR_MINOR_VERSION} PARENT_SCOPE)

endfunction()

function(get_godot_project_name PROJECT_FOLDER_PATH RESULT_VARIABLE)

	set(GODOT_PROJECT_FILE "${PROJECT_FOLDER_PATH}/project.godot")

	if(NOT EXISTS ${GODOT_PROJECT_FILE})
		message(FATAL_ERROR "project.godot file not found in ${PROJECT_FOLDER_PATH}")
	endif()

	# Read the file and parse the project name
	file(READ ${GODOT_PROJECT_FILE} PROJECT_FILE_CONTENT)

	# Use regular expressions to find the project name inside the godot project file
	string(REGEX MATCH "config/name=\"([^\"]+)\"" _ ${PROJECT_FILE_CONTENT})
	set(RAW_PROJECT_NAME ${CMAKE_MATCH_1})

	message(STATUS "Found Godot project name: ${RAW_PROJECT_NAME}")

	string(REPLACE " " "_" PROJECT_NAME ${RAW_PROJECT_NAME})

	set(${RESULT_VARIABLE} ${PROJECT_NAME} PARENT_SCOPE)

endfunction()

function(copy_godot_extension_to_godot_project_folder PROJECT_FOLDER_PATH PROJECT_NAME LIBRARY_ENTRY_SYMBOL MAJOR_MINOR_VERSION)

	set(TEMPLATE_GDEXTENSION_PATH "${PROJECT_SOURCE_DIR}/godot_extension/template.gdextension")
	set(DESTINATION_FOLDER "${PROJECT_FOLDER_PATH}/bin")
	set(DESTINATION_FILE "${DESTINATION_FOLDER}/${PROJECT_NAME}.gdextension")

	# Read contents of the template file.
	file(READ ${TEMPLATE_GDEXTENSION_PATH} FILE_CONTENTS)

	# Replace placeholders of the gdextension file with actual proper names.
	string(REPLACE "<LIBRARY_ENTRY_SYMBOL>" "${LIBRARY_ENTRY_SYMBOL}" UPDATED_FILE_CONTENTS "${FILE_CONTENTS}")
	string(REPLACE "<GODOT_MAJOR_MINOR_VERSION>" "${MAJOR_MINOR_VERSION}" UPDATED_FILE_CONTENTS "${UPDATED_FILE_CONTENTS}")
	string(REPLACE "<GODOT_CPP_PROJECT_NAME>" "${PROJECT_NAME}" UPDATED_FILE_CONTENTS "${UPDATED_FILE_CONTENTS}")

	# Create new gdextension file at destination location and copy updated contents to it.
	file(WRITE ${DESTINATION_FILE} ${UPDATED_FILE_CONTENTS})

endfunction()



# Setup project executable and project paths
set(GODOT_EXECUTABLE_PATH "" CACHE STRING "Godot executable path")
set(GODOT_PROJECT_FOLDER_PATH "" CACHE STRING "Godot project path")

message("GODOT_EXECUTABLE_PATH: ${GODOT_EXECUTABLE_PATH}")
message("GODOT_PROJECT_FOLDER_PATH: ${GODOT_PROJECT_FOLDER_PATH}")

if(GODOT_EXECUTABLE_PATH STREQUAL "")
	message(FATAL_ERROR "GODOT_EXECUTABLE_PATH is not set. Please provide a path to Godot executable with '-D GODOT_EXECUTABLE_PATH=\"<path>\"'.")
endif()
if(GODOT_PROJECT_FOLDER_PATH STREQUAL "")
	message(FATAL_ERROR "GODOT_PROJECT_FOLDER_PATH is not set. Please provide a path to Godot project folder with '-D GODOT_PROJECT_FOLDER_PATH=\"<path>\"'.")
endif()


get_godot_major_minor_version(${GODOT_EXECUTABLE_PATH} GODOT_MAJOR_MINOR_VERSION)
get_godot_project_name(${GODOT_PROJECT_FOLDER_PATH} GODOT_CPP_PROJECT_NAME)
message(STATUS "Godot C++ project name set to: ${GODOT_CPP_PROJECT_NAME}")

project(${GODOT_CPP_PROJECT_NAME} LANGUAGES CXX)



# Set C++ and C standards
set(CMAKE_CXX_STANDARD				17)
set(CMAKE_C_STANDARD				17)

set(CMAKE_CXX_STANDARD_REQUIRED		ON)
set(CMAKE_CXX_EXTENSIONS			OFF)
set(CMAKE_C_STANDARD_REQUIRED		ON)
set(CMAKE_C_EXTENSIONS				OFF)

# Disable in source build.
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/bin")



# Use multiple cores when compiling, not automatically enabled in Visual Studio. 
if(MSVC)
	add_compile_options(
		"/MP"
	)
endif()



add_subdirectory("submodules")



# Get Sources
file(GLOB_RECURSE PRIVATE_SOURCE_FILES CONFIGURE_DEPENDS src/*)

# Set source group for private files
foreach(SOURCE_FILE IN ITEMS ${PRIVATE_SOURCE_FILES})
	get_filename_component(SOURCE_FILE_PATH "${SOURCE_FILE}" PATH)
	file(RELATIVE_PATH SOURCE_FILE_RELATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${SOURCE_FILE_PATH}")
	source_group("${SOURCE_FILE_RELATIVE_PATH}" FILES "${SOURCE_FILE}")
endforeach()

# Define our godot-cpp library
add_library(${GODOT_CPP_PROJECT_NAME} SHARED)

target_sources(${GODOT_CPP_PROJECT_NAME}
	PRIVATE
		${PRIVATE_SOURCE_FILES}
)

set(GODOT_CPP_PROJECT_OUTPUT_FILE_NAME "${GODOT_CPP_PROJECT_NAME}.$<LOWER_CASE:${CMAKE_SYSTEM_NAME}_$<CONFIG>.x86_64>")

set_target_properties(${GODOT_CPP_PROJECT_NAME}
	PROPERTIES
		LINKER_LANGUAGE					CXX
		ARCHIVE_OUTPUT_DIRECTORY		"${PROJECT_BINARY_DIR}/lib/"
		LIBRARY_OUTPUT_DIRECTORY		"${PROJECT_BINARY_DIR}/lib/"
		RUNTIME_OUTPUT_DIRECTORY		"${PROJECT_BINARY_DIR}/bin/"
		OUTPUT_NAME 					"${GODOT_CPP_PROJECT_OUTPUT_FILE_NAME}"
		POSITION_INDEPENDENT_CODE		ON
		#INTERPROCEDURAL_OPTIMIZATION	ON
		USE_FOLDERS						ON
		VS_DEBUGGER_COMMAND				"${GODOT_EXECUTABLE_PATH}"
		VS_DEBUGGER_COMMAND_ARGUMENTS	"--path ${GODOT_PROJECT_FOLDER_PATH}"
)

target_link_libraries(${GODOT_CPP_PROJECT_NAME}
	PUBLIC
		godot::cpp
)

copy_godot_extension_to_godot_project_folder(
	${GODOT_PROJECT_FOLDER_PATH}
	${GODOT_CPP_PROJECT_NAME}
	"library_init"
	${GODOT_MAJOR_MINOR_VERSION}
)

add_custom_command(TARGET ${GODOT_CPP_PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
		"$<TARGET_FILE:${GODOT_CPP_PROJECT_NAME}>"
		"${GODOT_PROJECT_FOLDER_PATH}/bin/"
)

# Set the default startup project for Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${GODOT_CPP_PROJECT_NAME})
