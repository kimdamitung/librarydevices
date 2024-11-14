idf_component_register(
    SRCS 
                    "main.cpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../core/src/RGB.cpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../core/src/ST7735.cpp"
    INCLUDE_DIRS    
                    "."
                    "${CMAKE_CURRENT_SOURCE_DIR}/../core/include/"
)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)