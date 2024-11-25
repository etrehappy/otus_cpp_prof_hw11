cmake_minimum_required(VERSION 3.26)

project(join_server_prj)

set(SERVER_FILES
    main.cpp main.h    
    settings.cpp settings.h
    os_interface.h
)

set(DATABASE_FILES
    database.cpp    database.h
)

set(NETWORK_FILES
    network_handler.cpp network_handler.h
    session.cpp     session.h
    parser.cpp      parser.h
)

############################################# BOOST #############################################

if(${CMAKE_VERSION} VERSION_LESS "3.30")
    message(STATUS "CMake version is ${CMAKE_VERSION}. Used CMP0167 OLD")    
else()
    message(STATUS "CMake version >= 3.30. Used CMP0167 NEW. Need to set BOOST_ROOT and Boost_DIR")
    cmake_policy(SET CMP0167 NEW)
    cmake_policy(SET CMP0144 NEW)
    set (BOOST_ROOT "C:/Program Files/boost/boost_1_85_0")
    set (Boost_DIR "${BOOST_ROOT}/bin/x64/lib/cmake/Boost-1.85.0")
endif()

set(Boost_DEBUG OFF)

find_package (Boost 1.75.0 COMPONENTS program_options system REQUIRED) 

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found!")
endif()

include_directories (SYSTEM ${Boost_INCLUDE_DIR})


####################################################################################################

add_executable(join_server
    ${SERVER_FILES}
    ${DATABASE_FILES}
    ${NETWORK_FILES}
)

target_link_libraries(join_server PRIVATE
    ${Boost_LIBRARIES}
    Boost::program_options
)

if(MSVC)
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT join_server)	
    source_group("Server" FILES ${SERVER_FILES})
    source_group("Database" FILES ${DATABASE_FILES})
    source_group("Network" FILES ${NETWORK_FILES})
endif ()

target_compile_options(join_server PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)
set_target_properties(join_server PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)    