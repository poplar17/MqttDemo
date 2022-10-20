cmake_minimum_required(VERSION 3.22)
project(MqttConn)

set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

include_directories(include)
link_directories(lib)
add_executable(${PROJECT_NAME} user_callback.cpp user_callback.h MQTT_CONN.h MQTT_CONN.cpp LOG_SYS.h LOG_SYS.cpp Test_Mqtt_conn.cpp)
target_link_libraries(${PROJECT_NAME} libpaho-mqttpp3-static.a libpaho-mqtt3cs.dll.a libpaho-mqtt3as.dll.a)