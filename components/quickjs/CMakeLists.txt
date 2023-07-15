idf_component_register(
    SRC_DIRS .
    INCLUDE_DIRS .)

component_compile_options(-Wno-error=format=)
component_compile_options(-Wno-format)
component_compile_options(-Wno-incompatible-pointer-types)
component_compile_options(-Wno-unused-function)
component_compile_options(-Wno-cast-function-type)
component_compile_options(-Wno-implicit-fallthrough)
