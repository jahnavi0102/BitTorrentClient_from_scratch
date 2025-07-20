cmake_minimum_required(VERSION 3.10)
project(BitTorrentClient)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake where OpenSSL is
set(OPENSSL_ROOT_DIR "C:/Users/jjain/vcpkg/packages/openssl_x64-windows")
set(OPENSSL_INCLUDE_DIR "C:/Users/jjain/vcpkg/packages/openssl_x64-windows/include")
find_package(OpenSSL REQUIRED)

set(CURL_INCLUDE_DIR "C:/Users/jjain/vcpkg/packages/curl_x64-windows/include")
find_package(CURL REQUIRED)

add_executable(BitTorrentClient main.cpp)

# if (WIN32)
#     target_link_libraries(BitTorrentClient ws2_32)
# endif()

target_include_directories(BitTorrentClient PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link both OpenSSL and Curl
target_link_libraries(BitTorrentClient PRIVATE
    OpenSSL::SSL
    CURL::libcurl
    ws2_32 # This is the target name usually exported by find_package(CURL)
)