fileIn "exporter.ms"
fileIn "uab-property-manager.ms"
fileIn "export-scene-editor.ms"

utility UABUtility "UAB Utility" width:160 height:552
(
	button SetPathButton "Set Export Path" pos:[10,41] width:138 height:25
	label PathLabel "Label" pos:[8,68] width:140 height:19
	checkbox ExportMeshCheck "Also export all .mesh" pos:[11,232] width:138 height:17 checked:true
	checkbox CopyTexturesCheck "Copy texture files" pos:[11,306] width:138 height:17 checked:true
	GroupBox grp1 "Export Mesh Objects" pos:[5,184] width:150 height:100
	button ExportSceneBtn "Scene File" pos:[10,144] width:138 height:25
	button LightsButton "Lights" pos:[11,379] width:138 height:25
	button ExportMeshButton "Selected Mesh" pos:[11,200] width:138 height:25
	button ExportMaterialButton "Export All Materials" pos:[11,326] width:138 height:25
	button CameraButton "Camera" pos:[11,470] width:138 height:25
	button StaticMeshManagerButton "Static Meshes File" pos:[11,252] width:138 height:25
	button SceneButton "Everything in the Scene" pos:[11,413] width:138 height:25 enabled:false
	button btn7 "Button" pos:[48,381] width:0 height:0
	button PropsBtn "Properties Window" pos:[10,7] width:138 height:25
	GroupBox grp2 "Export Materials" pos:[5,289] width:150 height:69
	GroupBox grp3 "Other Elements" pos:[5,364] width:150 height:84
	GroupBox grp4 "Animations" pos:[5,454] width:150 height:82
	button exportObjAnimBtn "Object Cinematics" pos:[11,502] width:138 height:25
	groupBox grp5 "Export Scene" pos:[5,88] width:150 height:88
	editText SceneNameTxt "" pos:[10,120] width:138 height:16
	label lbl2 "Scene Name" pos:[16,104] width:88 height:16
	
	
	
	on UABUtility open do
	(
		setAbsolutePath maxFilePath
		PathLabel.text = getAbsolutePath("")
		
		SceneNameTxt.text = (substring maxFileName 1 (maxFileName.count - 4))
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
	on ExportSceneBtn pressed do
	(
		if SceneNameTxt.text == "" then (
			messageBox "La escena requiere un nombre!" title:"Error"
			return Ok
		)
		CollectCoresAndInstances()
		--createRenderableObjectsFile(GetInstances())
		--createLogic(GetInstances())
		--createTriggersFile(GetInstances())
		createSceneFile (SceneNameTxt.text) (GetGameObjects())
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
			CollectCoresAndInstances()
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
		CollectCoresAndInstances()
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
		CollectCoresAndInstances()
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
		CollectCoresAndInstances()
		local cores = GetCores()
		createStaticMeshesFile(cores)
		createMeshes(cores)
		--createRenderableObjectsFile()
		--createLogic()
		--createMaterials CopyTexturesCheck.checked
		
		messageBox "Escena exportada correctamente" title:"Exportación correcta"
	)
	on PropsBtn pressed do
	(
		--try(DestroyDialog UABPropertyManager)catch()
		--CreateDialog UABPropertyManager
		try(DestroyDialog ExportSceneEditor)catch()
		CreateDialog ExportSceneEditor
	)
	on exportObjAnimBtn pressed do
	(
		ExportAllObjectCinematics()
	)
)
openUtility UABUtility