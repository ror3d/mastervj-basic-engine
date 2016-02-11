fileIn "file-utils.ms"
fileIn "vertex-types.ms"
fileIn "export-scene-utils.ms"

fn createStaticMeshesFile = (
	MyCreateFile "static_meshes.xml"
	
	local cores = GetCores()
	
	WriteXMLPrologToFile()
	format "<static_meshes>\n" to:file
	
	for i=1 to cores.count do
	(
		format "\t<static_mesh name=\"%\" filename=\"%.mesh\"/>\n" cores[i].name (getMeshPath cores[i].name) to:file
	)
	
	format "</static_meshes>\n" to:file 
	format "____CreatedStatic____\n"
	
	MyCloseFile()
)
	
fn createRenderableObjectsFile = (
	MyCreateFile "renderable_objects.xml"
	
	local instances = GetInstances()
	
	WriteXMLPrologToFile()
	format "<renderable_objects>\n" to:file
	
	for i = 1 to instances.count do
	(
		Pos = TranslationToRH (instances[i].transform.translation)
		Yaw = GetYaw (instances[i].transform)
		Pitch = GetPitch (instances[i].transform)
		Roll = GetRoll (instances[i].transform)
		local core = getUserProp $*[i] "render_export_rend_obj"
		format "\t<mesh_instance name=\"%\" core_name=\"%\" pos=\"% % %\" yaw=\"%\" pitch=\"%\" roll=\"%\" visible=\"%\"/>\n"\
					instances[i].name core Pos[1] Pos[2] Pos[3] Yaw Pitch Roll instances[i].visibility to:file
	)

	format "</renderable_objects>\n" to:file 
	format "____CreatedRenderableObjects____\n"
	
	MyCloseFile()
)

---------------------------------------------------------------------
-- NORMALS
---------------------------------------------------------------------
global UABVtxsNormals=#()
function UABClearNormalsArray =
(
	while UABVtxsNormals.count>0 do
	(
		deleteItem UABVtxsNormals 1
	)
)

function IsSmoothingGroupEnabled IdGroup Value =
(
	local ValueMask=2^(IdGroup-1)
	return (bit.and Value ValueMask)==ValueMask
)
function UABGetVertexNormal obj IdVertex SmoothValue FaceNormal =
(
	local HasNormal=false
	local Normal=point3 0 0 0
	for b=1 to 32 do
	(
		if((IsSmoothingGroupEnabled b SmoothValue)==true) then
		(
			Normal=Normal+UABVtxsNormals[IdVertex][b]
			HasNormal=true
		)
	)
	if HasNormal==false then
	(
		Normal=FaceNormal
	)
	return Normal
)
function UABCalcVertexsNormals obj =
(
	UABClearNormalsArray()
	local NumVtxs=getNumVerts obj
	for b=1 to NumVtxs do
	(
		UABVtxsNormals[b]=#()
		for t=1 to 32 do
		(
			UABVtxsNormals[b][t]=point3 0 0 0
		)
	)
	local NumFaces=getNumFaces obj
	local InvTransform=inverse obj.transform
	for IdFace=1 to NumFaces do
	(
		local IdxsFace=getFace obj IdFace
		local Vtx1=(getVert obj IdxsFace.x)*InvTransform
		local Vtx2=(getVert obj IdxsFace.y)*InvTransform
		local Vtx3=(getVert obj IdxsFace.z)*InvTransform
		local FaceNormal=getFaceNormal obj IdFace
		local SmoothValue=getFaceSmoothGroup obj IdFace
		for b=1 to 32 do
		(
			if((IsSmoothingGroupEnabled b SmoothValue)==true) then
			(
				UABVtxsNormals[IdxsFace.x][b]=UABVtxsNormals[IdxsFace.x][b] + FaceNormal
				UABVtxsNormals[IdxsFace.y][b]=UABVtxsNormals[IdxsFace.y][b] + FaceNormal
				UABVtxsNormals[IdxsFace.z][b]=UABVtxsNormals[IdxsFace.z][b] + FaceNormal
			)
		)
	)
)

---------------------------------------------------------------------
-- /NORMALS
---------------------------------------------------------------------



function ConvertVtx Vtx =
(
	local l_Text = stringstream ""
	format "%" Vtx to:l_Text
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

global CurrentObjInvMat = matrix3 1

fn createVertexesForFace obj IdFace = (
	local IdxsFace=getFace obj IdFace
	-- Para coger los v?rtices de la primera cara
	local Vtx1=getVert obj IdxsFace[1]
	local Vtx2=getVert obj IdxsFace[2]
	local Vtx3=getVert obj IdxsFace[3]
	
	-- TODO: Obtener normales correctas
	
	-- Para coger las normales de los v?rtices de la primera cara
	local FaceNormal = getFaceNormal obj IdFace -- El ?ndice de la cara
	local SmoothValue = getFaceSmoothGroup obj IdFace -- El ?ndice de la cara

	--local Normal1=getNormal obj IdxsFace[1]
	--local Normal2=getNormal obj IdxsFace[2]
	--local Normal3=getNormal obj IdxsFace[3]
	local Normal1=normalize (UABGetVertexNormal obj IdxsFace[1] SmoothValue FaceNormal)
	local Normal2=normalize (UABGetVertexNormal obj IdxsFace[2] SmoothValue FaceNormal)
	local Normal3=normalize (UABGetVertexNormal obj IdxsFace[3] SmoothValue FaceNormal)
	
	
	local v1 = vertex pos: (ChangeCoordsBase (Vtx1*CurrentObjInvMat) ) normal: (ChangeCoordsBase Normal1)
	local v2 = vertex pos: (ChangeCoordsBase (Vtx2*CurrentObjInvMat) ) normal: (ChangeCoordsBase Normal2)
	local v3 = vertex pos: (ChangeCoordsBase (Vtx3*CurrentObjInvMat) ) normal: (ChangeCoordsBase Normal3)
	
	local faceMat
	local nmats = 1
	if (classOf obj.material) == Standardmaterial then
	(
		faceMat = obj.material
	)
	else
	(
		nmats = obj.material.count
		local faceMatId = getFaceMatID obj IdFace
		faceMatId = int(mod (faceMatId-1) obj.material.count)+1
		faceMat = obj.material[faceMatId]
		if faceMatId != getFaceMatID obj IdFace then
		(
			--setFaceMatID obj IdFace faceMatId
		)
	)
	
	if faceMat == undefined then
	(
		format "$$$$ Alerta! El objeto % tiene caras con material incorrecto!!!\n" obj.name
		--setFaceMatID obj IdFace 1
		if (classOf obj.material) == Standardmaterial then
			faceMat = obj.material
		else
			faceMat = obj.material[1]
	)
	
	if HasTexture faceMat then -- map[0] is the color, map[1] is the default texture, so at least 1 map (color) is always there
	(
		-- Para coger los ?ndices de las coordenadas de textura de la primera cara, el 1 se?ala al primer
		--   conjunto de coordenadas aplicadas sobre la cara
		local IdxsMap=meshop.getMapFace obj 1 IdFace
		
		-- Para coger las coordenadas de los v?rtices de la primera cara
		local TUVMap1= meshop.getMapVert obj 1 IdxsMap[1]
		local TUVMap2= meshop.getMapVert obj 1 IdxsMap[2]
		local TUVMap3= meshop.getMapVert obj 1 IdxsMap[3]
		
		v1.uv = TUVMap1
		v2.uv = TUVMap2
		v3.uv = TUVMap3
		
	)
	
	if HasLightmap faceMat then
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
	
	if HasNormal faceMat then
	(		
		--Binormal & Tang x Face
		--Font: http://forums.cgsociety.org/archive/index.php?t-407814.html
		local dV1 = Vtx1 - Vtx2
		local dV2 = Vtx1 - Vtx3

		local dUV1 = TUVMap1 - TUVMap2
		local dUV2 = TUVMap1 - TUVMap3

		local area = dUV1.x * dUV2.y - dUV1.y * dUV2.x
		local sign = if area < 0 then -1 else 1

		v1.tangencial = [0,0,1]

		v1.tangencial.x = dV1.x * dUV2.y - dUV1.y * dV2.x
		v1.tangencial.y = dV1.y * dUV2.y - dUV1.y * dV2.y
		v1.tangencial.z = dV1.z * dUV2.y - dUV1.y * dV2.z

		v1.tangencial = (normalize v1.tangencial) * sign

		v1.binormal = (normalize (cross (normalize (getFaceNormal obj idFace)) v1.tangencial)) * sign

		v2.tangencial = v1.tangencial
		v3.tangencial = v1.tangencial
		v2.binormal = v1.binormal
		v3.binormal = v1.binormal		
	)
	
	return #(v3, v2, v1)

)

fn createMesh obj = (
	format "____Creating Mesh %____\n" obj.name
	
	MyFOpen (MeshSubPath + obj.name + ".mesh") "wb"
	
	-- Centrar al origen
	--obj.position = [0, 0, 0]
	--obj.rotation = quat 0 0 0 1
	--obj.scale = [1, 1, 1]
	
	CurrentObjInvMat = inverse obj.transform
	
	UABCalcVertexsNormals obj
	
	--Header
	writeshort file 0xFE55
	local numMats = 1
	
	if obj.material == undefined then
	(
		format "\n$$$$ Object % doesn't have a material!\n\n" obj.name
		messageBox ("Object "+obj.name+" doesn't have a material!") title: "Error!"
		MyCloseFile()
		return OK
	)
	
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
	
	for i=1 to numMats do
	(
		local l_VtxsMap = dotnetobject "System.Collections.Generic.Dictionary`2[System.String,System.Int16]"
		local idxs = #()
		local vtxs = #()
		
		local mat
		if (classOf obj.material) == Standardmaterial then
		(
			mat = obj.material
		)
		else
		(
			mat = obj.material[i]
		)
		
		writeshort file (GetVertexTypeFromObjectMaterial obj mat)
		
		--TODO: coger solo vertices afectados por ese mat...
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
			writefloat file vtx.pos.y
			writefloat file vtx.pos.z
			
			-- Normal
			writefloat file vtx.normal.x
			writefloat file vtx.normal.y
			writefloat file vtx.normal.z
			
			if HasNormal mat then
			(
				-- Binormal
				writefloat file 0
				writefloat file 0
				writefloat file 0
				writefloat file 0
				-- Tangencial
				writefloat file 0
				writefloat file 0
				writefloat file 0
				writefloat file 0
			)
			
			if HasTexture mat then
			(
				-- UV
				writefloat file vtx.uv.x
				writefloat file (1 - vtx.uv.y)
			)
			
			if HasLightmap mat then
			(
				--UV Lightmap
				writefloat file vtx.uv2.x
				writefloat file (1 - vtx.uv2.y)
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
	
	-- TODO: Export bounding box/sphere? look into NodeGetBoundingBox
	
	--Footer
	writeshort file 0x55FE
	
	MyCloseFile()
	
	format "____Created Mesh %____\n" obj.name
)



fn createMeshes = (
	local cores = GetCores()
	for i=1 to cores.count do
	(
		-- Clone mesh and use the clone
		if superclassof cores[i] != GeometryClass then
		(
			format "$$$$ % no es una malla!\n" cores[i].name
			continue
		)
		local clone = #()
		maxOps.cloneNodes cores[i] newNodes:&clone
		if clone.count > 1 then
		(
			delete clone
			format "$$$$ % no es un objeto adecuado!\n" cores[i]
			continue
		)
		local n = cores[i].name
		cores[i].name = "_" + n
		clone[1].name = n
		
		createMesh (convertToMesh clone[1])
		
		delete clone
		cores[i].name = n
	)
)
