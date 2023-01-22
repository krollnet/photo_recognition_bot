
cmake_minimum_required(VERSION 3.13)

project ("photo_recognition_bot")


add_executable (photo_recognition_bot "cursovaya.cpp" "cursovaya.h"  "dialogs.h" "localStorage.h" "storagerecord.h" "storageuser.h")



set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")


set(Boost_USE_MULTITHREADED ON)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS REQUIRED)
find_package(CURL)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(date CONFIG REQUIRED)
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()
set(TG_BOT ${Boost_LIBRARY_DIRS}/TgBot.lib)

find_package( Tesseract 5.2.0 REQUIRED )
include_directories(${Tesseract_INCLUDE_DIRS})

if (DEFINED OUTPUT_DIR)
add_custom_command(TARGET photo_recognition_bot  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/config
                ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_DIR}/config)

add_custom_command(TARGET photo_recognition_bot  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/tessdata
                ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_DIR}/tessdata)
else()
add_custom_command(TARGET photo_recognition_bot  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/config
                ${CMAKE_CURRENT_BINARY_DIR}/config)

add_custom_command(TARGET photo_recognition_bot  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/tessdata
                ${CMAKE_CURRENT_BINARY_DIR}/tessdata)
endif()

target_link_libraries(photo_recognition_bot ${TG_BOT} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES} Tesseract::libtesseract nlohmann_json::nlohmann_json date::date date::date-tz)
