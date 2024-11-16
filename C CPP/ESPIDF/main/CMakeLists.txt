idf_component_register(
    SRCS 
                    "main.cpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../core/src/RGB.cpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../core/src/BUZZER.cpp"
                    "${CMAKE_CURRENT_SOURCE_DIR}/../core/src/ESPNOW.cpp"
    INCLUDE_DIRS    
                    "."
                    "${CMAKE_CURRENT_SOURCE_DIR}/../core/include/"
)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)