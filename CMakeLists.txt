add_library(libws2812
        src/ws2812.cpp
        )

target_include_directories(libws2812 PUBLIC inc)
target_link_libraries(libws2812 mbed-os)
