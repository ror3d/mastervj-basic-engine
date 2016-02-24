//ForwardShadingUberShader.fx
#include "globals.fxh"
#include "samplers.fxh"


//----------------------------------------------------------------------

struct VS_INPUT
{
	float3 Pos		: POSITION;
	float2 UV : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD0;
};

//----------------------------------------------------------------------
// Vertex Shader
//----------------------------------------------------------------------
PS_INPUT VS( VS_INPUT IN )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos=float4(IN.Pos.xyz, 1.0);
	
	l_Output.UV=IN.UV;
	
	return l_Output;
}

float4 GetPositionFromZDepthViewInViewCoordinates(float ZDepthView, float2 UV, float4x4
InverseProjection)
{
	// Get the depth value for this pixel
	// Get x/w and y/w from the viewport position
	float x = UV.x * 2 - 1;
	float y = (1 - UV.y) * 2 - 1;
	float4 l_ProjectedPos = float4(x, y, ZDepthView, 1.0);
	// Transform by the inverse projection matrix
	float4 l_PositionVS = mul(l_ProjectedPos, InverseProjection);
	// Divide by w to get the view-space position
	return l_PositionVS;
}

float3 GetPositionFromZDepthView(float ZDepthView, float2 UV, float4x4 InverseView, float4x4
InverseProjection)
{
	float4 l_PositionView=GetPositionFromZDepthViewInViewCoordinates(ZDepthView, UV,
	InverseProjection);
	float4 l_WorldPos = mul(l_PositionView/l_PositionView.w, InverseView);
	return l_WorldPos.xyz;
}

//----------------------------------------------------------------------
// Pixel Shader
//----------------------------------------------------------------------
float4 PS( PS_INPUT IN) : SV_Target
{
	float4 l_Depth;
	float4 l_WPos;
	float4 l_ViewPos;
	float4 l_WNorm;
	
	float4 l_Return = 0;
	float4 l_Albedo = 0;
	float l_SpecularFactor = 1.0f;
	float l_SpecularExp = 1.0f;
	
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
	
	
	l_ViewPos = T3Texture.Sample(S3Sampler, IN.UV);
	l_Depth = l_ViewPos.w;
	l_ViewPos.w = 1.0f;
	
	
	if(l_Depth.x == 1)
	{
		clip(-1);
	}
	
	l_Albedo = T0Texture.Sample(S0Sampler, IN.UV);
	
	
	l_WNorm = T2Texture.Sample(S2Sampler, IN.UV);
	l_WNorm.xy = (l_WNorm.xy-float2(0.5f, 0.5f))*2.0f;
	l_WNorm.xyz = mul(l_WNorm.xyz, (float3x3)(m_ViewInverse));
	
	//*/
	l_WPos.xyz = GetPositionFromZDepthView(l_Depth, IN.UV, m_ViewInverse, m_ProjectionInverse);
	l_WPos.w = 1.0f;
	//*/
	
	//l_WPos = mul(l_ViewPos, m_ViewInverse);
	
	l_SpecularFactor = 1.0/l_Albedo.a;
	l_Albedo = float4(l_Albedo.xyz, 1.0f);
	
	float4 l_Lightmap = T1Texture.Sample(S1Sampler, IN.UV);
	l_SpecularExp = 1.0f/l_Lightmap.a;

	linearAtten = 1;
	spotAtten = 1;
	
	if (m_LightTypeArray[0] != DIRECTIONAL)
	{
		L = m_LightPosition[0] - l_WPos.xyz;
		
		dist = length(L);
		L /= dist;
		
		linearAtten = clamp((m_LightAttenuationEndRangeArray[0] - dist) * (1/(m_LightAttenuationEndRangeArray[0] - m_LightAttenuationStartRange[0])), 0, 1);
	}
	else
	{
		L = -m_LightDirection[0];
	}
	
	if (m_LightTypeArray[0] == SPOT)
	{
		cosAngle = cos(m_LightAngleArray[0]);
		cosFallOff = cos(m_LightFallOffAngleArray[0]);
		cosAngleLight = max(0, dot(-L, m_LightDirection[0]));
		
		spotAtten = saturate((cosAngleLight - cosFallOff) / (cosAngle - cosFallOff));
	}
	
	atten = linearAtten * spotAtten;
	
	l_DiffuseContrib = max(0, dot(l_WNorm.xyz, L)) * m_LightColor[0];

	
	Hn = normalize(normalize(m_CameraPosition - l_WPos.xyz) + L);
	l_SpecularContrib = pow(max(0, dot(l_WNorm.xyz, Hn)), l_SpecularExp) * m_LightColor[0];
	
	
	l_Albedo = float4((l_DiffuseContrib * l_Albedo.xyz + l_SpecularContrib), 1.0f)*atten * m_LightIntensityArray[0];
	//l_Albedo = float4((l_DiffuseContrib.xyz * l_Albedo.xyz), 1.0f)*atten * m_LightIntensityArray[0];
	//l_Albedo = float4(1,0,0, 1.0f);
	l_Return = l_Albedo;
	
	return l_Return;
}
