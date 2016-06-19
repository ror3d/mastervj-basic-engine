global ExportSceneEditor
rollout ExportSceneEditor "Export Scene Editor" width:664 height:728
(
	label object_name_lbl "Label" pos:[80,8] width:272 height:16
	label lbl2 "Object Name:" pos:[8,8] width:72 height:16
	checkbox export_cbx "Export Object" pos:[16,40] width:120 height:16
	GroupBox grp1 "Components" pos:[16,64] width:632 height:464
	GroupBox grp2 "Renderable" pos:[24,80] width:200 height:216
	checkbox export_rend_ckb "Export Renderable" pos:[32,96] width:112 height:16
	checkbox static_mesh_ckb "Static Mesh" pos:[48,192] width:120 height:16 enabled:false
	checkbox skinned_mesh_ckb "Skinned Mesh" pos:[48,216] width:120 height:16 enabled:false
	MultiListBox layer_select "Layers" pos:[40,112] width:168 height:4 enabled:false items:#("solid", "models")
	edittext skinned_mesh_core_txt "" pos:[72,256] width:136 height:16 enabled:false
	label lbl3 "Model Name" pos:[72,240] width:104 height:16
	GroupBox grp3 "Collider" pos:[232,80] width:200 height:216
	checkbox export_collider_ckb "Export Collider" pos:[240,96] width:152 height:16
	dropdownList collider_type_select "Collider Type" pos:[248,152] width:152 height:40 enabled:false items:#("Triangle Mesh", "Convex Mesh", "Bounding Box", "Bounding Sphere")
	checkbox collider_static_ckb "Static" pos:[256,112] width:144 height:16 enabled:false
	checkbox collider_kinem_ckb "Kinematic" pos:[256,128] width:144 height:16 enabled:false
	radiobuttons collider_mesh_src_radio "" pos:[264,200] width:96 height:32 enabled:false labels:#("Use Own Mesh", "Specify Mesh") default:1 columns:1
	edittext collider_mesh_txt "" pos:[272,235] width:136 height:16 enabled:false
	GroupBox grp4 "Trigger" pos:[440,80] width:200 height:240
	dropdownList trigger_type_select "Collider Type" pos:[464,136] width:152 height:40 enabled:false items:#("Bounding Box", "Bounding Sphere", "Triangle Mesh", "Convex Mesh")
	checkbox trigger_static_ckb "Static" pos:[464,112] width:144 height:16 enabled:false
	radiobuttons trigger_mesh_src_radio "" pos:[472,176] width:96 height:32 enabled:false labels:#("Use Own Mesh", "Specify Mesh") columns:1
	edittext trigger_mesh_txt "" pos:[480,211] width:136 height:16 enabled:false
	label lbl12 "Scale" pos:[456,240] width:80 height:16
	edittext trigger_scale_x_txt "x" pos:[464,256] width:48 height:16 enabled:false
	edittext trigger_scale_y_txt "y" pos:[520,256] width:48 height:16 enabled:false
	edittext trigger_scale_z_txt "z" pos:[576,256] width:48 height:16 enabled:false
	label lbl13 "Offset" pos:[456,280] width:80 height:16
	edittext trigger_offset_x_txt "x" pos:[464,296] width:48 height:16 enabled:false
	edittext trigger_offset_y_txt "y" pos:[520,296] width:48 height:16 enabled:false
	edittext trigger_offset_z_txt "z" pos:[576,296] width:48 height:16 enabled:false
	GroupBox grp21 "Scripts" pos:[24,304] width:200 height:216
	GroupBox grp22 "Camera" pos:[232,304] width:200 height:80
	GroupBox grp23 "Character Controller" pos:[232,392] width:200 height:128
	GroupBox grp24 "Particle Emitter" pos:[440,328] width:200 height:88
	label lbl34 "Offset" pos:[248,336] width:80 height:16
	edittext cam_offset_x_txt "x" pos:[256,352] width:48 height:16 enabled:false
	edittext cam_offset_y_txt "y" pos:[312,352] width:48 height:16 enabled:false
	edittext cam_offset_z_txt "z" pos:[368,352] width:48 height:16 enabled:false
	checkbox export_trigger_ckb "Export Trigger" pos:[448,95] width:152 height:16
	checkbox export_camera_ckb "Export Camera" pos:[240,320] width:152 height:16
	label lbl35 "Offset" pos:[248,480] width:80 height:16
	edittext cc_offset_x_txt "x" pos:[256,496] width:48 height:16 enabled:false
	edittext cc_offset_y_txt "y" pos:[312,496] width:48 height:16 enabled:false
	edittext cc_offset_z_txt "z" pos:[368,496] width:48 height:16 enabled:false
	checkbox export_cc_ckb "Export Character Controller" pos:[240,408] width:152 height:16
	edittext cc_radius_txt "Radius" pos:[248,432] width:144 height:16 enabled:false
	edittext cc_height_txt "Height" pos:[248,456] width:144 height:16 enabled:false
	combobox scripts_combo "" pos:[32,320] width:184 height:8
	button scripts_edit_btn "Edit Script Properties" pos:[32,488] width:184 height:24
	GroupBox grp33 "Game Object" pos:[8,24] width:648 height:512
	GroupBox grp34 "Other Exports" pos:[8,536] width:648 height:136
	GroupBox grp35 "Light" pos:[16,552] width:200 height:112
	GroupBox grp36 "Transform Cinematic" pos:[222,552] width:200 height:112
	button get_btn "Get" pos:[16,680] width:136 height:40
	button set_btn "Set" pos:[160,680] width:136 height:40
	checkbox autoload_ckb "Load automatically when selection changes" pos:[312,680] width:232 height:16
	checkbox export_light_ckb "Export As Light" pos:[24,568] width:176 height:16
	checkbox export_cinem_ckb "Export Transform Anim." pos:[232,568] width:152 height:16
	radiobuttons cinematic_type_radio "" pos:[248,592] width:68 height:48 enabled:false labels:#("Cycle", "Reverse", "One-Shot") columns:1
	checkbox export_particle_ckb "Export Emitter" pos:[448,344] width:152 height:16
	label lbl43 "Emitter Class" pos:[464,368] width:112 height:16
	edittext emitter_class_txt "" pos:[464,384] width:144 height:16
	GroupBox grp49 "Audio Speaker" pos:[440,424] width:200 height:96
	checkbox export_speaker_ckb "Export Speaker" pos:[448,440] width:152 height:16
	spinner speaker_volume_txt "Volume" pos:[489,464] width:95 height:16 enabled:false range:[0,100,100] type:#integer
	button script_add_btn "Add Script" pos:[32,456] width:184 height:21
	
	
	
	local m_ObjSelected = undefined
	
	function UpdateEnabledProps = (
		local expGO = export_cbx.checked
		local eRend = false
		local eColl = false
		local eTrig = false
		local eCam = false
		local eCC = false
		local eSpeak = false
		local ePart = false
		if expGO then (
			export_rend_ckb.enabled = true
			export_collider_ckb.enabled = true
			export_trigger_ckb.enabled = true
			export_camera_ckb.enabled = true
			export_cc_ckb.enabled = true
			export_speaker_ckb.enabled = true
			export_particle_ckb.enabled = true
			scripts_combo.enabled = true
			script_add_btn.enabled = true
			scripts_edit_btn.enabled = true
			
			eRend = export_rend_ckb.checked
			eColl = export_collider_ckb.checked
			eTrig = export_trigger_ckb.checked
			eCam = export_camera_ckb.checked
			eCC = export_cc_ckb.checked
			eSpeak = export_speaker_ckb.checked
			ePart = export_particle_ckb.checked
		) else (
			export_rend_ckb.enabled = false
			export_collider_ckb.enabled = false
			export_trigger_ckb.enabled = false
			export_camera_ckb.enabled = false
			export_cc_ckb.enabled = false
			export_speaker_ckb.enabled = false
			export_particle_ckb.enabled = false
			scripts_combo.enabled = false
			script_add_btn.enabled = false
			scripts_edit_btn.enabled = false
		)
		static_mesh_ckb.enabled = eRend
		skinned_mesh_ckb.enabled = eRend
		layer_select.enabled = eRend
		skinned_mesh_core_txt.enabled = (eRend and skinned_mesh_ckb.checked)
		
		collider_type_select.enabled = eColl
		collider_static_ckb.enabled = eColl
		collider_kinem_ckb.enabled = eColl
		collider_mesh_src_radio.enabled = eColl
		collider_mesh_txt.enabled = eColl -- and collider_mesh_src_radio.enabled and collider_mesh_src_radio.state[0] == 2
		
		trigger_type_select.enabled = eTrig
		trigger_static_ckb.enabled = eTrig
		trigger_mesh_src_radio.enabled = eTrig
		trigger_mesh_txt.enabled = eTrig
		trigger_scale_x_txt.enabled = eTrig
		trigger_scale_y_txt.enabled = eTrig
		trigger_scale_z_txt.enabled = eTrig
		trigger_offset_x_txt.enabled = eTrig
		trigger_offset_y_txt.enabled = eTrig
		trigger_offset_z_txt.enabled = eTrig
		
		cam_offset_x_txt.enabled = eCam
		cam_offset_y_txt.enabled = eCam
		cam_offset_z_txt.enabled = eCam
		
		cc_offset_x_txt.enabled = eCC
		cc_offset_y_txt.enabled = eCC
		cc_offset_z_txt.enabled = eCC
		cc_radius_txt.enabled = eCC
		cc_height_txt.enabled = eCC
		
		speaker_volume_txt.enabled = eSpeak
		
		emitter_class_txt.enabled = ePart
		
		cinematic_type_radio.enabled = export_cinem_ckb.checked
	)
	
	function ResetProps = (
		export_cbx.checked = false
		
		export_rend_ckb.checked = false
		static_mesh_ckb.checked = true
		skinned_mesh_ckb.checked = false
		layer_select.selection = #{}
		skinned_mesh_core_txt.text = ""
		
		export_collider_ckb.checked = false
		collider_type_select.selection = 1
		collider_static_ckb.checked = false
		collider_kinem_ckb.checked = false
		collider_mesh_src_radio
		collider_mesh_txt.text = ""
		
		export_trigger_ckb.checked = false
		trigger_type_select.selection = 1
		trigger_static_ckb.checked = false
		trigger_mesh_src_radio.state = 1
		trigger_mesh_txt.text = ""

		trigger_scale_x_txt.text = "1"
		trigger_scale_y_txt.text = "1"
		trigger_scale_z_txt.text = "1"

		trigger_offset_x_txt.text = "0"
		trigger_offset_y_txt.text = "0"
		trigger_offset_z_txt.text = "0"


		export_camera_ckb.checked = false
		cam_offset_x_txt.text = "0"
		cam_offset_y_txt.text = "0"
		cam_offset_z_txt.text = "0"


		export_cc_ckb.checked = false
		cc_offset_x_txt.text = "0"
		cc_offset_y_txt.text = "0"
		cc_offset_z_txt.text = "0"
		cc_radius_txt.text = "0.5"
		cc_height_txt.text = "1"


		export_speaker_ckb.checked = false
		speaker_volume_txt.value = 100


		export_particle_ckb.checked = false
		emitter_class_txt.text = ""


		scripts_combo.text = ""
		scripts_combo.items = #()
		scripts_combo.selection = 0


		export_light_ckb.checked = false


		export_cinem_ckb.checked = false
		cinematic_type_radio.state = 1
	)
	
	
	fn LoadCkbProp ckb propName = (
		local val = getUserProp m_ObjSelected propName
		ckb.checked = (val != undefined)
	)
	
	fn SaveCkbProp obj ckb propName = (
		if ckb.checked then
		(
			setUserProp obj propName "true"
		)
		else
		(
			deleteUserProp obj propName
		)
	)
	
	fn LoadTxtProp txt propName = (
		local val = getUserProp m_ObjSelected propName
		if val != undefined then
		(
			txt.text = val
		)
		else
		(
			txt.text = ""
		)
	)
	
	fn SaveTxtProp obj txt propName = (
		if txt.text != "" then
		(
			setUserProp obj propName txt.text
		)
		else
		(
			deleteUserProp obj propName
		)
	)
	
	fn LoadDDLProp ddl propName = (
		local val = getUserProp m_ObjSelected propName
		if val != undefined then
		(
			ddl.selection = findItem ddl.items val
		)
	)
	
	fn SaveDDLProp obj ddl propName = (
		if ddl.selected != undefined then
		(
			setUserProp obj propName ddl.selected
		)
		else
		(
			deleteUserProp obj propName
		)
	)
	
	fn LoadRadioProp radio propName = (
		local val = getUserProp m_ObjSelected propName
		if val != undefined then
		(
			radio.state = findItem radio.labels val
		)
		else
		(
			radio.state = 0
		)
	)
	
	fn SaveRadioProp obj radio propName = (
		if radio.state != 0 then
		(
			setUserProp obj propName radio.labels[radio.state]
		)
		else
		(
			deleteUserProp obj propName
		)
	)
	
	
	fn LoadObjectProps =
	(
		ResetProps()
		
		if $ == undefined or $ == $selection then
		(
			return false
		)
		
		m_ObjSelected = $
		ExportSceneEditor.title = m_ObjSelected.name
		object_name_lbl.caption = m_ObjSelected.name
		
		
		LoadCkbProp export_cbx "ExportGameObject"
		
		LoadCkbProp export_rend_ckb "ExportRendererC"
		LoadCkbProp static_mesh_ckb "RendererStaticMesh"
		skinned_mesh_ckb.checked = not static_mesh_ckb.checked
		
		local rLayer = getUserProp m_ObjSelected "RendererLayer"
		if rLayer != undefined then (
			local rLayersN = filterString rLayer ","
			local rLayers = #()
			for i = 1 to layer_select.items.count do
			(
				if (findItem rLayersN layer_select.items[i]) != 0 then
				(
					append rLayers i
				)
			)
			layer_select.selection = rLayers
		) else (
			layer_select.selection = #{}
		)
		
		LoadTxtProp skinned_mesh_core_txt "SkinnedMeshCore"
		
		
		LoadCkbProp export_collider_ckb "ExportColliderC"
		LoadDDLProp collider_type_select "ColliderCType"
		LoadCkbProp collider_static_ckb "ColliderCStatic"
		LoadCkbProp collider_kinem_ckb "ColliderCKinematic"
		LoadRadioProp collider_mesh_src_radio "ColliderCMeshSource"
		LoadTxtProp collider_mesh_txt "ColliderCMeshSrcName"
		
		
		LoadCkbProp export_trigger_ckb "ExportTriggerC"
		LoadDDLProp trigger_type_select "TriggerCType"
		LoadCkbProp trigger_static_ckb "TriggerCStatic"
		LoadRadioProp trigger_mesh_src_radio "TriggerCMeshSource"
		LoadTxtProp trigger_mesh_txt "TriggerCMeshSrcName"
		
		LoadTxtProp trigger_scale_x_txt  "TriggerCScaleX"
		LoadTxtProp trigger_scale_y_txt  "TriggerCScaleY"
		LoadTxtProp trigger_scale_z_txt  "TriggerCScaleZ"
		LoadTxtProp trigger_offset_x_txt "TriggerCOffsetX"
		LoadTxtProp trigger_offset_y_txt "TriggerCOffsetY"
		LoadTxtProp trigger_offset_z_txt "TriggerCOffsetZ"
		
		
		LoadCkbProp export_camera_ckb "ExportCameraC"
		LoadTxtProp cam_offset_x_txt "CameraCOffsetX"
		LoadTxtProp cam_offset_y_txt "CameraCOffsetY"
		LoadTxtProp cam_offset_z_txt "CameraCOffsetZ"
		
		LoadCkbProp export_cc_ckb "ExportCharContrC"
		LoadTxtProp cc_offset_x_txt "CharContrCOffsetX"
		LoadTxtProp cc_offset_y_txt "CharContrCOffsetY"
		LoadTxtProp cc_offset_z_txt "CharContrCOffsetZ"
		LoadTxtProp cc_radius_txt "CharContrCRadius"
		LoadTxtProp cc_height_txt "CharContrCHeight"
		
		
		LoadCkbProp export_speaker_ckb "ExportSpeakerC"
		local vol = getUserProp m_ObjSelected "SpeakerCVolume"
		if vol != undefined then (
			speaker_volume_txt.value = vol
		) else (
			speaker_volume_txt.value = 100
		)
		
		
		LoadCkbProp export_particle_ckb "ExportParticleSC"
		LoadTxtProp emitter_class_txt "ParticleSCClass"
		
		-- TODO: Scripts
		
		LoadCkbProp export_light_ckb "ExportAsLight"
		
		LoadCkbProp export_cinem_ckb "ExportTAnimation"
		LoadRadioProp cinematic_type_radio "TAnimationType"
		
		UpdateEnabledProps()
	)
	
	function SaveObjectProps Obj =
	(
		SaveCkbProp Obj export_cbx "ExportGameObject"
		
		SaveCkbProp Obj export_rend_ckb "ExportRendererC"
		SaveCkbProp Obj static_mesh_ckb "RendererStaticMesh"
		local layers = ""
		for i = 1 to layer_select.items.count do (
			if layer_select.selection[i] then (
				layers = layers + layer_select.items[i] + ","
			)
		)
		if layers != "" then (
			layers = substring layers 1 (layers.count-1)
			setUserProp Obj "RendererLayer" layers
		) else (
			deleteUserProp obj "RendererLayer"
		)
		SaveTxtProp Obj skinned_mesh_core_txt "SkinnedMeshCore"
		
		SaveCkbProp Obj export_collider_ckb "ExportColliderC"
		SaveDDLProp Obj collider_type_select "ColliderCType"
		SaveCkbProp Obj collider_static_ckb "ColliderCStatic"
		SaveCkbProp Obj collider_kinem_ckb "ColliderCKinematic"
		SaveRadioProp Obj collider_mesh_src_radio "ColliderCMeshSource"
		SaveTxtProp Obj collider_mesh_txt "ColliderCMeshSrcName"
		
		
		SaveCkbProp Obj export_trigger_ckb "ExportTriggerC"
		SaveDDLProp Obj trigger_type_select "TriggerCType"
		SaveCkbProp Obj trigger_static_ckb "TriggerCStatic"
		SaveRadioProp Obj trigger_mesh_src_radio "TriggerCMeshSource"
		SaveTxtProp Obj trigger_mesh_txt "TriggerCMeshSrcName"
		
		SaveTxtProp Obj trigger_scale_x_txt  "TriggerCScaleX"
		SaveTxtProp Obj trigger_scale_y_txt  "TriggerCScaleY"
		SaveTxtProp Obj trigger_scale_z_txt  "TriggerCScaleZ"
		SaveTxtProp Obj trigger_offset_x_txt "TriggerCOffsetX"
		SaveTxtProp Obj trigger_offset_y_txt "TriggerCOffsetY"
		SaveTxtProp Obj trigger_offset_z_txt "TriggerCOffsetZ"
		
		
		SaveCkbProp Obj export_camera_ckb "ExportCameraC"
		SaveTxtProp Obj cam_offset_x_txt "CameraCOffsetX"
		SaveTxtProp Obj cam_offset_y_txt "CameraCOffsetY"
		SaveTxtProp Obj cam_offset_z_txt "CameraCOffsetZ"
		
		SaveCkbProp Obj export_cc_ckb "ExportCharContrC"
		SaveTxtProp Obj cc_offset_x_txt "CharContrCOffsetX"
		SaveTxtProp Obj cc_offset_y_txt "CharContrCOffsetY"
		SaveTxtProp Obj cc_offset_z_txt "CharContrCOffsetZ"
		SaveTxtProp Obj cc_radius_txt "CharContrCRadius"
		SaveTxtProp Obj cc_height_txt "CharContrCHeight"
		
		SaveCkbProp Obj export_speaker_ckb "ExportSpeakerC"
		setUserProp Obj "SpeakerCVolume" speaker_volume_txt.value
		
		SaveCkbProp Obj export_particle_ckb "ExportParticleSC"
		SaveTxtProp Obj emitter_class_txt "ParticleSCClass"
		
		-- TODO: Export scripts
		
		SaveCkbProp Obj export_light_ckb "ExportAsLight"
		
		SaveCkbProp Obj export_cinem_ckb "ExportTAnimation"
		SaveRadioProp Obj cinematic_type_radio "TAnimationType"
	)
	
	fn selectedObjectChanged =
	(
		if autoload_ckb.checked then
		(
			LoadObjectProps()
		)
	)

	on ExportSceneEditor open do
	(
		callbacks.addScript #selectionSetChanged "ExportSceneEditor.selectedObjectChanged()" id:#UABSceneProperties
		
		LoadObjectProps()
	)
	on ExportSceneEditor close do
	(
		callbacks.removeScripts #selectionSetChanged id:#UABSceneProperties
	)
	
	on get_btn pressed do
	(
		if $ == undefined then
		(
			messageBox "No has seleccionado ningún objeto."
		)
		else if $ == $selection then
		(
			messageBox "Sólo se puede obtener las propiedades de un objeto."
		)
		else
		(
			LoadObjectProps()
		)
	)
	
	on set_btn pressed do
	(	
		if $ == undefined then
		(
			messageBox "No has seleccionado ningún objeto."
		)
		else if $ == $selection then
		(
			local l_Option = queryBox "Hay más de un objeto seleccionado para guardar las propiedades.\n ¿Seguro que quieres guardar estas propiedades a los objetos?"
			if l_Option then
			(
				for i = 1 to $selection.count do
				(
					SaveObjectProps $selection[i]
				)
			)
		)
		else
		(
			if m_ObjSelected == $ then
			(
				SaveObjectProps $
			)
			else 
			(
				local l_Option  = queryBox "El objeto seleccionado no es el mismo del que se obtenieron las propiedades.\n ¿Asignar las propiedades al objeto?"
				if l_Option then
				(
					SaveObjectProps $
				)
			)
		)
		
	)
	on autoloadCkbx changed state do
	(
		if state then
		(
			selectedObjectChanged()
		)
	)
	
	
	
	on export_cbx changed state do
	(
		UpdateEnabledProps()
	)
	on export_rend_ckb changed state do
	(
		UpdateEnabledProps()
	)
	on export_collider_ckb changed state do
	(
		UpdateEnabledProps()
	)
	on export_trigger_ckb changed state do
	(
		UpdateEnabledProps()
	)
	on export_camera_ckb changed state do
	(
		UpdateEnabledProps()
	)
	on export_cc_ckb changed state do
	(
		UpdateEnabledProps()
	)
	on scripts_combo changed state do
	(
		UpdateEnabledProps()
	)
	on scripts_edit_btn changed state do
	(
		UpdateEnabledProps()
	)
	on export_light_ckb changed state do
	(
		UpdateEnabledProps()
	)
	on export_cinem_ckb changed state do
	(
		UpdateEnabledProps()
	)
	on export_particle_ckb changed state do
	(
		UpdateEnabledProps()
	)
	on export_speaker_ckb changed state do
	(
		UpdateEnabledProps()
	)
	on script_add_btn changed state do
	(
		UpdateEnabledProps()
	)
	
	on static_mesh_ckb changed state do
	(
		skinned_mesh_ckb.checked = not state
	)
	on skinned_mesh_ckb changed state do
	(
		static_mesh_ckb.checked = not state
	)
)

CreateDialog ExportSceneEditor
