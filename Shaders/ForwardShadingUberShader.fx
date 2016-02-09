//ForwardShadingUberShader.fx
#include "globals.fxh"
Texture2D DiffuseTexture : register( t0 );
SamplerState LinearSampler : register( s0 );

#ifdef HAS_UV2
Texture2D LightMapTexture : register( t1 );
SamplerState LightMapSampler : register( s1 );
#endif // HAS_UV2

#ifdef HAS_NORMAL_MAP
Texture2D NormalMapTexture : register( t1 );
SamplerState NormalMapSampler : register( s1 );
#endif // HAS_NORMAL_MAP

#ifdef HAS_ENVIRONMENT_MAP
Texture2D EnvironmentMapTexture : register( t1 );
SamplerState EnvironmentMapSampler : register( s1 );
#endif // HAS_ENVIRONMENT_MAP
//----------------------------------------------------------------------

struct VS_INPUT
{
	float3 Pos		: POSITION;

#ifdef HAS_NORMAL
	float3 Normal	: NORMAL;
#endif // HAS_NORMAL

#ifdef HAS_TANGENT_BINORMAL
	float4 Tangent : TANGENT;
	float4 Binormal : BINORMAL;
#endif // HAS_TANGENT_BINORMAL

#ifdef HAS_WEIGHT_INDICES
	float4 Weight	: BLENDWEIGHT;
	float4 Indices	: BLENDINDICES;
#endif // HAS_WEIGHT_INDICES

#ifdef HAS_UV
	float2 UV		: TEXCOORD0;
#endif // HAS_UV

#ifdef HAS_UV2
	float2 UV2		: TEXCOORD1;
#endif // HAS_UV2
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 WorldPos : POSITION1;

#ifdef HAS_NORMAL
	float3 Normal: NORMAL;
#endif // HAS_NORMAL

#ifdef HAS_TANGENT_BINORMAL
	float3 Tangent : TANGENT;
	float3 Binormal : BINORMAL;
#endif // HAS_TANGENT_BINORMAL

#ifdef HAS_UV
	float2 UV : TEXCOORD0;
#endif // HAS_UV

#ifdef HAS_UV2
	float2 UV2 : TEXCOORD1;
#endif // HAS_UV2
};

//----------------------------------------------------------------------
// Vertex Shader
//----------------------------------------------------------------------
PS_INPUT VS( VS_INPUT IN )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	
#ifndef HAS_WEIGHT_INDICES
	
	float4 l_Position = float4(IN.Pos.xyz, 1.0);
	
	#ifdef HAS_NORMAL
		float3 l_Normal = normalize(IN.Normal).xyz;
	#endif // HAS_NORMAL

#else // !HAS_WEIGHT_INDICES
	float4 l_Position = 0;
	float4 l_TempPos = float4(IN.Pos.xyz, 1.0);
	
	float4 l_Indices=IN.Indices;
	l_Position=mul(l_TempPos, m_Bones[l_Indices.x]) * IN.Weight.x;
	l_Position+=mul(l_TempPos, m_Bones[l_Indices.y]) * IN.Weight.y;
	l_Position+=mul(l_TempPos, m_Bones[l_Indices.z]) * IN.Weight.z;
	l_Position+=mul(l_TempPos, m_Bones[l_Indices.w]) * IN.Weight.w;
	
	#ifdef HAS_NORMAL
		float3 l_Normal = 0;
		float3x3 m;
		m[0].xyz = m_Bones[l_Indices.x][0].xyz;
		m[1].xyz = m_Bones[l_Indices.x][1].xyz;
		m[2].xyz = m_Bones[l_Indices.x][2].xyz;
		
		l_Normal += mul(IN.Normal.xyz, m)* IN.Weight.x;
		
		m[0].xyz = m_Bones[l_Indices.y][0].xyz;
		m[1].xyz = m_Bones[l_Indices.y][1].xyz;
		m[2].xyz = m_Bones[l_Indices.y][2].xyz;
		
		l_Normal += normalize(mul(IN.Normal.xyz, m)* IN.Weight.y);
		
		l_Normal = normalize(l_Normal);
	#endif
	
#endif // !HAS_WEIGHT_INDICES

	l_Output.Pos = mul(float4(l_Position.xyz, 1.0), m_World);
	l_Output.Pos = mul(l_Output.Pos, m_View);
	l_Output.Pos = mul(l_Output.Pos, m_Projection);

#ifdef HAS_NORMAL
	l_Output.Normal = normalize(mul(l_Normal, (float3x3)m_World));
#endif // HAS_NORMAL

#ifdef HAS_TANGENT_BINORMAL
	l_Output.Tangent = mul(IN.Tangent.xyz,(float3x3) m_World);
	l_Output.Binormal = mul(cross(IN.Tangent.xyz,IN.Normal),(float3x3)m_World);
#endif // HAS_TANGENT_BINORMAL

#ifdef HAS_UV
	l_Output.UV = IN.UV;
#endif // HAS_UV

#ifdef HAS_UV2
	l_Output.UV2 = IN.UV2;
#endif // HAS_UV
	
	l_Output.WorldPos = mul(float4(IN.Pos.xyz, 1.0), m_World);
	
	return l_Output;
}


//----------------------------------------------------------------------
// Pixel Shader
//----------------------------------------------------------------------
float4 PS( PS_INPUT IN) : SV_Target
{
	float4 l_Return = 0;
	float4 l_Albedo = 0;
	
#ifdef HAS_NORMAL
	float3 Nn = normalize(IN.Normal.xyz);
#endif

#ifdef HAS_TANGENT_BINORMAL
	float3 Tn = normalize(IN.Tangent);
	float3 Bn = normalize(IN.Binormal);
#endif // HAS_TANGENT_BINORMAL

	float3 Hn = 0;
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
	
#ifdef HAS_UV
	l_Albedo = DiffuseTexture.Sample(LinearSampler, IN.UV);
#endif // HAS_UV

#ifdef HAS_UV2
	l_Albedo = l_Albedo * LightMapTexture.Sample(LightMapSampler, IN.UV2);
#endif // HAS_UV2

#ifdef HAS_NORMAL_MAP
	float3 bump = m_Bump * (NormalMapTexture.Sample(NormalMapSampler, IN.UV).rgb - float3(0.5,0.5,0.5));
	Nn = Nn + bump.x * Tn + bump.y * Bn;
	Nn = normalize(Nn);
#endif // HAS_NORMAL_MAP

#ifdef HAS_LIGHTS
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
	
	l_Albedo = float4((m_LightAmbient.xyz * l_Albedo.xyz + l_DiffuseContrib * l_Albedo.xyz + l_SpecularContrib), l_Albedo.a);
#endif // HAS_LIGHTS

#ifdef HAS_ENVIRONMENT_MAP
	float3 l_EyeToWorldPosition = normalize(IN.WorldPos - m_ViewInverse[3].xyz);
	float3 l_ReflectVector = normalize(reflect(l_EyeToWorldPosition, Nn));
	float3 l_ReflectColor = EnvironmentMapTexture.Sample(EnvironmentMapSampler, l_ReflectVector).xyz * g_EnvironmentFactor;
	
	l_Albedo = float4(l_Albedo.xyz + l_ReflectColor, l_Albedo.a);
#endif //HAS_ENVIRONMENT_MAP

	l_Return = l_Albedo;
	
	return l_Return;
}
