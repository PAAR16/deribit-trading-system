cmake_minimum_required(VERSION 3.10)
project(DeribitTradingSystem)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED system thread)

add_executable(deribit
    src/main.cpp
    api/DeribitWsClient.cpp
    engine/TradingEngine.cpp
    cli/TradingCLI.cpp
)

target_include_directories(deribit PRIVATE .)
target_link_libraries(deribit OpenSSL::SSL Boost::system Boost::thread)
