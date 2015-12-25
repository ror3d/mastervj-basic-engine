//ForwardShadingUberShader.fx
#include "globals.fxh"
Texture2D DiffuseTexture : register( t0 );
SamplerState LinearSampler : register( s0 );
//----------------------------------------------------------------------

struct VS_INPUT
{
	float3 Pos		: POSITION;

#ifdef HAS_WEIGHT_INDICES
	float4 Weight	: BLENDWEIGHT;
	float4 Indices	: BLENDINDICES;
#endif // HAS_WEIGHT_INDICES

#ifdef HAS_NORMAL
	float3 Normal	: NORMAL;
#endif // HAS_NORMAL

#ifdef HAS_UV
	float2 UV		: TEXCOORD0;
#endif // HAS_UV
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	
#ifdef HAS_NORMAL
	float3 Normal: POSITION1;
#endif // HAS_NORMAL

#ifdef HAS_UV
	float2 UV : TEXCOORD0;
#endif // HAS_UV
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
	
#ifdef HAS_UV
	l_Output.UV = IN.UV;
#endif // HAS_UV



	l_Output.Pos = mul(float4(l_Position.xyz, 1.0), m_World);
	l_Output.Pos = mul(l_Output.Pos, m_View);
	l_Output.Pos = mul(l_Output.Pos, m_Projection);

#ifdef HAS_NORMAL
	l_Output.Normal = normalize(mul(l_Normal, (float3x3)m_World));
#endif // HAS_NORMAL

#ifdef HAS_UV
	l_Output.UV = IN.UV;
#endif // HAS_UV
	
	return l_Output;
}


//----------------------------------------------------------------------
// Pixel Shader
//----------------------------------------------------------------------
float4 PS( PS_INPUT IN) : SV_Target
{
	float4 l_Return = 0;
	float4 l_Albedo = 0;
	
#ifdef HAS_UV
	l_Albedo = DiffuseTexture.Sample(LinearSampler, IN.UV);
#endif

	l_Return = l_Albedo;
	
	return l_Return;
}
