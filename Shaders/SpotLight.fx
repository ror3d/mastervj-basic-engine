#include "globals.fxh"

Texture2D DiffuseTexture : register( t0 );
SamplerState LinearSampler : register( s0 );

struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD0;
	float4 WorldPos : TEXCOORD1;
	float3 WorldNormal : TEXCOORD2;
};

PS_INPUT VS(VS_INPUT IN)
{
	PS_INPUT l_Output = (PS_INPUT)0;
	
	l_Output.WorldPos = mul(float4(IN.Pos.xyz, 1.0), m_World);
	l_Output.Pos = mul(l_Output.WorldPos, m_View);
	l_Output.Pos = mul(l_Output.Pos, m_Projection);
	
	l_Output.WorldNormal = normalize(mul(normalize(IN.Normal), (float3x3)m_World));
	l_Output.UV = IN.UV;
	
	return l_Output;
}

float4 PS(PS_INPUT IN) : SV_Target
{
	float3 Nn = normalize(IN.WorldNormal);
	float3 L = m_LightPosition[0] - IN.WorldPos.xyz;
	
	//normalizamos L
	float dist = length(L);
	L /= dist;
	
	float fLinearAtten = clamp((m_LightAttenuationEndRangeArray[0] - dist) * (1/(m_LightAttenuationEndRangeArray[0] - m_LightAttenuationStartRange[0])), 0, 1);
	
	float fSpotAtten = 0.0f; // default value simplifies branch:
    if( m_LightAngleArray[0] > m_LightFallOffAngleArray[0] )
    {
        fSpotAtten = 1.0f;
    }
    else 
    {
		fSpotAtten = m_LightAngleArray[0] / m_LightFallOffAngleArray[0];
	}
	
	float fAtten = fLinearAtten * fSpotAtten;
	
	//calculos difuso
	 float l_DiffuseContrib = max(0, dot(Nn, -m_LightDirection[0])) * m_LightIntensityArray[0] * fAtten;
	//m_LightIntensity = Spotlight Factor
	float4 l_Albedo = DiffuseTexture.Sample(LinearSampler, IN.UV); // Textura
	
	//calculos Specular
	float3 Hn = normalize(normalize(m_CameraPosition - IN.WorldPos.xyz) + L);
	float l_SpecularContrib = pow(max(0, dot(Nn, Hn)), SpecularExp) * m_LightIntensityArray[0];
	
	return float4((m_LightAmbient.xyz * l_Albedo.xyz + l_DiffuseContrib * l_Albedo.xyz * m_LightColor[0] + fAtten * l_SpecularContrib * m_LightColor[0]), l_Albedo.a);
}

