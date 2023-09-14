cmake_minimum_required(VERSION 3.22)
project(GROUP-B)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(USE_UART OFF)

set(SERVER_DIR server/desktop)
set(CLIENT_DIR client/desktop)

set(SERVER_LIBRARIES Qt6::Core Qt6::Widgets)
set(CLIENT_LIBRARIES Qt6::Core Qt6::Widgets Qt6::Multimedia)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia SerialPort)

# Conditional check for USE_UART for the server
if(${USE_UART} MATCHES ON)
    add_compile_definitions(USE_UART)
    set(SERVER_COM_SERVICE_INCLUDE ${SERVER_DIR}/include/uartservice.h)
    set(SERVER_COM_SERVICE_SOURCES ${SERVER_DIR}/src/uartservice.cpp)
    set(SERVER_LIBRARIES ${SERVER_LIBRARIES} Qt6::SerialPort)
else()
    set(SERVER_COM_SERVICE_INCLUDE ${SERVER_DIR}/include/tcpservice.h)
    set(SERVER_COM_SERVICE_SOURCES ${SERVER_DIR}/src/tcpservice.cpp)
endif()

# Conditional check for USE_UART for the client
if(USE_UART)
    set(CLIENT_COM_SERVICE_INCLUDE ${CLIENT_DIR}/include/uartservice.h)
    set(CLIENT_COM_SERVICE_SOURCES ${CLIENT_DIR}/src/uartservice.cpp)
    set(CLIENT_LIBRARIES ${CLIENT_LIBRARIES} Qt6::SerialPort)
else()
    set(CLIENT_COM_SERVICE_INCLUDE ${CLIENT_DIR}/include/tcpservice.h)
    set(CLIENT_COM_SERVICE_SOURCES ${CLIENT_DIR}/src/tcpservice.cpp)
endif()

set(SERVER_HEADERS shared/setting.h ${SERVER_DIR}/include/window.h ${SERVER_DIR}/include/comservice.h ${SERVER_COM_SERVICE_INCLUDE})
set(SERVER_SOURCES ${SERVER_DIR}/main.cpp ${SERVER_DIR}/src/window.cpp ${SERVER_DIR}/src/comservice.cpp ${SERVER_COM_SERVICE_SOURCES})

set(CLIENT_HEADERS shared/setting.h ${CLIENT_DIR}/include/window.h ${CLIENT_DIR}/include/canvas.h ${CLIENT_DIR}/include/comservice.h ${CLIENT_COM_SERVICE_INCLUDE})
set(CLIENT_SOURCES ${CLIENT_DIR}/main.cpp ${CLIENT_DIR}/src/window.cpp ${CLIENT_DIR}/src/canvas.cpp ${CLIENT_DIR}/src/comservice.cpp ${CLIENT_COM_SERVICE_SOURCES})

add_executable(server ${SERVER_HEADERS} ${SERVER_SOURCES})
target_link_libraries(server PUBLIC ${SERVER_LIBRARIES})
target_include_directories(server PUBLIC shared ${SERVER_DIR}/include)

add_executable(client ${CLIENT_HEADERS} ${CLIENT_SOURCES})
target_link_libraries(client PUBLIC ${CLIENT_LIBRARIES})
target_include_directories(client PUBLIC shared ${CLIENT_DIR}/include)


add_custom_target(upload_client
        COMMAND platformio run --target upload
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/client/esp32
        COMMENT "Building and uploading the project to the ESP32 for client")


#The custom target depends on the exceuatbles being built
add_dependencies(upload_client client)

add_custom_target( upload_server
        COMMAND platformio run --target upload
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/server/esp32
        COMMENT "Building and uploading the project to the ESP32 for server")


#The custom target depends on the exceuatbles being built
add_dependencies(upload_server server)

