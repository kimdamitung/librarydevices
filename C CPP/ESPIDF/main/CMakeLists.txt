idf_component_register(
    SRCS 
                    "main.c"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../src/i2c.c"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../core/src/mpu6050.cpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../core/src/pcf8575.cpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../core/src/st7735.cpp"
    INCLUDE_DIRS    
                    "."
                    "${CMAKE_CURRENT_SOURCE_DIR}/../include/"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../core/include/"
)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)