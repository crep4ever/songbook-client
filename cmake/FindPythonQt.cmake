# Find PythonQt
#
# Sets PYTHONQT_FOUND, PYTHONQT_INCLUDE_DIR, PYTHONQT_LIBRARY, PYTHONQT_LIBRARIES
#

include(ExternalProject)

# Download PythonQt from SVN, patch then build
ExternalProject_Add(PythonQt
PREFIX pythonqt
SVN_REPOSITORY svn://svn.code.sf.net/p/pythonqt/code/trunk
SVN_REVISION -r403
# Backup URL in case SVN messes up.
# URL "http://downloads.sourceforge.net/project/pythonqt/pythonqt/PythonQt-3.0/PythonQt3.0.zip"
PATCH_COMMAND patch -p0 --ignore-whitespace --batch --input=${CMAKE_CURRENT_SOURCE_DIR}/cmake/python3.patch
CONFIGURE_COMMAND qmake ${CMAKE_CURRENT_BINARY_DIR}/pythonqt/src/PythonQt/PythonQt.pro
BUILD_COMMAND make
BUILD_IN_SOURCE 1
INSTALL_COMMAND ""
# Wrap download, configure and build steps in a script to log output
LOG_DOWNLOAD ON
LOG_CONFIGURE ON
LOG_BUILD ON
)

set(PYTHONQT_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/pythonqt/src/PythonQt)
set(PYTHONQT_INCLUDE_DIR ${PYTHONQT_INSTALL_DIR}/src)
set(PYTHONQT_LIBRARY PythonQtLib)

# Create dummy PythonQtLib that depends on real pythonqt in order to have cmake behaving properly without having to resort
# to a superbuild mechanism.
add_library(PythonQtLib SHARED IMPORTED)
set_target_properties(PythonQtLib PROPERTIES IMPORTED_LOCATION ${PYTHONQT_INSTALL_DIR}/lib/${CMAKE_SHARED_MODULE_PREFIX}PythonQt${CMAKE_SHARED_LIBRARY_SUFFIX})
get_target_property(PYTHONQT_LOCATION PythonQtLib IMPORTED_LOCATION)
add_dependencies(PythonQtLib PythonQt)

mark_as_advanced(PYTHONQT_INSTALL_DIR)
mark_as_advanced(PYTHONQT_INCLUDE_DIR)

set(PYTHONQT_FOUND 0)
if(PYTHONQT_INCLUDE_DIR AND PYTHONQT_LIBRARY)
  # Currently CMake'ified PythonQt only supports building against a python Release build.
  # This applies independently of CTK build type (Release, Debug, ...)
  add_definitions(-DPYTHONQT_USE_RELEASE_PYTHON_FALLBACK)
  set(PYTHONQT_LIBRARIES ${PYTHONQT_LIBRARY})
  set(PYTHONQT_FOUND 1)
endif()

if(NOT ${PYTHONQT_FOUND})
  message(FATAL_ERROR "PythonQt is required to build ${PROJECT_NAME}")
else()
  message(STATUS "PythonQt found")
endif()
