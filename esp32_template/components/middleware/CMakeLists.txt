idf_component_register(
    SRCS "ring_buffer.c" 
         "debug_print.c"
         "protocol_frame.c"
         "ring_buffer.c"
    INCLUDE_DIRS "."
    REQUIRES driver config
)