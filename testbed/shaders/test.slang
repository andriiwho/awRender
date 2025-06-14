[shader("vertex")]
float4 vs_main() : SV_Position
{
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

[shader("pixel")]
float4 ps_main() : SV_Target
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

[shader("compute")]
[numthreads(1, 1, 1)]
void cs_main()
{
    
}