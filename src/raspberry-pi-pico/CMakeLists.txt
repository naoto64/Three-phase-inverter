add_executable(inverter
        inverter.c
        )

# pull in common dependencies
target_link_libraries(inverter pico_stdlib hardware_adc)

# create map/bin/hex file etc.
pico_add_extra_outputs(inverter)

# add url via pico_set_program_url
example_auto_set_url(inverter)
