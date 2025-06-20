module main;

#include "aw_helpers.slang"

struct Vertex
{
    float3 position;
    float3 color;
};

struct BindlessParameters
{
    StructuredBuffer<Vertex>.Handle vertex_buffer;
};

ParameterBlock<BindlessParameters> u_BindlessParameters;

struct VSOut
{
    float4 position : SV_Position;
};

[shader("vertex")]
VSOut vertex_shader(in uint vertex_id : AW_VertexID)
{
    VSOut out;
    out.position = float4(u_BindlessParameters.vertex_buffer[vertex_id].position, 1.0f);
    return out;
}

[shader("fragment")]
float4 fragment_shader(in VSOut vs_out) : SV_Target0
{
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
}