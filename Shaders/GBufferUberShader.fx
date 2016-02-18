//ForwardShadingUberShader.fx
#include "globals.fxh"
#include "samplers.fxh"
//----------------------------------------------------------------------

struct VS_INPUT
{
	float3 Pos		: POSITION;

#ifdef HAS_NORMAL
	float3 Normal	: NORMAL;
#endif // HAS_NORMAL

#ifdef HAS_TANGENT_BINORMAL
	float4 Binormal : BINORMAL;
	float4 Tangent : TANGENT;
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
	
	float4 HPos : TEXCOORD3;
	
	float4 ViewPos : POSITION1;
	
#ifdef HAS_NORMAL
	float3 Normal: NORMAL0;
	float3 ViewNormal: NORMAL1;
#endif // HAS_NORMAL

#ifdef HAS_TANGENT_BINORMAL
	float3 Binormal : BINORMAL;
	float3 Tangent : TANGENT;
#endif // HAS_TANGENT_BINORMAL

#ifdef HAS_UV
	float2 UV : TEXCOORD0;
#endif // HAS_UV

#ifdef HAS_UV2
	float2 UV2 : TEXCOORD1;
#endif // HAS_UV2
};

struct PS_OUTPUT
{
	float4 Albedo;
	float4 LightMap;
	float4 ViewNormal;
	float4 ViewPosAndDepth;
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
	
#ifdef HAS_TANGENT_BINORMAL
	l_Output.Tangent = mul(IN.Tangent.xyz,(float3x3) m_World);
	l_Output.Binormal = mul(cross(IN.Tangent.xyz,IN.Normal),(float3x3)m_World);
#endif // HAS_TANGENT_BINORMAL

#ifdef HAS_UV
	l_Output.UV = IN.UV;
#endif // HAS_UV

#ifdef HAS_UV2
	l_Output.UV2 = IN.UV2;
#endif // HAS_UV2



	l_Output.Pos = mul(float4(l_Position.xyz, 1.0), m_World);
	l_Output.Pos = mul(l_Output.Pos, m_View);
	
	l_Output.ViewPos = l_Output.Pos;
	
	l_Output.Pos = mul(l_Output.Pos, m_Projection);
	l_Output.HPos = l_Output.Pos;
	
#ifdef HAS_NORMAL
	l_Output.Normal = normalize(mul(l_Normal, (float3x3)m_World));
	l_Output.ViewNormal = normalize(mul(l_Output.Normal, (float3x3)m_View));
#endif // HAS_NORMAL

#ifdef HAS_UV
	l_Output.UV = IN.UV;
#endif // HAS_UV
	
	return l_Output;
}


//----------------------------------------------------------------------
// Pixel Shader
//----------------------------------------------------------------------
PS_OUTPUT PS( PS_INPUT IN) : SV_Target
{
	PS_OUTPUT l_Return = (PS_OUTPUT)0;
	float4 l_Albedo = float4(1, 1, 1, 1);
	
	float4 l_ViewNormal = float4(0.5f, 0.5f, 1.0f, 1.0f);
	
	float3 l_LightMapColor = m_LightAmbient.xyz;
	
#ifdef HAS_NORMAL
	float3 Nn = normalize(IN.Normal.xyz);
#endif // HAS_NORMAL

#ifdef HAS_TANGENT_BINORMAL
	float3 Tn = normalize(IN.Tangent);
	float3 Bn = normalize(IN.Binormal);
#endif // HAS_TANGENT_BINORMAL

#ifdef HAS_UV
	l_Albedo = T0Texture.Sample(S0Sampler, IN.UV);
#endif

#ifdef HAS_UV2
	l_LightMapColor = l_LightMapColor + T1Texture.Sample(S1Sampler, IN.UV2).xyz; // Maybe should be * instead of +?
#endif // HAS_UV2

#if defined(HAS_TANGENT_BINORMAL) && defined(HAS_NORMAL)
	float3 bump = m_Bump * (T1Texture.Sample(S1Sampler, IN.UV).rgb - float3(0.5,0.5,0.5));
	Nn = Nn + bump.x * Tn - bump.y * Bn;
	Nn = mul(Nn, (float3x3)m_View);
#elif defined(HAS_NORMAL)
	Nn = IN.ViewNormal.xyz;
#endif // HAS_NORMAL_MAP

#if defined(HAS_NORMAL)
	l_ViewNormal = float4(normalize(Nn.xyz).xyz, 1.0f);
	l_ViewNormal.xy = (l_ViewNormal.xy/2.0f)+float2(0.5f, 0.5f);
#endif

	l_Return.Albedo = float4(l_Albedo.xyz, 1.0f);
	
	l_Return.LightMap = float4(l_LightMapColor, 1.0f/80.f); // Lightmap + Specular Exponent
	l_Return.ViewNormal = l_ViewNormal;
	
	float l_Depth=IN.HPos.z/IN.HPos.w;
	l_Return.ViewPosAndDepth = float4(IN.ViewPos.xyz, l_Depth);
	
	return l_Return;
}
