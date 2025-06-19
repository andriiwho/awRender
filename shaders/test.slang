module main;

import awrender;

struct Vertex
{
    float3 position;
    float3 color;
};

uniform DescriptorHandle<StructuredBuffer<Vertex>> u_VertexBuffer;
uniform uint u_VertexBufferId;

struct VSOut
{
    float4 position : SV_Position;
};

[shader("vertex")]
VSOut vertex_shader(in uint vertex_id : SV_VertexID)
{
    VSOut out;
    out.position = float4(u_VertexBuffer[vertex_id].position, 1.0f);
    return out;
}
