module main;

import other;

[shader("vertex")]
float4 vertex_shader() : SV_Position
{
    return calculate_out_pos();
}
