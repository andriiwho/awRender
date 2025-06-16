float4 vs_main() : SV_Position
{
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

float4 fs_main() : SV_Target
{
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
}