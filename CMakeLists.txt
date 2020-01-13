cmake_minimum_required(VERSION 3.0)

project(tab2meta)

add_executable(tab2meta tab2meta.c)
target_compile_options(tab2meta PRIVATE -Wall -Wextra)

install(TARGETS tab2meta RUNTIME DESTINATION bin)
