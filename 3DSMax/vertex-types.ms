----------------------------------------------------------------------------
----------------------------------------------------------------------------
-- Vertex Type Functions
----------------------------------------------------------------------------
----------------------------------------------------------------------------
struct vertex (pos, normal, uv, uv2, binormal, tangencial)

fn DisplayMaterialError errorString = (
	messageBox errorString title:"MENSAJE PARA ARTISTAS"
	format ("\n$$$$ Alerta: " + errorString + "\n\n")
)

fn HasColor mat alertOnMissing:false = (
	local ret = false
	
	return ret
)

fn HasTexture mat alertOnMissing:false = (
	local ret = mat.diffuseMap != undefined and classof mat.diffuseMap == Bitmaptexture and mat.diffuseMap.bitmap != undefined
	if alertOnMissing and not ret then
	(
		DisplayMaterialError ("En el material " + mat.name + " no hay textura difusa o no es un bitmap!!!")
	)
	return ret
)

fn HasReflection mat alertOnMissing:false = (
	local ret = mat.reflectionMap != undefined and classof mat.reflectionMap == Bitmaptexture and mat.reflectionMap.bitmap != undefined
	if alertOnMissing and not ret then
	(
		DisplayMaterialError ("En el material " + mat.name + " no hay textura de skymap o no es un bitmap!!!")
	)
	return ret
)

fn HasNormal mat alertOnMissing:false = (
	local ret = mat.bumpMap != undefined and classof mat.bumpMap == Bitmaptexture and mat.bumpMap.bitmap != undefined
	if alertOnMissing and not ret then
	(
		DisplayMaterialError ("En el material " + mat.name + " no hay textura normal o no es un bitmap!!!")
	)
	return ret
)

fn HasWeights mat = (
	return false -- TODO: change when we have skeletal animations
)

fn HasLightmap mat alertOnMissing:false = (
	local ret = mat.selfIllumMap != undefined and classof mat.diffuseMap == Bitmaptexture and mat.selfIllumMap.bitmap != undefined
	if alertOnMissing and not ret and mat.selfIllumMap != undefined then
	(
		DisplayMaterialError ("El material " + mat.name + " parece tener Lightmap pero no tiene una textura asociada v�lida!!!")
	)
	return ret
)

fn GetVertexTypeFromObjectMaterial obj mat = (
	local MV_VERTEX_TYPE_POSITION		=	0x01
	local MV_VERTEX_TYPE_COLOR			=	0x02
	local MV_VERTEX_TYPE_NORMAL			=	0x04
	local MV_VERTEX_TYPE_BLEND_INDICES	=	0x08
	local MV_VERTEX_TYPE_BLEND_WEIGHT	=	0x10
	local MV_VERTEX_TYPE_TEXTURE1		=	0x20
	local MV_VERTEX_TYPE_TEXTURE2		=	0x40
	local MV_VERTEX_TYPE_POSITION4		=	0x80
	local MV_VERTEX_TYPE_NORMAL4		=   0x100
	local MV_VERTEX_TYPE_BINORMAL		=	0x200
	local MV_VERTEX_TYPE_TANGENT		=	0x400
	
	local ret = 0;
	
	ret = bit.or ret MV_VERTEX_TYPE_POSITION
	ret = bit.or ret MV_VERTEX_TYPE_NORMAL
	
	if (HasTexture mat) or (HasReflection mat) then
	(
		ret = bit.or ret MV_VERTEX_TYPE_TEXTURE1
	)
	
	
	if HasNormal mat then
	(
		ret = bit.or ret MV_VERTEX_TYPE_BINORMAL
		ret = bit.or ret MV_VERTEX_TYPE_TANGENT
	)
	
	/*if HasWeights mat then
	(
		ret = bit.or ret MV_VERTEX_TYPE_BLEND_INDICES
		ret = bit.or ret MV_VERTEX_TYPE_BLEND_WEIGHT
	)*/
	
	if HasLightmap mat then
	(
		ret = bit.or ret MV_VERTEX_TYPE_TEXTURE2
	)
	
	if HasColor mat then
	(
		ret = bit.or ret MV_VERTEX_TYPE_COLOR
	)
	
	-- TODO position4, HasWeights, and HasColor
	
	return ret
)
