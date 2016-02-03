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
	float3 Nn = normalize(IN.WorldNormal.xyz);
	float3 Hn = 1;
	float3 L = 0;
	float dist = 0;
	float4 l_DiffuseContrib = 0;
	float4 l_SpecularContrib = 0;
	float linearAtten = 1;
	float spotAtten = 1;
	float atten = 1;
	float cosAngle = 0;
	float cosFallOff = 1;
	float cosAngleLight = 0;
	
	for (uint i = 0; i < MAX_LIGHTS_BY_SHADER; i++)
	{
		linearAtten = 1;
		spotAtten = 1;
		
		if (m_LightEnabledArray[i])
		{
			if (m_LightTypeArray[i] != DIRECTIONAL)
			{
				L = m_LightPosition[i] - IN.WorldPos.xyz;
				
				dist = length(L);
				L /= dist;
				
				linearAtten = clamp((m_LightAttenuationEndRangeArray[i] - dist) * (1/(m_LightAttenuationEndRangeArray[i] - m_LightAttenuationStartRange[i])), 0, 1);
			}
			else
			{
				L = -m_LightDirection[i];
			}
			
			if (m_LightTypeArray[i] == SPOT)
			{	
				cosAngle = cos(m_LightAngleArray[i]);
				cosFallOff = cos(m_LightFallOffAngleArray[i]);
				cosAngleLight = max(0, dot(-L, m_LightDirection[i]));
				
				spotAtten = saturate((cosAngleLight - cosFallOff) / (cosAngle - cosFallOff));
			}
			
			atten = linearAtten * spotAtten;
			
			l_DiffuseContrib += max(0, dot(Nn, L)) * m_LightIntensityArray[i] * atten * m_LightColor[i];
			
			Hn = normalize(normalize(m_CameraPosition - IN.WorldPos.xyz) + L);
			l_SpecularContrib += pow(max(0, dot(Nn, Hn)), SpecularExp) * m_LightIntensityArray[i] * atten * m_LightColor[i];
		}
	}
	
	float4 l_Albedo = DiffuseTexture.Sample(LinearSampler, IN.UV); // Textura

	return float4((m_LightAmbient.xyz * l_Albedo.xyz + l_DiffuseContrib * l_Albedo.xyz + l_SpecularContrib), l_Albedo.a);
}