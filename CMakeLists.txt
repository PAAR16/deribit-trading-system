cmake_minimum_required(VERSION 3.10)
project(DeribitTradingSystem)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED system thread)

# Find websocketpp (assuming installed)
find_path(WEBSOCKETPP_INCLUDE_DIR websocketpp/client.hpp)
if(NOT WEBSOCKETPP_INCLUDE_DIR)
    message(FATAL_ERROR "websocketpp not found")
endif()

# Find nlohmann/json (assuming installed)
find_package(nlohmann_json REQUIRED)

add_executable(deribit
    src/main.cpp
    api/DeribitWsClient.cpp
    engine/TradingEngine.cpp
    cli/TradingCLI.cpp
)

target_include_directories(deribit PRIVATE . ${WEBSOCKETPP_INCLUDE_DIR})
target_link_libraries(deribit OpenSSL::SSL Boost::system Boost::thread nlohmann_json::nlohmann_json)
