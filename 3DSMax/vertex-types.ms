----------------------------------------------------------------------------
----------------------------------------------------------------------------
-- Vertex Type Functions
----------------------------------------------------------------------------
----------------------------------------------------------------------------
struct vertex (pos, normal, uv, uv2)

fn HasTexture mat = (
	return mat.diffuseMap != undefined and classof mat.diffuseMap == Bitmaptexture and mat.diffuseMap.bitmap != undefined
)

fn HasNormal mat = (
	return true -- TODO: change when we have normal maps
)

fn HasWeights mat = (
	return false -- TODO: change when we have skeletal animations
)

fn HasLightmap mat alertOnMissing:false = (
	local ret = mat.selfIllumMap != undefined and classof mat.diffuseMap == Bitmaptexture and mat.selfIllumMap.bitmap != undefined
	if alertOnMissing and not ret and mat.selfIllumMap != undefined then
	(
		-- TODO: show message box?
		format "\n$$$$ Alerta: El material % parece tener Lightmap pero no tiene una textura asociada válida!!!\n\n" mat.name
	)
	return ret
)

fn GetVertexTypeFromMaterial mat = (
	local MV_VERTEX_TYPE_POSITION		=	0x01
	local MV_VERTEX_TYPE_COLOR			=	0x02
	local MV_VERTEX_TYPE_NORMAL			=	0x04
	local MV_VERTEX_TYPE_BLEND_INDICES	=	0x08
	local MV_VERTEX_TYPE_BLEND_WEIGHT	=	0x10
	local MV_VERTEX_TYPE_TEXTURE1		=	0x20
	local MV_VERTEX_TYPE_TEXTURE2		=	0x40
	local MV_VERTEX_TYPE_POSITION4		=	0x80
	
	local ret = 0;
	
	ret = bit.or ret MV_VERTEX_TYPE_POSITION
	if HasTexture mat then
		ret = bit.or ret MV_VERTEX_TYPE_TEXTURE1
	
	if HasNormal mat then
		ret = bit.or ret MV_VERTEX_TYPE_NORMAL
	
	if HasWeights mat then
		ret = bit.or ret MV_VERTEX_TYPE_BLEND_INDICES
		ret = bit.or ret MV_VERTEX_TYPE_BLEND_WEIGHT
	
	if HasLightmap mat then
		ret = bit.or ret MV_VERTEX_TYPE_TEXTURE2
	
	-- TODO do the rest
	
	return ret
)
