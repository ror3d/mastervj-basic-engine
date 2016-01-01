fileIn "file-utils.ms"
fileIn "vertex-types.ms"
fileIn "export-scene-utils.ms"

shouldCopyTextures = true

fn exportTextureFile texTypeName texMap = (
	local textureName = filenameFromPath texMap.filename
	format "\t\t <% filename=\"%\"/>\n" texTypeName (getTexturePath textureName) to:file
	if shouldCopyTextures then
	(
		format "Copying % from % to output dir %\n" textTypeName texMap.filename (getAbsTexturePath textureName)
		copyFile texMap.filename (getAbsTexturePath textureName)
	)
)

fn exportSingleTexture mat meshName = (
	if HasTexture mat alertOnMissing:true then
	(
		exportTextureFile "texture" mat.diffuseMap
	)
)

fn exportSingleLightmap mat meshName = (
	if HasLightmap mat alertOnMissing:true then
	(
		exportTextureFile "lightmap" mat.selfIllumMap
	)
)

fn createSingleMaterial obj mat = (
	
	-- TODO cambiar technique dependiendo del tipo de material
	
	format "\t<material name=\"%\" effect_technique=\"diffuse_technique\" >\n" mat.name to:file	

	exportSingleTexture mat obj.name
	
	exportSingleLightmap mat obj.name
	
	format "\t</material>\n" to:file 
)

fn matExistsElseAdd matMap mat = (
	if matMap.ContainsKey (mat.name) then
	(
		return true
	)
	else
	(
		matMap.Add mat.name 0
		return false
	)
)

fn createMaterials copyTextures = (
	MyCreateFile "materials.xml"
	
	local cores = GetCores()
	
	WriteXMLPrologToFile()
	format "<materials>\n" to:file
	
	local l_MatsMap = dotnetobject "System.Collections.Generic.Dictionary`2[System.String,System.Int16]"
	
	shouldCopyTextures = copyTextures
	
	for i=1 to cores.count do
	(
		if classof cores[i].material == Standardmaterial then
		(
			if not matExistsElseAdd l_MatsMap cores[i].material then
			(
				createSingleMaterial cores[i] cores[i].material
			)
		)
		else -- Si se trata de un multimaterial
		(
			for j = 1 to cores[i].material.numsubs do
			(
				--Si ya existe no exportamos
				if not matExistsElseAdd l_MatsMap cores[i].material[j] then
				(
					createSingleMaterial cores[i] cores[i].material[j]
				)
			)
		)
	)
	
	format "</materials>\n" to:file 
	format "____CreatedMaterials____\n"
	
	MyCloseFile()
)
