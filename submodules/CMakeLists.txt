
# Submodules

cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED GIT_UPDATE_SUBMODULES)
    set(GIT_UPDATE_SUBMODULES ON CACHE BOOL "Check submodules during build")
endif()

set(GIT_SUBMODULES_UPDATED OFF)

# Initialize and update the godot-cpp submodule
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
	# Update submodules as needed
	if(GIT_UPDATE_SUBMODULES)
		message(STATUS "Submodule update")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			RESULT_VARIABLE GIT_UPDATE_SUBMODULES_RESULT
		)
		if(NOT GIT_UPDATE_SUBMODULES_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init failed with ${GIT_UPDATE_SUBMODULES_RESULT}, please checkout submodules")
		endif()
		set(GIT_UPDATE_SUBMODULES OFF CACHE BOOL "Check submodules during build" FORCE)
		set(GIT_SUBMODULES_UPDATED ON)
	endif()
endif()

# Assuming godot-cpp submodule is in 'extern/godot-cpp'
set(GODOT_CPP_SUBMODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/godot-cpp")

# Determine the branch name based on Godot version, by default we'll treat the godot version major and minor as the branch.
set(GODOT_CPP_BRANCH ${GODOT_MAJOR_MINOR_VERSION})
set(GODOT_CPP_CURRENT_BRANCH "" CACHE STRING "Current branch of godot-cpp submodule")

# Checkout the correct branch
if(EXISTS "${GODOT_CPP_SUBMODULE_PATH}/.git")
	if(${GIT_SUBMODULES_UPDATED} OR NOT GODOT_CPP_CURRENT_BRANCH STREQUAL GODOT_CPP_BRANCH)

		execute_process(
			COMMAND ${GIT_EXECUTABLE} checkout ${GODOT_CPP_BRANCH}
			WORKING_DIRECTORY ${GODOT_CPP_SUBMODULE_PATH}
			RESULT_VARIABLE GIT_CHECKOUT_RESULT
		)
		if(GIT_CHECKOUT_RESULT EQUAL "0")
			message(STATUS "Checked out godot-cpp branch ${GODOT_CPP_BRANCH}")
			set(GODOT_CPP_CURRENT_BRANCH ${GODOT_CPP_BRANCH} CACHE STRING "Current branch of godot-cpp submodule" FORCE)
		else()
			message(FATAL_ERROR "git checkout failed with ${GIT_CHECKOUT_RESULT}, please checkout the branch manually")
		endif()

	endif()
endif()



add_subdirectory(godot-cpp)
