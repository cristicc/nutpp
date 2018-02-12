#
# Nutpp - CMake build configuration.
#

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(nutpp VERSION 1.0.0 LANGUAGES C CXX)
set(PROJECT_DESCRIPTION "Utility to manage nutrition plans.")
set(PROJECT_LOGO "${CMAKE_CURRENT_SOURCE_DIR}/web/favicon.ico")

add_definitions(-DAPP_NAME="${PROJECT_NAME}")
add_definitions(-DAPP_VERSION="${PROJECT_VERSION}")

# Main components.
set(WEBSERVER_COMPONENT "webserver")
set(STORAGE_COMPONENT "storage")
set(RUNTIME_COMPONENT "runtime")

#
# Set global build flags.
#

# Enable C++14 features.
set(CMAKE_CXX_STANDARD 14)

# Configure supported compilers.
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # Visual Studio C++: do not generate manifests for the executables.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # GCC: enable common compiler warnings.
    add_compile_options(-Wall -Wextra -Wcast-align -Wpointer-arith
        -Wformat-security -Wmissing-format-attribute)
    # Make linker report any unresolved symbols from object files.
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # Using Clang.
    message(WARNING "Clang compiler is not officially supported.")

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    # Using Intel C++.
    message(WARNING "Intel C++ compiler is not officially supported.")
endif()

#
# Debugging options.
#
set(CMAKE_VERBOSE_MAKEFILE 0)

#
# Locate project prerequisites.
#
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Log4cplus.
find_package(Log4cplus 2.0 REQUIRED)
include_directories(${LOG4CPLUS_INCLUDE_DIRS})

# Wt.
find_package(Wt REQUIRED)
include_directories(${WT_INCLUDE_DIRS})

#
# Project search paths.
#
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")
add_subdirectory(src)

#
# Add build targets.
#
include(DoxygenTargets)
#add_custom_target(check COMMENT "Checking sources..")
#add_dependencies(check
#    all-libs-check
#)

#
# Add packaging support.
#
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR "Cristian Ciocaltea")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README.adoc)
set(CPACK_PACKAGE_ICON ${PROJECT_LOGO})
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${PROJECT_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})

# Select components to be packaged by default.
set(CPACK_COMPONENTS_ALL ${CORE_COMPONENT} ${RUNTIME_COMPONENT})

# Select one or more generators (e.g.: ZIP;TBZ2;TXZ;7Z).
if(UNIX)
    set(CPACK_GENERATOR "TXZ")
    set(CPACK_SOURCE_GENERATOR "TXZ")
else()
    set(CPACK_GENERATOR "ZIP")
    set(CPACK_SOURCE_GENERATOR "7Z")
endif()

# Include/exclude the top level directory in/from the package.
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 1)
set(CPACK_COMPONENT_INCLUDE_TOPLEVEL_DIRECTORY 1)

# Enable/disable file stripping.
if(UNIX)
    set(CPACK_STRIP_FILES 1)
    set(CPACK_SOURCE_STRIP_FILES 0)
endif()

# Enable/disable component install for ARCHIVE generators.
set(CPACK_ARCHIVE_COMPONENT_INSTALL 1)

# Specify components grouping for multi-package component-aware generators.
# - ONE_PER_GROUP: creates one package file per component group (default)
# - ALL_COMPONENTS_IN_ONE: creates a single package with all components
# - IGNORE: creates one package per component
set(CPACK_COMPONENTS_GROUPING ALL_COMPONENTS_IN_ONE)

# Enable/disable component aware generators to ignore component if one set.
set(CPACK_MONOLITHIC_INSTALL 0)

# Add default packaging targets: package, package_source.
include(CPack)
