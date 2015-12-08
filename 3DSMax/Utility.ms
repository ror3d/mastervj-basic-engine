fileIn "exporter.ms"

utility UABUtility "UAB Utility" width:162 height:360
(
	button SetPathButton "Set Path" pos:[8,8] width:144 height:24
	label PathLabel "Label" pos:[8,40] width:144 height:32
	checkbox ExportMeshCheck "Export mesh" pos:[8,96] width:144 height:24
	checkbox CopyTexturesCheck "Copy textures" pos:[8,120] width:144 height:24
	dropDownList LevelList "" pos:[8,144] width:144 height:21
	groupBox grp1 "Export" pos:[8,168] width:144 height:152
	button LightsButton "Lights" pos:[16,192] width:64 height:24
	button ExportMeshButton "Mesh" pos:[88,192] width:56 height:24
	button ExportMaterialButton "Material" pos:[88,224] width:56 height:24
	button CameraButton "Camera" pos:[16,224] width:64 height:24
	button StaticMeshManagerButton "Static mesh manager" pos:[17,256] width:128 height:24
	button SceneButton "Scene" pos:[16,288] width:128 height:24
	button btn7 "Button" pos:[48,336] width:0 height:0
	
	-- Inicializador de la utility
	on UABUtility open do
	(
		PathLabel.text = "C:\\Game\\Data\\"
		path = PathLabel.text
		local levels=#("level 1","level 2", "level 3","level 4", "main menu", "select player")
		LevelList.items=levels
		fn GetLevelName =
		(
			return PathLabel.text
		)
	)
	
	-- Set del path donde esta el juego
	on SetPathButton pressed do
	(
		local data_path=getSavePath caption:"Data path" initialDir:PathLabel.text
		if data_path != undefined then
			PathLabel.text = data_path + "\\"
			path = PathLabel.text
		)
	
	-- Crear static_meshes.xml
	on StaticMeshManagerButton pressed do
	(
		createStaticMeshes()
		messageBox "Fichero static_meshes.xml creado correctamente" title:"Exportación correcta"
		
		if ExportMeshCheck.checked then
		(
			createMeshes()
			messageBox "Ficheros .mesh creados correctamente" title:"Exportación correcta"
		)
	)
	
	on ExportMeshButton pressed do
	(
		if $ != undefined then
		(
			for i = 1 to $selection.count do
			(
				createMesh $selection[i]
			)
			messageBox "Fichero .mesh creado correctamente" title:"Exportación correcta"
		)
		else
		(
			messageBox "No hay ningún objeto seleccionado" title:"Seleccionar objeto"
		)
	)
	
	on ExportMaterialButton pressed do
	(
			createMaterials CopyTexturesCheck.checked
			messageBox "Fichero Materials.xml creado correctamente" title:"Creación correcta"
		
	)
	
	on SceneButton pressed do
	(
		createStaticMeshes()
		createRenderableObjects()
		createMaterials CopyTexturesCheck.checked
		createMeshes()
		
		messageBox "Escena exportada correctamente" tittle:"Exportación correcta"
	)
)
