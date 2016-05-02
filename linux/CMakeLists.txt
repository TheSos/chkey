cmake_minimum_required(VERSION 2.6)

set (FILES 
	Test.cpp
)


link_libraries(X11 Xtst)

add_executable(controller Controller.cpp)
