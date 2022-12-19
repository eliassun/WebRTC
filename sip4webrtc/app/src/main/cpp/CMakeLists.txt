# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("sip4webrtc")

set(src_root wrapper/)
set(media_src wrapper/med/)
set(call_src wrapper/call/)
set(inc_src wrapper/include/)
include_directories(
        ${src_root}
        ${media_src}
        ${call_src}
)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             pjsip4webrtc-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             native-lib.cpp
        ${src_root}logger.cpp
        )

target_include_directories(pjsip4webrtc-lib PUBLIC
        ${CMAKE_SOURCE_DIR}/pjsip/${ANDROID_ABI}/include
        )


add_library( libssl STATIC IMPORTED )
set_target_properties(libssl
        PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/openssl/${ANDROID_ABI}/lib/libssl.a )

add_library( libcrypto STATIC IMPORTED )
set_target_properties(libcrypto
        PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/openssl/${ANDROID_ABI}/lib/libcrypto.a )

add_library( libpj STATIC IMPORTED )
set_target_properties(libpj
        PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/pjsip/${ANDROID_ABI}/lib/libpj.a )

add_library( libpjlib-util STATIC IMPORTED )
set_target_properties(libpjlib-util
        PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/pjsip/${ANDROID_ABI}/lib/libpjlib-util.a )

add_library(libpjnath STATIC IMPORTED )
set_target_properties(libpjnath
        PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/pjsip/${ANDROID_ABI}/lib/libpjnath.a)

add_library(libpjsip STATIC IMPORTED )
set_target_properties(libpjsip
        PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/pjsip/${ANDROID_ABI}/lib/libpjsip.a)

add_library(libpjsip-ua STATIC IMPORTED )
set_target_properties(libpjsip-ua
        PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/pjsip/${ANDROID_ABI}/lib/libpjsip-ua.a)

add_library(libpjsua STATIC IMPORTED )
set_target_properties(libpjsua
        PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/pjsip/${ANDROID_ABI}/lib/libpjsua.a)

add_library(libpjsua2 STATIC IMPORTED )
set_target_properties(libpjsua2
        PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/pjsip/${ANDROID_ABI}/lib/libpjsua2.a)

add_library(libpjmedia STATIC IMPORTED )
set_target_properties(libpjmedia
        PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/pjsip/${ANDROID_ABI}/lib/libpjmedia.a)


add_library(libpjsip-simple STATIC IMPORTED )
set_target_properties(libpjsip-simple
        PROPERTIES
        IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/pjsip/${ANDROID_ABI}/lib/libpjsip-simple.a)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib
        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        pjsip4webrtc-lib
        libpjmedia
        libpjsip
        libpjnath
        libpjlib-util
        libpj
        libpjsip-simple
        libpjsip-ua
        libssl
        libcrypto
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib} )
