fn Init = (
	format "____Init____\n"
)

global dataPath = "Data/"
global meshPath = dataPath + "Meshes/"
global texturePath = dataPath + "Textures/"

fn getAbsolutePath = (
	return (maxFilePath + dataPath)
)

fn createDirs = (
	if not doesFileExist (getAbsolutePath()) then
	(
		makeDir (getAbsolutePath()) all:true
	)
	if not doesFileExist (getAbsolutePath() + "/Textures") then
	(
		makeDir (getAbsolutePath() + "/Textures")
	)
	if not doesFileExist (getAbsolutePath() + "/Meshes") then
	(
		makeDir (getAbsolutePath() + "/Meshes")
	)
)

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
	local file = createfile (getAbsolutePath() + "static_meshes.xml")
	local cores = GetCores()
	
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<static_meshes>\n" to:file
	
	for i=1 to cores.count do
	(
		format "\t<static_mesh name=\"%\" filename=\"%.mesh\"/>\n" cores[i].name (meshPath + cores[i].name) to:file
	)
	
	format "</static_meshes>\n" to:file 
	format "____CreatedStatic____\n"
	close file
)
	
fn createRenderableObjects = (
	local file = createfile (getAbsolutePath() + "renderable_objects.xml" )
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
	close file
)	

fn createLights = (
	local file = createfile (getAbsolutePath() + "lights.xml")
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

	format "</lights>\n" to:file
	format "____CreatedRenderableObjects____\n"
	close file
	)	
)

fn createMaterials ExportTexture = (
	local file = createfile (getAbsolutePath() + "materials.xml" )
	local cores = GetCores()
	
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<materials>\n" to:file
	
	for i=1 to cores.count do
	(
		if classof cores[i].material == Standardmaterial then
		(
			format "\t<material name=\"%\" effect_technique=\"lightmap_technique\" >\n" cores[i].material.name to:file
			if cores[i].material.diffuseMap != undefined and cores[i].material.diffuseMap.bitmap != undefined then --TODO: comprobar difuso no bitmap??
			(
				textureName = filenameFromPath cores[i].material.diffuseMap.filename
				format "\t\t <texture filename=\"%\"/>\n" (texturePath + textureName) to:file
				copyFile cores[i].material.diffuseMap.filename (getAbsolutePath() + "Textures/" + textureName)
			)
			else
			(
				messageBox ("¡¡¡El material " + cores[i].material.name + " de la malla " + cores[i].name + " tiene las texturas mal!!!") title:"MENSAJE PARA ARTISTAS" 
			)
			
			if cores[i].material.selfIllumMap != undefined and cores[i].material.selfIllumMap.bitmap != undefined then
			(
				lightMapName = fileNameFromPath cores[i].material.selfIllumMap.filename
				format "\t\t <lightmap filename=\"%\"/>\n" (texturePath + lightMapName) to:file
				copyFile cores[i].material.selfIllumMap.filename (getAbsolutePath() + "Textures/" + lightMapName)
			)
			
			format "\t</material>\n" to:file 
		)
		else
		(
			for j = 1 to cores[i].material.numsubs do
			(
				--Si ya existe no exportamos
				format "\t<material name=\"%\" effect_technique=\"diffuse_technique\" >\n" cores[i].material[j].name to:file	
				if cores[i].material[j].diffuseMap != undefined and cores[i].material[j].diffuseMap.bitmap != undefined then --TODO: comprobar difuso no bitmap??
				(
					textureName = filenameFromPath cores[i].material[j].diffuseMap.filename
					format "\t\t <texture filename=\"%\"/>\n" (texturePath + textureName) to:file
					copyFile cores[i].material[j].diffuseMap.filename (getAbsolutePath() + "Textures/" + textureName)
				)
				else
				(
					messageBox ("¡¡¡El material " + cores[i].material.name + " de la malla " + cores[i].name + " tiene las texturas mal!!!") title:"MENSAJE PARA ARTISTAS" 
				)
				format "\t</material>\n" to:file 
			)
		)
	)
	
	format "</materials>\n" to:file 
	format "____CreatedMaterials____\n"
	close file
)

global file

function ConvertVtx Vtx =
(
	local l_Text = stringstream ""
	local l_Value=""
	for i=1 to Vtx.count do
	(
		format "%," Vtx[i] to:l_Text
	)
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
	-- Para coger los índices de las coordenadas de textura de la primera cara, el 1 señala al primer
	--   conjunto de coordenadas aplicadas sobre la cara
	local IdxsMap=meshop.getMapFace obj 1 IdFace
	-- Para coger las coordenadas de los vértices de la primera cara
	local TUVMap1= meshop.getMapVert obj 1 IdxsMap[1]
	local TUVMap2= meshop.getMapVert obj 1 IdxsMap[2]
	local TUVMap3= meshop.getMapVert obj 1 IdxsMap[3]
	
	-- Coordenades de lightmap
	local LUVMap1= meshop.getMapVert obj 2 IdxsMap[1]
	local LUVMap2= meshop.getMapVert obj 2 IdxsMap[2]
	local LUVMap3= meshop.getMapVert obj 2 IdxsMap[3]
	
	return #(#(Vtx1,Normal1,TUVMap1,LUVMap1), #(Vtx2,Normal2,TUVMap2,LUVMap2), #(Vtx3,Normal3,TUVMap3,LUVMap3))

)

fn createMesh obj = (
	format "____Creating Mesh %____\n" obj.name
	
	file = fopen (getAbsolutePath() + "Meshes/" + obj.name + ".mesh") "wb"
	
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
		writeshort file 101
		--writeshort file (getNumVerts obj) --TODO: coger vertices afectados por ese mat...
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
			writefloat file vtx[1].x
			writefloat file vtx[1].z
			writefloat file vtx[1].y
			
			-- Normal
			writefloat file vtx[2].x
			writefloat file vtx[2].z
			writefloat file vtx[2].y
			
			-- UV
			writefloat file vtx[3].x
			writefloat file vtx[3].y
			
			--UV Lightmap
			writefloat file vtx[4].x
			writefloat file vtx[4].y
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
	
	fflush file
	fclose file
	
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

