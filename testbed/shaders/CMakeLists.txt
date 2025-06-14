add_custom_target(awTestbedShaders ALL)

macro(testbed_spv name output entry)
    aw_compile_shader(awTestbedShaders ${name}
            SHADER_TARGET "spirv"
            OUTPUT ${output}
            PROFILE "spirv_1_3"
            ENTRY ${entry})
endmacro()

macro(testbed_dxil name output entry)
    aw_compile_shader(awTestbedShaders ${name}
            SHADER_TARGET "dxil"
            OUTPUT ${output}
            PROFILE "sm_6_2"
            ENTRY ${entry})
endmacro()

macro(testbed_shader name output entry)
    testbed_spv(${name} ${output}.spv ${entry})
    testbed_dxil(${name} ${output}.dxil ${entry})
endmacro()

execute_process(WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory bin)

testbed_shader(test.slang "bin/test.vert" vs_main)
testbed_shader(test.slang "bin/test.frag" ps_main)
testbed_shader(test.slang "bin/test.comp" cs_main)