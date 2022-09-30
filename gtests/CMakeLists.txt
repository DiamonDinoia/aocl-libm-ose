# USING CPM TO HANDLE DEPENDENCIES
set(CPM_DOWNLOAD_VERSION 0.35.6)

if (CPM_SOURCE_CACHE)
    set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif (DEFINED ENV{CPM_SOURCE_CACHE})
    set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else ()
    set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif ()

if (NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif ()

include(${CPM_DOWNLOAD_LOCATION})

CPMAddPackage(
        NAME googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
        GIT_SHALLOW YES
        VERSION 1.12.1
        OPTIONS "INSTALL_GTEST OFF" "gtest_force_shared_crt"

)

add_definitions(-DLIBM_PROTOTYPE=PROTOTYPE_GLIBC)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/almtest
        ${CMAKE_CURRENT_SOURCE_DIR}/dynamic/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/gapi/gtest
        ${CMAKE_CURRENT_SOURCE_DIR}/gapi/gbench
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/mparith/mparith
        ${CMAKE_CURRENT_SOURCE_DIR}/atan
        ${CMAKE_CURRENT_SOURCE_DIR}/cos
        ${CMAKE_CURRENT_SOURCE_DIR}/cosh
        ${CMAKE_CURRENT_SOURCE_DIR}/exp
        ${CMAKE_CURRENT_SOURCE_DIR}/exp2
        ${CMAKE_CURRENT_SOURCE_DIR}/expm1
        ${CMAKE_CURRENT_SOURCE_DIR}/fabs
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/log
        ${CMAKE_CURRENT_SOURCE_DIR}/pow
        ${CMAKE_CURRENT_SOURCE_DIR}/sin
        ${CMAKE_CURRENT_SOURCE_DIR}/sinh
        ${CMAKE_CURRENT_SOURCE_DIR}/tan
        ${CMAKE_CURRENT_SOURCE_DIR}/tanh
)

list(APPEND FLAGS -Wno-missing-field-initializers
        -Wno-unused-parameter
        -Wno-multichar
        -falign-functions=32
        -Wno-maybe-uninitialized
        -Wno-unused-variable
        )

add_compilation_flags(FLAGS)

file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c*")

add_definitions(-DDOUBLE)

#find_package(MPFR REQUIRED)


add_executable(hello_test ${TEST_SOURCES})
target_link_libraries(hello_test amdlibm gtest_main gtest gmock)
target_compile_features(hello_test PRIVATE cxx_std_17)

enable_testing()
add_test(hello_test hello_test)