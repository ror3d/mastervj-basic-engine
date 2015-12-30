fileIn "file-utils.ms"
fileIn "vertex-types.ms"
fileIn "export-scene-utils.ms"

fn exportSingleTexture mat meshName = (
	if HasTexture mat then --TODO: comprobar difuso no bitmap??
	(
		textureName = filenameFromPath mat.diffuseMap.filename
		format "\t\t <texture filename=\"%\"/>\n" (getTexturePath textureName) to:file
		format "Copying texture from % to output dir %\n" mat.diffuseMap.filename (getAbsTexturePath textureName)
		copyFile mat.diffuseMap.filename (getAbsTexturePath textureName)
	)
	else if mat.diffuseMap != undefined then
	(
		err = "En el material " + mat.name + " de la malla " + meshName + " no hay textura difusa o no es un bitmap!!!"
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
		-- TODO: Export only once per material!!!
		if classof cores[i].material == Standardmaterial then
		(
			format "\t<material name=\"%\" effect_technique=\"diffuse_technique\" >\n" cores[i].material.name to:file
			
			exportSingleTexture cores[i].material cores[i].name
			
			if HasLightmap cores[i].material alertOnMissing:true then
			(
				lightMapName = fileNameFromPath cores[i].material.selfIllumMap.filename
				format "\t\t <lightmap filename=\"%\"/>\n" (getTexturePath lightMapName) to:file
				copyFile cores[i].material.selfIllumMap.filename (getAbsTexturePath textureName)
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
