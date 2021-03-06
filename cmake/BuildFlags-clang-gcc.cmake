# Copyright 2016 Joachim Pileborg
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

check_cxx_compiler_flag(-D_DEBUG HAVE_FLAG_DDEBUG)
if(HAVE_FLAG_DDEBUG)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
endif()
check_cxx_compiler_flag(-O0 HAVE_FLAG_O0)
if(HAVE_FLAG_O0)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
endif()

# Check for supported warning flags
check_cxx_compiler_flag(-Wall HAVE_FLAG_WALL)
if(HAVE_FLAG_WALL)
    list(APPEND COMPILER_OPTIONS -Wall)
endif()
check_cxx_compiler_flag(-Wextra HAVE_FLAG_WEXTRA)
if(HAVE_FLAG_WEXTRA)
    list(APPEND COMPILER_OPTIONS -Wextra)
endif()
check_cxx_compiler_flag(-Weffc++ HAVE_FLAG_WEFFCXX)
if(HAVE_FLAG_WEFFCXX)
    list(APPEND COMPILER_OPTIONS -Weffc++)
endif()
check_cxx_compiler_flag(-Wpedantic HAVE_FLAG_WPEDANTIC)
if(HAVE_FLAG_WPEDANTIC)
    list(APPEND COMPILER_OPTIONS -Wpedantic)
else()
    check_cxx_compiler_flag(-pedantic HAVE_FLAG_PEDANTIC)
    if(HAVE_FLAG_PEDANTIC)
        list(APPEND COMPILER_OPTIONS -pedantic)
    endif()
endif()
check_cxx_compiler_flag(-Wimplicit-fallthrough HAVE_FLAG_WIMPLICIT_FALLTHROUGH)
if(HAVE_FLAG_WIMPLICIT_FALLTHROUGH)
    list(APPEND COMPILER_OPTIONS -Wimplicit-fallthrough)
endif()

# Check for standard to use
check_cxx_compiler_flag(-std=c++17 HAVE_FLAG_STD_CXX17)
if(HAVE_FLAG_STD_CXX17)
    list(APPEND COMPILER_OPTIONS -std=c++17)
else()
    check_cxx_compiler_flag(-std=c++1z HAVE_FLAG_STD_CXX1Z)
    if(HAVE_FLAG_STD_CXX1Z)
        list(APPEND COMPILER_OPTIONS -std=c++1z)
    else()
        check_cxx_compiler_flag(-std=c++14 HAVE_FLAG_STD_CXX14)
        if(HAVE_FLAG_STD_CXX14)
            list(APPEND COMPILER_OPTIONS -std=c++14)
        else()
            check_cxx_compiler_flag(-std=c++1y HAVE_FLAG_STD_CXX1Y)
            if(HAVE_FLAG_STD_CXX1Y)
                list(APPEND COMPILER_OPTIONS -std=c++1y)
            else()
                check_cxx_compiler_flag(-std=c++11 HAVE_FLAG_STD_CXX11)
                if(HAVE_FLAG_STD_CXX11)
                    list(APPEND COMPILER_OPTIONS -std=c++11)
                else()
                    message(FATAL_ERROR "Need a compiler capable of at least C++11")
                endif()
            endif()
        endif()
    endif()
endif()

check_cxx_compiler_flag(-stdlib=libc++ HAVE_FLAG_STDLIB_LIBCXX)

if(HAVE_FLAG_STDLIB_LIBCXX)
    message(STATUS "Looking for libc++")
    find_library(HAVE_LIBCXX c++)
    if(NOT HAVE_LIBCXX)
        message(FATAL_ERROR "Need libc++")
    endif()
    message(STATUS "Looking for libc++ - found")

    # Check for the ABI library, and if we can build with `-stdlib=libc++` without linking with `-lc++abi`
    # TODO: Need better check for this, it seems I still need to build with -lc++abi even though this test says it's not needed
    # TODO: Need to check the run-time output of the program
    message(STATUS "Looking for libc++abi")
    find_library(HAVE_LIBCXXABI c++abi)
    if(NOT HAVE_LIBCXXABI)
        message(STATUS "Looking for libc++abi - not found")
    else()
        message(STATUS "Looking for libc++abi - found")
        set(OLD_CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS})
        set(CMAKE_REQUIRED_FLAGS "-stdlib=libc++")
        CHECK_CXX_SOURCE_COMPILES(
                "#include <iostream>
            int main()
            {
                std::cout << 'x';
            }"
                STDLIBCXX_WITHOUT_LIBCXXABI
                FAIL_REGEX "libc\\\\+\\\\+abi\\\\.so")
        set(CMAKE_REQUIRED_FLAGS ${OLD_CMAKE_REQUIRED_FLAGS})
    endif()
endif()

if(HAVE_FLAG_STDLIB_LIBCXX)
    list(APPEND COMPILER_OPTIONS -stdlib=libc++)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
endif()

# Check for other miscelaneous flags
check_cxx_compiler_flag(-pipe HAVE_FLAG_PIPE)
if(HAVE_FLAG_PIPE)
    list(APPEND COMPILER_OPTIONS -pipe)
endif()

if(Threads_FOUND)
    check_cxx_compiler_flag(-pthread HAVE_FLAG_PTHREAD)
    if(HAVE_FLAG_PTHREAD)
        list(APPEND COMPILER_OPTIONS -pthread)
    endif()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
    # TODO: Add the pthread linker library?
    # TODO: Check if it is needed first
endif()
