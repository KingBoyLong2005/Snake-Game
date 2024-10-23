cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

project(SdlCreateWindow VERSION 1.0.0)

if(WIN32)
    set(SDL2_LIB_DIR C:/dev/SDL/lib)
    set(SDL2_LIBRARY C:/dev/SDL/bin)
    set(SDL2_INCLUDE_DIR C:/dev/SDL/include/SDL2)
    link_directories(${SDL2_LIB_DIR})
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} src/main.cpp src/Game.cpp src/Snake.cpp src/Apple.cpp src/Point.cpp )


if(WIN32)
  message("Win32 App")
  target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIR})    
    target_link_libraries(
        ${PROJECT_NAME}
        mingw32
        SDL2main    
        SDL2
        SDL2_image
        SDL2_ttf
    )
    file(COPY ${SDL2_LIBRARY}/SDL2.dll DESTINATION ./)
    file(COPY ${SDL2_LIBRARY}/SDL2_image.dll DESTINATION ./)
    file(COPY ${SDL2_LIBRARY}/SDL2_ttf.dll DESTINATION ./)

    # Sao chép thư mục assets (bao gồm fonts) vào thư mục thực thi
  file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
  
elseif(APPLE)
  message("macOS App")
  find_package(SDL2 REQUIRED COMPONENTS SDL2)
  target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
  find_package(SDL2_image REQUIRED COMPONENTS SDL2_image)
  target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image::SDL2_image)
  find_package(SDL2_ttf REQUIRED COMPONENTS SDL2_ttf)
  target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf::SDL2_ttf)
elseif(UNIX)

endif()

# Copy file
file(COPY assets DESTINATION ./)