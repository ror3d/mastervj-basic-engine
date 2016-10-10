global ExportSceneEditor

struct script_t (name, props)
struct script_property_t (name, type, val)
struct script_property_array_val_t (type, val)

global g_Scripts = #()
global g_CurrentScript = 0

rollout ScriptPropertiesEditor "Script Properties" width:432 height:296
(
	local property_value_types = #("string", "number", "boolean", "array")
	local array_value_types = #("string", "number", "boolean")
	
	button save_btn "Save" pos:[101,256] width:96 height:32
	button cancel_btn "Cancel" pos:[213,256] width:96 height:32
	radiobuttons type_radio "Type" pos:[184,24] width:61 height:78 enabled:true labels:#("string", "number", "boolean", "array") columns:1
	GroupBox grp14 "Scalar Value" pos:[256,24] width:168 height:40
	edittext scalar_value_txt "" pos:[264,40] width:152 height:16
	GroupBox grp15 "Array Value" pos:[256,72] width:168 height:176
	listbox array_values_list "" pos:[264,136] width:152 height:7
	edittext array_value_value_txt "" pos:[344,88] width:72 height:16
	dropdownList array_value_type_ddl "" pos:[264,88] width:72 height:21 items:#("string", "number", "boolean")
	button array_add_btn "Add" pos:[280,112] width:56 height:16
	button array_remove_btn "Remove" pos:[352,112] width:56 height:16
	edittext script_name_txt "" pos:[14,8] width:146 height:21
	button add_prop_btn "Add New" pos:[176,119] width:68 height:22
	
	
	button update_prop_btn "Update" pos:[176,152] width:68 height:22
	button remove_prop_btn "Remove" pos:[176,184] width:68 height:22
	listbox properties_list "" pos:[16,96] width:147 height:11
	edittext property_name_txt "" pos:[16,72] width:144 height:24
	label lbl51 "Properties" pos:[16,56] width:120 height:16
	
	local m_script = undefined
	
	local m_arrayVals = #()
	
	fn EnableInputs = (
		if type_radio.state == 4 then -- array
		(
			scalar_value_txt.enabled = false
			array_value_value_txt.enabled = true
			array_value_type_ddl.enabled = true
			array_add_btn.enabled = true
			array_remove_btn.enabled = true
			array_values_list.enabled = true
		)
		else
		(
			scalar_value_txt.enabled = true
			array_value_value_txt.enabled = false
			array_value_type_ddl.enabled = false
			array_add_btn.enabled = false
			array_remove_btn.enabled = false
			array_values_list.enabled = false
		)
	)
	
	fn AddArrayValue type val = (
		local l = array_values_list.items
		local av = script_property_array_val_t type: type val: val
		append l (type + ": " + val)
		append m_arrayVals av
		array_values_list.items = l
		array_values_list.selection = 0
	)
	
	fn InitPropertyValues propIdx = (
		if propIdx < 1 then
		(
			return ok
		)
		
		local prop = m_script.props[propIdx]
		
		property_name_txt.text = prop.name
		
		array_value_value_txt.text = ""
		array_value_type_ddl.selection = 1
		
		if prop.type == "array" then
		(
			scalar_value_txt.text = ""
			
			local l = #()
			m_arrayVals = prop.val
			for i = 1 to prop.val.count do
			(
				local av = prop.val[i].type + ": " + prop.val[i].val
				append l av
			)
			array_values_list.items = l
			array_values_list.selection = 0
			type_radio.state = 4
		)
		else
		(
			scalar_value_txt.text = prop.val
			array_values_list.items = #()
			m_arrayVals = #()
			
			if prop.type == "string" then
			(
				type_radio.state = 1
			)
			else if prop.type == "number" then
			(
				type_radio.state = 2
			)
			else if prop.type == "boolean" then
			(
				type_radio.state = 3
			)
		)
	)
	
	on ScriptPropertiesEditor open do
	(
		m_script = g_Scripts[g_CurrentScript]
		script_name_txt.text = m_script.name
		local props = #()
		for i = 1 to m_script.props.count do
		(
			append props m_script.props[i].name
		)
		properties_list.items = props
		properties_list.selection = 0
		
		EnableInputs()
	)
	on save_btn pressed do
	(
		m_script.name = script_name_txt.text
		g_Scripts[g_CurrentScript] = m_script
		DestroyDialog ScriptPropertiesEditor
	)
	on cancel_btn pressed do
	(
		DestroyDialog ScriptPropertiesEditor
	)
	on type_radio changed state do
	(
		EnableInputs()
	)
	on array_add_btn pressed do
	(
		AddArrayValue array_value_types[array_value_type_ddl.selection] array_value_value_txt.text
	)
	on array_remove_btn pressed do
	(
		if array_values_list.selection != 0 then
		(
			local l = array_values_list.items
			local idx = array_values_list.selection
			deleteItem l idx
			deleteItem m_arrayVals idx
			array_values_list.items = l
			array_values_list.selection = 0
		)
	)
	on add_prop_btn pressed do
	(
		local propName = property_name_txt.text
		property_name_txt.text = ""
		if propName != "" then
		(
			local comboItems = properties_list.items
			local props = m_script.props
			
			local type = property_value_types[type_radio.state]
			local val = scalar_value_txt.text
			
			if type_radio.state == 4 then
			(
				val = m_arrayVals --array_values_list.items
			)
			
			local prop = script_property_t name: propName type: type val: val
			
			local itemIdx = findItem comboItems propName
			if itemIdx == 0 then -- Does not exist: add
			(
				append props prop
				append comboItems propName
			)
			else -- Exists: edit
			(
				props[itemIdx] = prop
			)
			
			m_script.props = props
			properties_list.items = comboItems
			
			-- TODO: Empty inputs
		)
	)
	on update_prop_btn pressed do
	(
		local propIdx = properties_list.selection
		local propName = property_name_txt.text
		if propIdx != 0 and propName != "" then
		(
			local comboItems = properties_list.items
			local props = m_script.props
			
			local type = property_value_types[type_radio.state]
			local val = scalar_value_txt.text
			
			if type_radio.state == 4 then
			(
				val = m_arrayVals --array_values_list.items
			)
			
			local prop = script_property_t name: propName type: type val: val
			
			comboItems[propIdx] = propName
			props[propIdx] = prop
			
			m_script.props = props
			properties_list.items = comboItems
		)
	)
	on remove_prop_btn pressed do
	(
		local propIdx = properties_list.selection
		if propIdx != 0 then
		(
			local comboItems = properties_list.items
			local props = m_script.props
			deleteItem comboItems propIdx
			deleteItem props propIdx
			properties_list.items = comboItems
			m_script.props = props
		)
	)
	on properties_list selected sel do
	(
		InitPropertyValues sel
		EnableInputs()
	)
)

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
	multiListBox layer_select "Layers" pos:[40,112] width:168 height:4 enabled:false items:#("solid", "models")
	edittext skinned_mesh_core_txt "" pos:[72,256] width:136 height:16 enabled:false
	label lbl3 "Model Name" pos:[72,240] width:104 height:16
	GroupBox grp3 "Collider" pos:[232,80] width:200 height:136
	checkbox export_collider_ckb "Export Collider" pos:[240,96] width:152 height:16
	dropdownList collider_type_select "Collider Type" pos:[248,152] width:152 height:40 enabled:false items:#("Triangle Mesh", "Convex Mesh", "Bounding Box", "Bounding Sphere")
	checkbox collider_static_ckb "Static" pos:[256,112] width:144 height:16 enabled:false
	checkbox collider_kinem_ckb "Kinematic" pos:[256,128] width:144 height:16 enabled:false
	GroupBox grp4 "Trigger" pos:[440,80] width:200 height:192
	dropdownList trigger_type_select "Collider Type" pos:[464,136] width:152 height:40 enabled:false items:#("Bounding Box", "Bounding Sphere", "Triangle Mesh", "Convex Mesh")
	checkbox trigger_static_ckb "Static" pos:[464,112] width:144 height:16 enabled:false
	label lbl12 "Scale" pos:[456,184] width:80 height:16
	edittext trigger_scale_x_txt "x" pos:[464,200] width:48 height:16 enabled:false
	edittext trigger_scale_y_txt "y" pos:[520,200] width:48 height:16 enabled:false
	edittext trigger_scale_z_txt "z" pos:[576,200] width:48 height:16 enabled:false
	label lbl13 "Offset" pos:[456,224] width:80 height:16
	edittext trigger_offset_x_txt "x" pos:[464,240] width:48 height:16 enabled:false
	edittext trigger_offset_y_txt "y" pos:[520,240] width:48 height:16 enabled:false
	edittext trigger_offset_z_txt "z" pos:[576,240] width:48 height:16 enabled:false
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
	GroupBox grp36 "Transform Cinematic" pos:[221,552] width:200 height:112
	button get_btn "Get" pos:[16,680] width:136 height:40
	button set_btn "Set" pos:[160,680] width:136 height:40
	checkbox autoload_ckb "Load automatically when selection changes" pos:[312,680] width:232 height:16
	checkbox export_light_ckb "Export As Light" pos:[24,568] width:176 height:16
	checkbox export_cinem_ckb "Export Transform Anim." pos:[232,568] width:152 height:16
	local CinematicsLoopTypes = #("Cycle", "Reverse", "One-Shot")
	radiobuttons cinematic_type_radio "" pos:[248,592] width:68 height:48 enabled:false labels:#("Cycle", "Reverse", "One-Shot") columns:1
	checkbox cinem_playStart_ckb "Play At Start" pos:[248,642] width:109 height:18
	checkbox export_particle_ckb "Export Emitter" pos:[448,344] width:152 height:16
	label lbl43 "Emitter Class" pos:[464,368] width:112 height:16
	edittext emitter_class_txt "" pos:[464,384] width:144 height:16
	GroupBox grp49 "Audio Speaker" pos:[440,424] width:200 height:96
	checkbox export_speaker_ckb "Export Speaker" pos:[448,440] width:152 height:16
	spinner speaker_volume_txt "Volume" pos:[489,464] width:95 height:16 enabled:false range:[0,100,100] type:#integer
	button script_add_btn "Add Script" pos:[96,456] width:120 height:21
	
	GroupBox grp14 "Spawn" pos:[430,553] width:200 height:45
	checkbox export_spawn_ckb "Export As Spawn" pos:[440,569] width:152 height:16
	
	
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
			export_spawn_ckb.enabled = true
			
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
		
		trigger_type_select.enabled = eTrig
		trigger_static_ckb.enabled = eTrig
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
		cinem_playStart_ckb.enabled = export_cinem_ckb.checked
		
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
		
		export_trigger_ckb.checked = false
		trigger_type_select.selection = 1
		trigger_static_ckb.checked = false

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
		cinem_playStart_ckb.checked = false
		cinematic_type_radio.state = 1
		
		export_spawn_ckb.checked = false
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
			txt.text = val as string
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
	
	fn LoadVec3DProp txt1 txt2 txt3 propName = (
		local val = getUserProp m_ObjSelected propName
		if val != undefined and val != "" then (
			local vals = filterString val " "
			txt1.text = vals[1]
			txt2.text = vals[2]
			txt3.text = vals[3]
		)
	)
	
	fn SaveVec3DProp obj txt1 txt2 txt3 propName = (
		local v1 = "0"
		local v2 = "0"
		local v3 = "0"
		if txt1.text != "" then (
			v1 = (txt1.text as float) as string
		)
		if txt2.text != "" then (
			v2 = (txt2.text as float) as string
		)
		if txt2.text != "" then (
			v3 = (txt3.text as float) as string
		)
		setUserProp obj propName (v1 + " " + v2 + " " + v3)
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
	
	fn LoadRadioProp radio arr propName = (
		local val = getUserProp m_ObjSelected propName
		if val != undefined then
		(
			for i = 1 to arr.count do
			(
				if arr[i] == val then
				(
					radio.state = i
					exit
				)
			)
		)
		else
		(
			radio.state = 0
		)
	)
	
	fn SaveRadioProp obj radio arr propName = (
		if radio.state != 0 then
		(
			setUserProp obj propName arr[radio.state]
		)
		else
		(
			deleteUserProp obj propName
		)
	)
	
	fn SaveScripts obj scriptList propName =
	(
		setUserProp obj propName scriptList
	)
	
	fn LoadScripts combo propName =
	(
		local val = getUserProp m_ObjSelected propName
		local items = #()
		g_Scripts = #()
		if val != undefined then
		(
			local scripts = execute val
			g_Scripts = scripts
			print g_Scripts
			for i = 1 to scripts.count do
			(
				append items scripts[i].name
			)
		)
		combo.items = items
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
		LoadCkbProp skinned_mesh_ckb "RendererSkinnedMesh"
		static_mesh_ckb.checked = not skinned_mesh_ckb.checked
		
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
		
		skinned_mesh_core_txt.enabled = skinned_mesh_ckb.checked
		LoadTxtProp skinned_mesh_core_txt "SkinnedMeshCore"
		
		
		LoadCkbProp export_collider_ckb "ExportColliderC"
		LoadDDLProp collider_type_select "ColliderCType"
		LoadCkbProp collider_static_ckb "ColliderCStatic"
		LoadCkbProp collider_kinem_ckb "ColliderCKinematic"
		
		
		LoadCkbProp export_trigger_ckb "ExportTriggerC"
		LoadDDLProp trigger_type_select "TriggerCType"
		LoadCkbProp trigger_static_ckb "TriggerCStatic"
		LoadVec3DProp trigger_scale_x_txt trigger_scale_y_txt trigger_scale_z_txt "TriggerCScale"
		LoadVec3DProp trigger_offset_x_txt trigger_offset_y_txt trigger_offset_z_txt "TriggerCOffset"
		
		
		LoadCkbProp export_camera_ckb "ExportCameraC"
		LoadVec3DProp cam_offset_x_txt cam_offset_y_txt cam_offset_z_txt "CameraCOffset"
		
		LoadCkbProp export_cc_ckb "ExportCharContrC"
		LoadVec3DProp cc_offset_x_txt cc_offset_y_txt cc_offset_z_txt "CharContrCOffset"
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
		LoadRadioProp cinematic_type_radio CinematicsLoopTypes "TAnimationType"
		LoadCkbProp cinem_playStart_ckb "TAnimationPlayStart"
		LoadCkbProp export_spawn_ckb "ExportAsSpawn"
		
		g_Scripts = #()
		
		LoadScripts scripts_combo "ExportScripts"
		
		UpdateEnabledProps()
	)
	
	function SaveObjectProps Obj =
	(
		local exportMeshCore = false
		
		SaveCkbProp Obj export_cbx "ExportGameObject"
		
		SaveCkbProp Obj export_rend_ckb "ExportRendererC"
		SaveCkbProp Obj skinned_mesh_ckb "RendererSkinnedMesh"
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
			deleteUserProp Obj "RendererLayer"
		)
		SaveTxtProp Obj skinned_mesh_core_txt "SkinnedMeshCore"
		
		exportMeshCore = exportMeshCore or (export_rend_ckb.checked and static_mesh_ckb.checked)
		
		
		SaveCkbProp Obj export_collider_ckb "ExportColliderC"
		SaveDDLProp Obj collider_type_select "ColliderCType"
		SaveCkbProp Obj collider_static_ckb "ColliderCStatic"
		SaveCkbProp Obj collider_kinem_ckb "ColliderCKinematic"
		
		exportMeshCore = exportMeshCore or (export_collider_ckb.checked and (collider_type_select.selected == "Triangle Mesh" or collider_type_select.selected == "Convex Mesh"))
		
		
		SaveCkbProp Obj export_trigger_ckb "ExportTriggerC"
		SaveDDLProp Obj trigger_type_select "TriggerCType"
		SaveCkbProp Obj trigger_static_ckb "TriggerCStatic"
		
		exportMeshCore = exportMeshCore or (export_trigger_ckb.checked and (trigger_type_select.selected == "Triangle Mesh" or trigger_type_select.selected == "Convex Mesh"))
		
		SaveVec3DProp Obj trigger_scale_x_txt trigger_scale_y_txt trigger_scale_z_txt "TriggerCScale"
		SaveVec3DProp Obj trigger_offset_x_txt trigger_offset_y_txt trigger_offset_z_txt "TriggerCOffset"
		
		
		SaveCkbProp Obj export_camera_ckb "ExportCameraC"
		SaveVec3DProp Obj cam_offset_x_txt cam_offset_y_txt cam_offset_z_txt "CameraCOffset"

		
		SaveCkbProp Obj export_cc_ckb "ExportCharContrC"
		SaveVec3DProp Obj cc_offset_x_txt cc_offset_y_txt cc_offset_z_txt "CharContrCOffset"
		SaveTxtProp Obj cc_radius_txt "CharContrCRadius"
		SaveTxtProp Obj cc_height_txt "CharContrCHeight"
		
		
		SaveCkbProp Obj export_speaker_ckb "ExportSpeakerC"
		setUserProp Obj "SpeakerCVolume" speaker_volume_txt.value
		
		
		SaveCkbProp Obj export_particle_ckb "ExportParticleSC"
		SaveTxtProp Obj emitter_class_txt "ParticleSCClass"
		
		-- TODO: Export scripts
		
		SaveCkbProp Obj export_light_ckb "ExportAsLight"
		
		SaveCkbProp Obj export_cinem_ckb "ExportTAnimation"
		SaveRadioProp Obj cinematic_type_radio CinematicsLoopTypes "TAnimationType"
		SaveCkbProp Obj cinem_playStart_ckb "TAnimationPlayStart"
		
		SaveCkbProp Obj export_spawn_ckb "ExportAsSpawn"
		
		if exportMeshCore then (
			setUserProp Obj "export_mesh_instance" true
		) else (
			deleteUserProp Obj "export_mesh_instance"
			deleteUserProp Obj "export_core_name"
		)
		
		SaveScripts Obj g_Scripts "ExportScripts"
	)
	
	fn selectedObjectChanged =
	(
		if autoload_ckb.checked then
		(
			LoadObjectProps()
		)
	)

	
	
	
	
	
	
	button reset_btn "Reset" pos:[576,680] width:80 height:40
	button remove_script_btn "Remove" pos:[32,456] width:56 height:21
	
	
	
	
	on ExportSceneEditor open do
	(
		callbacks.addScript #selectionSetChanged "ExportSceneEditor.selectedObjectChanged()" id:#UABSceneProperties
		
		LoadObjectProps()
	)
	on ExportSceneEditor close do
	(
		callbacks.removeScripts #selectionSetChanged id:#UABSceneProperties
	)
	on export_cbx changed state do
	(
		UpdateEnabledProps()
	)
	on export_rend_ckb changed state do
	(
		UpdateEnabledProps()
	)
	on static_mesh_ckb changed state do
	(
		skinned_mesh_ckb.checked = not state
		skinned_mesh_core_txt.enabled = skinned_mesh_ckb.checked
	)
	on skinned_mesh_ckb changed state do
	(
		static_mesh_ckb.checked = not state
		skinned_mesh_core_txt.enabled = skinned_mesh_ckb.checked
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
	on export_spawn_ckb state do
	(
		UpdateEnabledProps()
	)
	on scripts_edit_btn pressed do
	(
		if scripts_combo.selection != 0 then
		(
			g_CurrentScript = scripts_combo.selection
			CreateDialog ScriptPropertiesEditor modal: true parent: ExportSceneEditor.hwnd
			local l = scripts_combo.items
			l[g_CurrentScript] = g_Scripts[g_CurrentScript].name
			scripts_combo.items = l
		)
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
	on script_add_btn pressed do
	(
		local txt = scripts_combo.text
		print txt
		if txt != "" then
		(
			local l = scripts_combo.items
			local i = findItem l scripts_combo.text
			if i == 0 then
			(
				append l txt
				scripts_combo.items = l
				local s = script_t name: txt props: #()--(script_property_t name: scripts_combo.text type: "string" val: "")
				append g_Scripts s
				print g_Scripts
				i = scripts_combo.items.count
			)
			scripts_combo.text = ""
			scripts_combo.selection = i
		)
	)
	on reset_btn pressed do
	(	
		ResetProps()
		UpdateEnabledProps()
	)
	on remove_script_btn pressed do
	(
		if scripts_combo.selection != 0 then
		(
			local l = scripts_combo.items
			local id = scripts_combo.selection
			deleteItem l id
			deleteItem g_Scripts id
			scripts_combo.items = l
			scripts_combo.selection = 0
		)
	)
	on export_spawn_ckb changed state do
	(
		UpdateEnabledProps()
	)
)

CreateDialog ExportSceneEditor
