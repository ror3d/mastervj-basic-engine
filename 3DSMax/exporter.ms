fileIn "export-lights.ms"
fileIn "export-materials.ms"
fileIn "export-meshes.ms"

fn Init = (
	format "____Init____\n"
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

fn TestExportAll = (
	createDirs()
	createStaticMeshes()
	createRenderableObjects()
	createMaterials true
	createMeshes()
)

