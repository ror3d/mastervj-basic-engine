fn Init = (
	format "____Init____\n"
)


----------------------------------------------------------------------------
----------------------------------------------------------------------------
-- File functions
----------------------------------------------------------------------------
----------------------------------------------------------------------------
global file = undefined
global fileFromFOpen = undefined

fn MyCloseFile = (
	if file != undefined then
	(
		if fileFromFOpen then
		(
			fflush file
			fclose file
		)
		else
		(
			close file
		)
		file = undefined
	)
)

fn MyCreateFile path = (
	MyCloseFile()
	file = createfile (getAbsolutePath() + path)
	fileFromFOpen = false
)

fn MyFOpen path mode = (
	MyCloseFile()
	file = fopen (getAbsolutePath() + path) mode
	fileFromFOpen = true
)

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

fn HasLightmap mat = (
	return mat.selfIllumMap != undefined and classof mat.diffuseMap == Bitmaptexture and mat.selfIllumMap.bitmap != undefined
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

----------------------------------------------------------------------------
----------------------------------------------------------------------------
-- Paths
----------------------------------------------------------------------------
----------------------------------------------------------------------------
global dataPath = "Data\\"
global meshPath = dataPath + "Meshes\\"
global texturePath = dataPath + "Textures\\"

fn getAbsolutePath = (
	return (maxFilePath + dataPath)
)

fn createDirs = (
	if not doesFileExist (getAbsolutePath()) then
	(
		makeDir (getAbsolutePath()) all:true
	)
	if not doesFileExist (getAbsolutePath() + "\\Textures") then
	(
		makeDir (getAbsolutePath() + "\\Textures")
	)
	if not doesFileExist (getAbsolutePath() + "\\Meshes") then
	(
		makeDir (getAbsolutePath() + "\\Meshes")
	)
)

----------------------------------------------------------------------------
----------------------------------------------------------------------------
-- Export utilities
----------------------------------------------------------------------------
----------------------------------------------------------------------------

fn GetCores = (
	return $*
)

fn GetInstances = (
	return $*
)

fn GetLights = (
	light_obj = #()
	for i =1 to $*.count do
	(
		if SuperclassOf $*[i] == light then
		(
			append light_obj $*[i]
			format "\t<light =\"%\"/>" $*[i]
		)
	)
	format "<light_obj =\"%\"/>" light_obj
	return light_obj --Mirar el classof
)

fn createStaticMeshes = (
	MyCreateFile "static_meshes.xml"
	
	local cores = GetCores()
	
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<static_meshes>\n" to:file
	
	for i=1 to cores.count do
	(
		format "\t<static_mesh name=\"%\" filename=\"%.mesh\"/>\n" cores[i].name (meshPath + cores[i].name) to:file
	)
	
	format "</static_meshes>\n" to:file 
	format "____CreatedStatic____\n"
	
	MyCloseFile()
)
	
fn createRenderableObjects = (
	MyCreateFile "renderable_objects.xml"
	
	local instances = GetInstances()
	
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<renderable_objects>\n" to:file
	
	for i = 1 to instances.count do
	(
		Pos = TranslationToRH (instances[i].transform.translation)
		Yaw = GetYaw (instances[i].transform)
		Pitch = GetPitch (instances[i].transform)
		Roll = GetRoll (instances[i].transform)
		format "\t<mesh_instance name=\"%\" core_name=\"%\" pos=\"% % %\" yaw=\"%\" pitch=\"%\" roll=\"%\" visible=\"%\"/>\n" instances[i].name instances[i].name Pos[1] Pos[2] Pos[3] Yaw Pitch Roll instances[i].visibility to:file
	)

	format "</renderable_objects>\n" to:file 
	format "____CreatedRenderableObjects____\n"
	
	MyCloseFile()
)	

fn createLights = (
	MyCreateFile "lights.xml"
	
	local all_lights = GetLights()
	
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<lights>\n" to:file
	
	for i=1 to all_lights.count do
	(
		if  classof all_lights[i] == Omnilight then
		(
			-- Omni (omni)
			format "\t<light name=\"%\" type=\"%\" pos=\"%\" color=\"%\" att_start_range=\"0.0\" att_end_range=\"0.0\" intensity=\"%\"/>\n" all_lights[i].name all_lights[i].type all_lights[i].pos all_lights[i].rgb all_lights[i].farAttenStart all_lights[i].farAttenEnd all_lights[i].multiplier to:file
		
		)
		else if classof all_lights[i] == TargetDirectionallight then
		(
			-- Direcional (targetDirect)
			local dir=($.target.pos-$.pos)
			format "\t<light name=\"%\" type=\"%\" pos=\"%\" dir=\"%\" color=\"%\" att_start_range=\"0.0\" att_end_range=\"0.0\" intensity=\"%\"/>\n" all_lights[i].name all_lights[i].type all_lights[i].pos dir all_lights[i].rgb all_lights[i].farAttenStart all_lights[i].farAttenEnd all_lights[i].multiplier to:file
		
		)
		else if classof all_lights[i] == targetSpot then
		(
			--Spot  (targetSpot)
			local dir=($.target.pos-$.pos)
			format "\t<light name=\"%\" type=\"%\" pos=\"%\" dir=\"%\" color=\"%\" att_start_range=\"0.0\" att_end_range=\"0.0\" intensity=\"%\"/>\n" all_lights[i].name all_lights[i].type all_lights[i].pos dir all_lights[i].rgb all_lights[i].farAttenStart all_lights[i].farAttenEnd all_lights[i].multiplier to:file
		)
	)
	
	format "</lights>\n" to:file
	format "____CreatedRenderableObjects____\n"
	
	MyCloseFile()	
)

fn exportSingleTexture mat meshName = (
	if HasTexture mat then --TODO: comprobar difuso no bitmap??
	(
		textureName = filenameFromPath mat.diffuseMap.filename
		format "\t\t <texture filename=\"%\"/>\n" (texturePath + textureName) to:file
		copyFile mat.diffuseMap.filename (getAbsolutePath() + "Textures\\" + textureName)
	)
	else if mat.diffuseMap != undefined then
	(
		err = "¡¡¡El material " + mat.name + " de la malla " + meshName + " no hay textura difusa o no es un bitmap!!!"
		messageBox err title:"MENSAJE PARA ARTISTAS" 
		format ("\n$$$$ Alerta: " + err + "\n\n")
	)
)

fn createMaterials ExportTexture = (
	MyCreateFile "materials.xml"
	
	local cores = GetCores()
	
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<materials>\n" to:file
	
	for i=1 to cores.count do
	(
		if classof cores[i].material == Standardmaterial then
		(
			format "\t<material name=\"%\" effect_technique=\"diffuse_technique\" >\n" cores[i].material.name to:file
			
			exportSingleTexture cores[i].material cores[i].name
			
			if HasLightmap cores[i].material then
			(
				lightMapName = fileNameFromPath cores[i].material.selfIllumMap.filename
				format "\t\t <lightmap filename=\"%\"/>\n" (texturePath + lightMapName) to:file
				copyFile cores[i].material.selfIllumMap.filename (getAbsolutePath() + "Textures\\" + lightMapName)
			)
			
			format "\t</material>\n" to:file 
		)
		else -- Si se trata de un multimaterial
		(
			for j = 1 to cores[i].material.numsubs do
			(
				--Si ya existe no exportamos
				format "\t<material name=\"%\" effect_technique=\"diffuse_technique\" >\n" cores[i].material[j].name to:file	
				
				exportSingleTexture cores[i].material[j] cores[i].name
				
				format "\t</material>\n" to:file 
			)
		)
	)
	
	format "</materials>\n" to:file 
	format "____CreatedMaterials____\n"
	
	MyCloseFile()
)


function ConvertVtx Vtx =
(
	local l_Text = stringstream ""
	format "%" Vtx to:l_Text
	/*for i=1 to Vtx.count do
	(
		format "%," Vtx[i] to:l_Text
	)*/
	return (l_Text as string)
)

function DotNetAddVertex VtxArray VtxMap Vtx =
(
	local l_VtxString=ConvertVtx Vtx
	local l_Idx=1
	if (VtxMap.ContainsKey (l_VtxString))==false then
	(
		l_Idx=VtxArray.count
		append VtxArray Vtx
		VtxMap.Add l_VtxString l_Idx
	)
	else
		l_Idx=VtxMap.Item[l_VtxString]
	return l_Idx
)

fn createVertexesForFace obj IdFace = (
	local IdxsFace=getFace obj IdFace
	-- Para coger los vértices de la primera cara
	local Vtx1=getVert obj IdxsFace[1]
	local Vtx2=getVert obj IdxsFace[2]
	local Vtx3=getVert obj IdxsFace[3]
	-- Para coger las normales de los vértices de la primera cara
	local Normal1=getNormal obj IdxsFace[1]
	local Normal2=getNormal obj IdxsFace[2]
	local Normal3=getNormal obj IdxsFace[3]
	
	local v1 = vertex pos: Vtx1 normal: Normal1
	local v2 = vertex pos: Vtx2 normal: Normal2
	local v3 = vertex pos: Vtx3 normal: Normal3
	
	
	local numMaps = meshop.getNumMaps obj
	
	if numMaps > 1 then -- map[0] is the color, map[1] is the default texture, so at least 1 map (color) is always there
	(
		-- Para coger los índices de las coordenadas de textura de la primera cara, el 1 señala al primer
		--   conjunto de coordenadas aplicadas sobre la cara
		local IdxsMap=meshop.getMapFace obj 1 IdFace
		
		-- Para coger las coordenadas de los vértices de la primera cara
		local TUVMap1= meshop.getMapVert obj 1 IdxsMap[1]
		local TUVMap2= meshop.getMapVert obj 1 IdxsMap[2]
		local TUVMap3= meshop.getMapVert obj 1 IdxsMap[3]
		
		v1.uv = TUVMap1
		v2.uv = TUVMap2
		v3.uv = TUVMap3
		
		if numMaps > 2 then
		(
			local IdxsMap2=meshop.getMapFace obj 2 IdFace
			
			-- Coordenades de lightmap
			local LUVMap1= meshop.getMapVert obj 2 IdxsMap2[1]
			local LUVMap2= meshop.getMapVert obj 2 IdxsMap2[2]
			local LUVMap3= meshop.getMapVert obj 2 IdxsMap2[3]
			
			v1.uv2 = LUVMap1
			v2.uv2 = LUVMap2
			v3.uv2 = LUVMap3
		)
	)
	
	return #(v1, v2, v3)

)

fn createMesh obj = (
	format "____Creating Mesh %____\n" obj.name
	
	MyFOpen ("Meshes\\" + obj.name + ".mesh") "wb"
	
	--Header
	writeshort file 0xFE55
	local numMats
	
	--Materials
	if (classOf obj.material) == Standardmaterial then
	(
		numMats=1
		writeshort file numMats
		writeshort file obj.material.name.count
		writestring file obj.material.name
	)
	else
	(
		numMats = obj.material.numsubs
		writeshort file numMats
		for mats=1 to numMats do
		(	
			writeshort file obj.material[mats].name.count
			writestring file obj.material[mats].name
		)
	)
	
	--Vertex

	
	--TODO: Evitar duplicados
	for i=1 to numMats do
	(
		local l_VtxsMap = dotnetobject "System.Collections.Generic.Dictionary`2[System.String,System.Int16]"
		local idxs = #()
		local vtxs = #()
		
		--writeshort file 37 --vertexType
		--writeshort file 101
		local mat;
		if (classOf obj.material) == Standardmaterial then
		(
			mat = obj.material
		)
		else
		(
			mat = obj.material[i]
		)
		
		writeshort file (GetVertexTypeFromMaterial mat)
		
		--TODO: coger vertices afectados por ese mat...
		for idFace = 1 to (getNumFaces obj) do
		(
			local faceVtxs = createVertexesForFace obj idFace
			for numVtx = 1 to 3 do
			(
				local idx = DotNetAddVertex vtxs l_VtxsMap faceVtxs[numVtx]
				append idxs idx
			)
		)
		
		
		writeshort file vtxs.count
		
		for idVtx = 1 to vtxs.count do
		(
			local vtx = vtxs[idVtx]
			-- Vertex
			writefloat file vtx.pos.x
			writefloat file vtx.pos.z
			writefloat file vtx.pos.y
			
			if HasNormal mat then
			(
				-- Normal
				writefloat file vtx.normal.x
				writefloat file vtx.normal.z
				writefloat file vtx.normal.y
			)
			
			if HasTexture mat then
			(
				-- UV
				writefloat file vtx.uv.x
				writefloat file vtx.uv.y
			)
			
			if HasLightmap mat then
			(
				--UV Lightmap
				writefloat file vtx.uv2.x
				writefloat file vtx.uv2.y
			)
		)
		
		-- Write the index byte width
		writeshort file 16
		
		writeshort file idxs.count
		
		for idx = 1 to idxs.count do
		(
			writeshort file idxs[idx]
		)
	)

	
	--Footer
	writeshort file 0x55FE
	
	MyCloseFile()
	
	format "____Created Mesh %____\n" obj.name
)

fn createMeshes = (
	local cores = GetCores()
	for i=1 to cores.count do
	(
		createMesh (convertToMesh cores[i])
	)
)


fn GetYaw transform =
(
	return -(transform as eulerangles).z
)

fn GetPitch transform =
(
	return -(transform as eulerangles).y
)

fn GetRoll transform =
(
	return (transform as eulerangles).x
) 


fn TranslationToRH translation =
(
	return point3 -translation.x translation.z translation.y
) 

createDirs()
createStaticMeshes()
createRenderableObjects()
createMaterials true
createMeshes()

