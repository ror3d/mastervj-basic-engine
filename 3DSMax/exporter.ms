fileIn "export-lights.ms"
fileIn "export-materials.ms"
fileIn "export-meshes.ms"
fileIn "export-animations.ms"

fn Init = (
	format "____Init____\n"
)





fn TestExportAll = (
	createDirs()
	createStaticMeshes()
	createRenderableObjects()
	createLogic()
	createTriggersFile()
	createMaterials true
	createMeshes()
)

