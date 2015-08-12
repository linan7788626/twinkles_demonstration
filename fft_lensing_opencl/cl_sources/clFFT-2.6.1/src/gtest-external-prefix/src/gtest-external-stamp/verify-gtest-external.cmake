set(file "/Users/uranus/GitHub/twinkles_demonstration/fft_lensing_opencl/cl_sources/clFFT-2.6.1/src/gtest-external-prefix/src/gtest-1.7.0.zip")
message(STATUS "verifying file...
     file='${file}'")
set(expect_value "2d6ec8ccdf5c46b05ba54a9fd1d130d7")
set(attempt 0)
set(succeeded 0)
while(${attempt} LESS 3 OR ${attempt} EQUAL 3 AND NOT ${succeeded})
  file(MD5 "${file}" actual_value)
  if("${actual_value}" STREQUAL "${expect_value}")
    set(succeeded 1)
  elseif(${attempt} LESS 3)
    message(STATUS "MD5 hash of ${file}
does not match expected value
  expected: ${expect_value}
    actual: ${actual_value}
Retrying download.
")
    file(REMOVE "${file}")
    execute_process(COMMAND ${CMAKE_COMMAND} -P "/Users/uranus/GitHub/twinkles_demonstration/fft_lensing_opencl/cl_sources/clFFT-2.6.1/src/gtest-external-prefix/src/gtest-external-stamp/download-gtest-external.cmake")
  endif()
  math(EXPR attempt "${attempt} + 1")
endwhile()

if(${succeeded})
  message(STATUS "verifying file... done")
else()
  message(FATAL_ERROR "error: MD5 hash of
  ${file}
does not match expected value
  expected: ${expect_value}
    actual: ${actual_value}
")
endif()
