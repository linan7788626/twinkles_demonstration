if(EXISTS "/Users/uranus/GitHub/twinkles_demonstration/fft_lensing_opencl/cl_sources/clFFT-2.6.1/src/gtest-external-prefix/src/gtest-1.7.0.zip")
  file("MD5" "/Users/uranus/GitHub/twinkles_demonstration/fft_lensing_opencl/cl_sources/clFFT-2.6.1/src/gtest-external-prefix/src/gtest-1.7.0.zip" hash_value)
  if("x${hash_value}" STREQUAL "x2d6ec8ccdf5c46b05ba54a9fd1d130d7")
    return()
  endif()
endif()
message(STATUS "downloading...
     src='http://googletest.googlecode.com/files/gtest-1.7.0.zip'
     dst='/Users/uranus/GitHub/twinkles_demonstration/fft_lensing_opencl/cl_sources/clFFT-2.6.1/src/gtest-external-prefix/src/gtest-1.7.0.zip'
     timeout='none'")




file(DOWNLOAD
  "http://googletest.googlecode.com/files/gtest-1.7.0.zip"
  "/Users/uranus/GitHub/twinkles_demonstration/fft_lensing_opencl/cl_sources/clFFT-2.6.1/src/gtest-external-prefix/src/gtest-1.7.0.zip"
  SHOW_PROGRESS
  # no TIMEOUT
  STATUS status
  LOG log)

list(GET status 0 status_code)
list(GET status 1 status_string)

if(NOT status_code EQUAL 0)
  message(FATAL_ERROR "error: downloading 'http://googletest.googlecode.com/files/gtest-1.7.0.zip' failed
  status_code: ${status_code}
  status_string: ${status_string}
  log: ${log}
")
endif()

message(STATUS "downloading... done")
