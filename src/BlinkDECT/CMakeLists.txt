add_executable(BlinkDECT main.c)

target_include_directories(BlinkDECT PUBLIC ../dect-hdlc/lowLevel/linux ../dect-hdlc/hdlc ../dect-hdlc/utils)
target_link_libraries(BlinkDECT dialog-hdlc-utils dialog-hdlc-cvmdect dialog-hdlc-wsmdata dialog-hdlc-lowLevel)

