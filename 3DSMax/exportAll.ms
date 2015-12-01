fn Init =  (
	format "____Init____\n"
)		

fn GetLevelName = (
	return maxFilePath + "Data\\Level\\"
)

fn GetCores = (
	return $*
	)
fn GetInstances = (
	return $*
)

fn createStaticMeshes = (
	local file=createfile (GetLevelName()+"static_meshes.xml" )
	local cores = GetCores()
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<static_meshes>\n" to:file
	for i=1 to cores.count do(
		format "\t<static_mesh name=\"%\" filename=\"%.mesh\">\n" cores[i].name (GetLevelName()+cores[i].name) to:file
		)
	
	format "</static_meshes>\n" to:file 
	format "____CreatedStatic____\n"
	close file
	)
	
fn createRenderableObjects = (
	local file=createfile (GetLevelName()+"renderable_objects.xml" )
	local instances = GetInstances()
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<renderable_objects>\n" to:file
	for i=1 to instances.count do(
		format "\t<mesh_instance name=\"%\" core_name=\"%\" pos=\"%\" yaw=\"%\" pitch=\"0.0\" roll=\"0.0\" visible=\"%\">\n" instances[i].name instances[i].name TranslationToRH(instances[i].transform.translation) GetYaw(instances[i].transform) instances[i].visible to:file
		)

	format "</renderable_objects>\n" to:file 
	format "____CreatedRenderableObjects____\n"
	close file
)	

fn createMaterials = (
	local file=createfile (GetLevelName()+"materials.xml" )
	local cores = GetCores()
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<materials>\n" to:file
	for i=1 to cores.count do(
		if classoff cores[i].material == Standardmaterial then (	
			format "\t<material name=\"%\" filename=\"%\ diffuseMap=\"%\">\n" cores[i].material.name cores[i].material.name cores[i].material.diffuseMap.filename to:file		
		) else (
			for j = 1 to cores[i].material.numsubs do (
				format "\t<material name=\"%\" filename=\"%\ diffuseMap=\"%\">\n" cores[i].material[j].name cores[i].material[j].namecores[i].material[j].diffuseMap.filename to:file		
			)
		)
	)
	
	format "</materials>\n" to:file 
	format "____CreatedMaterials____\n"
	close file
)

fn createMaterials = (
	local file=createfile (GetLevelName()+"materials.xml" )
	
	local cores = GetCores()
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<materials>\n" to:file
	for i=1 to cores.count do( --undefined
		if classof cores[i].material == Standardmaterial then (				
			format "\t<material name=\"%\" effect_technique=\"diffuse_technique\" >\n" cores[i].material.name (GetLevelName()+cores[i].material.name) to:file		
			if cores[i].material.diffuseMap != undefined and cores[i].material.diffuseMap.bitmap != undefined then ( --TODO: comprobar difuso no bitmap??
				format "\t\t <texture filename=\"%\"/>\n" cores[i].material.diffuseMap.filename to:file
			)else(
				messageBox ("¡¡¡El material "+cores[i].material.name+" de la malla "+cores[i].name+" tiene las texturas mal!!!") title:"MENSAJE PARA ARTISTAS" 
			)
			format "\t</material>\n" to:file 
		) else (
			for j = 1 to cores[i].material.numsubs do (
				--Si ya existe no exportamos
				format "\t<material name=\"%\" effect_technique=\"diffuse_technique\" >\n" cores[i].material[j].name (GetLevelName()+cores[i].material[j].name) to:file	
				if cores[i].material[j].diffuseMap != undefined and cores[i].material[j].diffuseMap.bitmap != undefined then ( --TODO: comprobar difuso no bitmap??
					format "\t\t <texture filename=\"%\"/>\n" cores[i].material[j].diffuseMap.filename to:file
				)else(
					messageBox ("¡¡¡El material "+cores[i].material.name+" de la malla "+cores[i].name+" tiene las texturas mal!!!") title:"MENSAJE PARA ARTISTAS" 
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
	
	return #(#(Vtx1,Normal1,TUVMap1), #(Vtx2,Normal2,TUVMap2), #(Vtx3,Normal3,TUVMap3))

)

fn createMesh obj= (
	format "____Creating Mesh %____\n" obj.name
	
	file = fopen (GetLevelName()+obj.name+".mesh" ) "wb"
	
	--Header
	writeshort file 0xFE55
	local numMats
	
	--Materials
	if (classOf obj.material) == Standardmaterial then (
		numMats=1
		writeshort file numMats
		writeshort file obj.material.name.count
		writestring file obj.material.name
	) else (
		numMats = obj.material.numsubs
		writeshort file numMats
		for mats=1 to numMats do (			
			writeshort file obj.material[mats].name.count
			writestring file obj.material[mats].name
		)
	)
	
	--Vertex

	
	--TODO: Evitar duplicados
	for i=1 to numMats do (
		local l_VtxsMap = dotnetobject "System.Collections.Generic.Dictionary`2[System.String,System.Int16]"
		local idxs = #()
		local vtxs = #()
		
		writeshort file 37 --vertexType
		--writeshort file (getNumVerts obj) --TODO: coger vertices afectados por ese mat...
		for idFace = 1 to (getNumFaces obj) do (
			local faceVtxs = createVertexesForFace obj idFace
			for numVtx = 1 to 3 do (
				local idx = DotNetAddVertex vtxs l_VtxsMap faceVtxs[numVtx]
				append idxs idx
			)

		)
		
		
		writeshort file vtxs.count
		
		for idVtx = 1 to vtxs.count do (
			local vtx = vtxs[idVtx]
			writefloat file vtx[1].x
			writefloat file vtx[1].y
			writefloat file vtx[1].z
			
			writefloat file vtx[2].x
			writefloat file vtx[2].y
			writefloat file vtx[2].z
			
			writefloat file vtx[3].x
			writefloat file vtx[3].y
		)
		
		-- Write the index byte width
		writeshort file 16
		
		writeshort file idxs.count
		
		for idx = 1 to idxs.count do (
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
	for i=1 to cores.count do(
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
	
	
--Init()
--createStaticMeshes()
--createRenderableObjects()
--createMaterials()
createMeshes()

