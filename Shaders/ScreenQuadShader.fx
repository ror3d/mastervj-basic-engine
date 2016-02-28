Texture2D DiffuseTexture : register( t0 );
SamplerState LinearSampler : register( s0 );

#ifdef HAS_LIGHTMAP
Texture2D LightmapTexture : register( t1 );
SamplerState LinearLightmapSampler : register( s1 );
#endif

struct VS_SCREEN_INPUT
{
	float3 Pos		: POSITION;
	float2 UV : TEXCOORD0;
};

struct PS_SCREEN_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD0;
};

PS_SCREEN_INPUT VS(VS_SCREEN_INPUT IN)
{
	PS_SCREEN_INPUT l_Output = (PS_SCREEN_INPUT)0;
	l_Output.Pos=float4(IN.Pos.xyz, 1.0);
	l_Output.UV=IN.UV;
	return l_Output;
}

float4 PS( PS_SCREEN_INPUT IN) : SV_Target
{
#ifdef HAS_LIGHTMAP
	return float4(DiffuseTexture.Sample(LinearSampler, IN.UV).xyz, 1.0f)
			* float4(LightmapTexture.Sample(LinearLightmapSampler, IN.UV).xyz, 1.0f);
#else
	return DiffuseTexture.Sample(LinearSampler, IN.UV);
#endif
}
