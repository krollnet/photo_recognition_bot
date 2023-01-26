
cmake_minimum_required(VERSION 3.13)

project ("photo_recognition_bot" VERSION 1.0 LANGUAGES CXX)

add_executable (
    photo_recognition_bot 
    "cursovaya.cpp" "cursovaya.h" "dialogs.h" "localStorage.h" "storagerecord.h" "storageuser.h"
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()
add_compile_options(
    -Werror

    -Wall
    -Wextra
    -Wpedantic

    -Wcast-align
    -Wcast-qual
    -Wconversion
    -Wctor-dtor-privacy
    -Wenum-compare
    -Wfloat-equal
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    -Wsign-conversion
    -Wsign-promo
)

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

target_link_libraries(photo_recognition_bot ${TG_BOT} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES} Tesseract::libtesseract nlohmann_json::nlohmann_json date::date date::date-tz)

if (DEFINED OUTPUT_DIR)
    add_custom_command(TARGET photo_recognition_bot  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/config
                ${CMAKE_CURRENT_BINARY_DIR}/bin/${OUTPUT_DIR}/config)

    add_custom_command(TARGET photo_recognition_bot  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/tessdata
                ${CMAKE_CURRENT_BINARY_DIR}/bin/${OUTPUT_DIR}/tessdata)
else()
    add_custom_command(TARGET photo_recognition_bot  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/config
                ${CMAKE_CURRENT_BINARY_DIR}/bin/config)

    add_custom_command(TARGET photo_recognition_bot  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/tessdata
                ${CMAKE_CURRENT_BINARY_DIR}/bin/tessdata)
endif()

find_package(Doxygen)
if (DOXYGEN_FOUND)
    add_custom_target(docs ${DOXYGEN_EXECUTABLE} Doxyfile
    COMMENT "Generating API documentation with Doxygen" VERBATIM SOURCES ${PRJ_DOXY}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

set_target_properties( photo_recognition_bot
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin DESTINATION ${CMAKE_INSTALL_PREFIX} PATTERN ".dll")
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/config DESTINATION bin)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tessdata DESTINATION bin)
install(FILES $<TARGET_RUNTIME_DLLS:photo_recognition_bot> DESTINATION bin)
install(TARGETS photo_recognition_bot 
RUNTIME DESTINATION bin
ARCHIVE DESTINATION bin
)
