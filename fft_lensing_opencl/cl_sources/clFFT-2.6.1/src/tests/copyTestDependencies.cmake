# Customized install script for fftw test program; analyzes all the shared library dependencies and installs
# the dependencies into the package
include( GetPrerequisites )

#    message( testLocation ": /Users/uranus/GitHub/twinkles_demonstration/fft_lensing_opencl/cl_sources/clFFT-2.6.1/src/staging/Test" )

# The Microsoft IDE presents a challenge because the full configuration is not known at cmake time
# This logic allows us to 'substitute' the proper confguration at install time
if( "${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "Debug" )
    string( REPLACE "\$(Configuration)" "Debug" fixedTestLocation "/Users/uranus/GitHub/twinkles_demonstration/fft_lensing_opencl/cl_sources/clFFT-2.6.1/src/staging/Test" )
elseif( "${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "Release" )
    string( REPLACE "\$(Configuration)" "Release" fixedTestLocation "/Users/uranus/GitHub/twinkles_demonstration/fft_lensing_opencl/cl_sources/clFFT-2.6.1/src/staging/Test" )
elseif( "${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "MinSizeRel" )
    string( REPLACE "\$(Configuration)" "MinSizeRel" fixedTestLocation "/Users/uranus/GitHub/twinkles_demonstration/fft_lensing_opencl/cl_sources/clFFT-2.6.1/src/staging/Test" )
elseif( "${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "RelwithDebInfo" )
    string( REPLACE "\$(Configuration)" "RelwithDebInfo" fixedTestLocation "/Users/uranus/GitHub/twinkles_demonstration/fft_lensing_opencl/cl_sources/clFFT-2.6.1/src/staging/Test" )
endif( )

#    message( fixedTestLocation ": ${fixedTestLocation}" )
# Get the directory that the test executable resides in; this helps get_prerequisites( ) find dependent libraries
get_filename_component( testDir "${fixedTestLocation}" PATH )
#    message( testDir ": ${testDir}" )

set( installPath "" )
if( WIN32 )
    set( installPath "${CMAKE_INSTALL_PREFIX}/bin" )
else( )
    set( installPath "${CMAKE_INSTALL_PREFIX}/lib" )
endif( )

# Only search for dependencies that have ROOT defined
set( depList "" )

#This logic assumes that FindFFTW.cmake has been called
get_filename_component( fftwDirSingle "/opt/local/lib/libfftw3f.dylib" PATH )
get_filename_component( fftwDirDouble "/opt/local/lib/libfftw3.dylib" PATH )

if( EXISTS "${fftwDirSingle}" )
    list( APPEND depList "${fftwDirSingle}" )
#    message( "fftwDirSingle: ${fftwDirSingle}" )
endif( )

string( COMPARE NOTEQUAL "${fftwDirSingle}" "${fftwDirDouble}" fftwDiffDirs )
if( ${fftwDiffDirs} AND EXISTS "${fftwDirDouble}" )
    list( APPEND depList "${fftwDirDouble}" )
#    message( "fftwDirDouble: ${fftwDirDouble}" )
endif( )

#This logic assumes that FindGTest.cmake has been called
get_filename_component( gtestDir "" PATH )
get_filename_component( gtestDirDebug "" PATH )

if( EXISTS "${gtestDir}" )
    list( APPEND depList "${gtestDir}" )
#    message( "gtestDir: ${gtestDir}" )
endif( )

string( COMPARE NOTEQUAL "${gtestDir}" "${gtestDirDebug}" gtestDiffDirs )
if( ${gtestDiffDirs} AND EXISTS "${gtestDirDebug}" )
    list( APPEND depList "${gtestDirDebug}" )
#    message( "gtestDirDebug: ${gtestDirDebug}" )
endif( )

#This logic assumes that FindOpenCL.cmake has been called
get_filename_component( openclDir "/System/Library/Frameworks/OpenCL.framework" PATH )

if( EXISTS "${openclDir}" )
    list( APPEND depList "${openclDir}" )
#    message( "openclDir: ${openclDir}" )
endif( )

if( EXISTS "${testDir}" )
    list( APPEND depList "${testDir}" )
endif( )

# message( "depList: ${depList}" )

# This retrieves a list of shared library dependencies from the target; they are not full path names
# Skip system dependencies and skip recursion
get_prerequisites( ${fixedTestLocation} testDependencies 1 0 "" "${depList}" )

# Loop on queried library dependencies and copy them into package
foreach( dep ${testDependencies} )
    # This converts the dependency into a full path
    gp_resolve_item( "${fixedTestLocation}" "${dep}" "" "${depList}" dep_test_path )

    # In linux, the dep_test_path may point to a symbolic link, we also need to copy real file
    get_filename_component( dep_realpath "${dep_test_path}" REALPATH )
    get_filename_component( dep_name "${dep_test_path}" NAME )
    # message( STATUS "depName: ${dep_name}" )
    # message( STATUS "depFullPath: ${dep_test_path}" )
    # message( STATUS "dep_realpath: ${dep_realpath}" )

    if( NOT EXISTS ${installPath}/${dep_name} )
        file( INSTALL ${dep_test_path} ${dep_realpath}
              USE_SOURCE_PERMISSIONS
              DESTINATION ${installPath}
            )
    endif( )
endforeach( )
