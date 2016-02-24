fileIn "exporter.ms"
fileIn "uab-property-manager.ms"

utility UABUtility "UAB Utility" width:160 height:527
(
	button SetPathButton "Set Export Path" pos:[10,41] width:138 height:25
	label PathLabel "Label" pos:[8,68] width:140 height:19
	checkbox ExportMeshCheck "Also export all .mesh" pos:[11,142] width:138 height:17 checked:true
	checkbox CopyTexturesCheck "Copy texture files" pos:[11,250] width:138 height:17 checked:true
	GroupBox grp1 "Export Mesh Objects" pos:[5,93] width:150 height:135
	button ExportRenderableObjectsBtn "Renderable Objects File" pos:[11,195] width:138 height:25
	button LightsButton "Lights" pos:[11,323] width:138 height:25
	button ExportMeshButton "Selected Mesh" pos:[11,110] width:138 height:25
	button ExportMaterialButton "Export All Materials" pos:[11,270] width:138 height:25
	button CameraButton "Camera" pos:[11,414] width:138 height:25
	button StaticMeshManagerButton "Static Meshes File" pos:[11,162] width:138 height:25
	button SceneButton "Everything in the Scene" pos:[11,357] width:138 height:25
	button btn7 "Button" pos:[48,325] width:0 height:0
	button PropsBtn "Properties Window" pos:[10,7] width:138 height:25
	GroupBox grp2 "Export Materials" pos:[5,233] width:150 height:69
	GroupBox grp3 "Other Elements" pos:[5,308] width:150 height:84
	GroupBox grp4 "Animations" pos:[5,398] width:150 height:111
	button exportObjAnimBtn "Object Cinematics" pos:[11,446] width:138 height:25
	
	
	
	on UABUtility open do
	(
		setAbsolutePath maxFilePath
		PathLabel.text = getAbsolutePath("")
	)
	on SetPathButton pressed do
	(
		local data_path=getSavePath caption:"Data path" initialDir:(getAbsolutePath "")
		if data_path != undefined then
		(
			setAbsolutePath data_path + "\\"
			PathLabel.text = getAbsolutePath("")
		)
	)
	on ExportRenderableObjectsBtn pressed do
	(
		createRenderableObjectsFile()
	)
	on LightsButton pressed do
	(
		createLights()
		messageBox "Fichero Lights.xml creado correctamente" title:"Creación correcta"
		
	)
	on ExportMeshButton pressed do
	(
		if $ != undefined then
		(
			createMeshes($selection)
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
	on CameraButton pressed do
	(
		if $ != undefined and $ != $selection then
		(
			if superclassOf $ == camera then
			(
				ExportCameraAnimation $
			)
			else
			(
				messageBox "El objeto seleccionado no es una cámara!" title: "Error"
			)
		)
		else
		(
			messageBox "No hay ninguna cámara seleccionada!" title: "Error"
		)
	)
	on StaticMeshManagerButton pressed do
	(
		createStaticMeshesFile( GetCores() )
		messageBox "Fichero static_meshes.xml creado correctamente" title:"Exportación correcta"
		
		if ExportMeshCheck.checked then
		(
			createMeshes( GetCores() )
			messageBox "Ficheros .mesh creados correctamente" title:"Exportación correcta"
		)
	)
	on SceneButton pressed do
	(
		local cores = GetCores()
		createStaticMeshesFile(cores)
		createRenderableObjectsFile()
		createMaterials CopyTexturesCheck.checked
		createMeshes(cores)
		
		messageBox "Escena exportada correctamente" title:"Exportación correcta"
	)
	on PropsBtn pressed do
	(
		try(DestroyDialog UABPropertyManager)catch()
		CreateDialog UABPropertyManager
	)
	on exportObjAnimBtn pressed do
	(
		ExportAllObjectCinematics()
	)
)
openUtility UABUtility
