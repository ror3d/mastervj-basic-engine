global UABPropertyManager
rollout UABPropertyManager "UABPropertyManager" width:558 height:531
(
	label lblObjectName "ObjectName" pos:[15,10] width:250 height:22
	
	checkbox cbExportRenderableObject "Export Renderable Object" pos:[14,52] width:154 height:19
	checkbox cbLayerRender "Layer" pos:[14,106] width:54 height:19
	dropdownList ddlLayerRender "" pos:[81,108] width:158 height:21
	edittext txtAnimModel "Anim. Model " pos:[14,140] width:226 height:23	
	checkbox cbLayerPhysics "Layer" pos:[14,254] width:48 height:19
	GroupBox grpLights "Lights" pos:[8,383] width:248 height:58
	checkbox cbExportLight "Export Light" pos:[14,405] width:92 height:19	
	dropdownList ddlLayerPhysics "" pos:[81,255] width:158 height:21	
	GroupBox grpLogic "Render" pos:[8,29] width:532 height:174
	button btnGet "Get" pos:[25,456] width:85 height:30
	button btnSet "Set" pos:[129,456] width:85 height:30
	checkbox autoloadCkbx "Auto load on selection change" pos:[25,500] width:187 height:19	
	
	-- Physics
	GroupBox grpRender "Physics" pos:[8,203] width:248 height:88
	checkbox cbGeneratePhysics "Generate" pos:[14,220] width:66 height:19
	dropdownList ddlGeneratePhysics "" pos:[81,220] width:158 height:21
	
	--LOGIC
	GroupBox grpPhysics "Logic" pos:[8,291] width:248 height:82
	checkbox cbGenerateLogic "Generate" pos:[14,311] width:65 height:19
	dropdownList ddlGenerateLogic "" pos:[81,310] width:158 height:21
	edittext edtLogicActivatesText "Activates" pos:[28,339] width:200 height:21	
	
	--ANIMATIONS
	GroupBox grp15 "Cinematics" pos:[261,212] width:279 height:61
	checkbox cbExportCinematics "Export" pos:[268,230] width:56 height:17
	local CinematicsLoopTypes = #("Cycle", "Reverse", "One-Shot")
	radiobuttons rbCinematicsLoopType "" pos:[290,250] width:255 height:16 enabled:false labels:#("Cycle", "Reverse", "One-Shot") columns:3

	--TRIGGERS
	GroupBox grpTrigger "Triggers" pos:[263,281] width:277 height:120
	checkbox cbIsTrigger "Is Trigger" pos:[269,301] width:117 height:13
	local TriggerTypes = #("Sphere", "Box")
	radiobuttons rdoTriggerType "" pos:[295,324] width:134 height:16 enabled:false labels:#("Sphere", "Box") default:2 columns:2
	checkbox cbTriggerActivedOnce "Active Once" pos:[439,325] width:87 height:14
	

	local m_ObjSelected = undefined	
	
	fn ToggleInputElements enabled =
	(
		cbExportRenderableObject.enabled = enabled
		
		cbLayerRender.enabled = false --enabled
		cbGeneratePhysics.enabled = false --enabled
		cbLayerPhysics.enabled = false --enabled
		cbGenerateLogic.enabled = false --enabled
		txtAnimModel.enabled = false --enabled
		txtAnimModel.enabled = false --enabled
		
		cbExportLight.enabled = enabled
		cbExportCinematics.enabled = enabled
		rbCinematicsLoopType.enabled = enabled
		
		cbIsTrigger.enabled = false
	)
	
	----------------------------------------------------
	--ResetProps: resetea cb y txt
	----------------------------------------------------
	function ResetProps =
	(
		cbExportRenderableObject.checked = false
		
		cbLayerRender.checked = false
		cbGeneratePhysics.checked = false
		cbLayerPhysics.checked = false
		
		cbGenerateLogic.checked = false
		ddlGenerateLogic.enabled = false
		edtLogicActivatesText.enabled = false
		
		txtAnimModel.text = ""
		txtAnimModel.enabled= false
		cbExportLight.checked = false
		
		cbExportCinematics.checked = false
		rbCinematicsLoopType.enabled = false
		rbCinematicsLoopType.state = 1
		
		cbIsTrigger.checked = false
		rdoTriggerType.enabled = false
		cbTriggerActivedOnce.enabled = false
		
		
		lblObjectName.caption = ""
		
		if autoloadCkbx.checked then
		(
			ToggleInputElements false
		)
	)
	
	fn LoadObjectProps =
	(
		--Reset de cb y text
		ResetProps()
		
		if $ == undefined or $ == $selection then
		(
			return false
		)
		
		m_ObjSelected = $
		UABPropertyManager.title = m_ObjSelected.name
		lblObjectName.caption = m_ObjSelected.name
		
		--Export Renderable Object
		local l_ExportRenderableObject = getUserProp m_ObjSelected "render_export_rend_obj"
		if l_ExportRenderableObject != undefined then
		(
			cbExportRenderableObject.checked = true
		)
		
		--Trigger
		local l_IsTrigger = getUserProp m_ObjSelected "is_trigger"
		local l_TriggerType = getUserProp m_ObjSelected "tigger_type"
		
		if l_IsTrigger!= "undefined" then
		(
			
			cbIsTrigger.checked = true
			rdoTriggerType.enabled = true
			cbTriggerActivedOnce.enabled = true
			local l_TriggActivedOnce = getUserProp m_ObjSelected "trigger_active_once"
			if l_TriggActivedOnce != "" then
			(
				cbTriggerActivedOnce.checked = true
			)
			else
			(
				cbTriggerActivedOnce.checked = false
			)
									 
		)
		
		/*
		--Layer Render
		local l_LayerRender = getUserProp m_ObjSelected "render_layer"
		if l_LayerRender != undefined then
		(
			for i = 1 to ddlLayerRender.items.count do
			(
				if ddlLayerRender.items[i] == l_LayerRender then
				(
					ddlLayerRender.selection = i
					cbLayerRender.checked = true
				)
			)
		)
		
		--Animated Core Model
		local l_AnimModel = getUserProp m_ObjSelected "anim_model_core"
		if l_AnimModel != undefined then
		(
			txtAnimModel.text = l_AnimModel
		)
		*/
		--Generate Physics
		local l_GeneratePhysics = getUserProp m_ObjSelected "physics_generate"
		if l_GeneratePhysics != undefined then
		(
			for i = 1 to ddlGeneratePhysics.items.count do
			(
				if ddlGeneratePhysics.items[i] == l_GeneratePhysics then
				(
					ddlGeneratePhysics.selection = i
					cbGeneratePhysics.checked = true
				)
			)
		)
		/*
		--Layer Physics 
		local l_LayerPhysics = getUserProp m_ObjSelected "physics_layer"
		if l_LayerPhysics != undefined then
		(
			for i = 1 to ddlLayerPhysics.items.count do
			(
				if ddlLayerPhysics.items[i] == l_LayerPhysics then
				(
					ddlLayerPhysics.selection = i
					cbLayerPhysics.checked = true
				)
			)
		)
		*/
		--Generate Logic
		local l_GenerateLogic = getUserProp m_ObjSelected "logic_type"
		if l_GenerateLogic != undefined then
		(
			ddlGenerateLogic.enabled = true
			for i = 1 to ddlGenerateLogic.items.count do
			(
				if ddlGenerateLogic.items[i] == l_GenerateLogic then
				(
					ddlGenerateLogic.selection = i
					cbGenerateLogic.checked  =true
				)
			)
			edtLogicActivatesText.enabled = true
			local l_LogicActivates = getUserProp m_ObjSelected "logic_activates"
			edtLogicActivatesText.text = l_LogicActivates
		)
		else
		(
			edtLogicActivatesText.enabled = false
		)
		
		-- Export Light
		local l_ExportLight = getUserProp m_ObjSelected "light_export"
		
		if l_ExportLight != undefined  and SuperclassOf m_ObjSelected == light then
		(
			cbVisibleRender.enabled=check
		)
		
		
		-- Cinematic Loop
		local l_ExportCinematics = getUserProp m_ObjSelected "cinematics_export"
		local l_CinemLoopType = getUserProp m_ObjSelected "cinematics_loop"
		
		if l_ExportCinematics != undefined then
		(
			cbExportCinematics.checked = l_ExportCinematics
			rbCinematicsLoopType.enabled = l_ExportCinematics
		)
		if l_CinemLoopType != undefined then
		(
			for i = 1 to CinematicsLoopTypes.count do
			(
				if CinematicsLoopTypes[i] == l_CinemLoopType then
				(
					rbCinematicsLoopType.state = i
					exit
				)
			)
		)
		
	
		
		return true
	)
	
	----------------------------------------------------
	--SaveObjectProps: carga al obj las propiedades seleccionadas
	----------------------------------------------------
	function SaveObjectProps Obj =
	(
		-- Renderable Object
		if cbExportRenderableObject.checked then
		(
			setUserProp Obj "render_export_rend_obj" "true"
		)
		else
		(
			deleteUserProp Obj "render_export_rend_obj"
		)
		
		/*
		--Layer Render 
		if cbLayerRender.checked then
		(
			setUserProp Obj "render_layer" ddlLayerRender.selected
		)
		else
		(
			setUserProp Obj "render_layer" "undefined"
		)
		
		--Anim Model Core 
		setUserProp Obj "anim_model_core" txtAnimModel.text
		*/
		
		--Generate Physics
		if cbGeneratePhysics.checked then
		(
			setUserProp Obj "physics_generate" ddlGeneratePhysics.selected
		)
		else
		(
			setUserProp Obj "physics_generate" "undefined"
		)
		/*
		
		--Layer Physics 
		if cbLayerPhysics.checked then
		(
			setUserProp Obj "physics_layer" ddlLayerPhysics.selected
		)
		else
		(
			setUserProp Obj "physics_layer" "undefined"
		)
		*/
		--Generate Logic 
		if cbGenerateLogic.checked then
		(
			setUserProp Obj "logic_type" ddlGenerateLogic.selected
			setUserProp Obj "logic_activates" edtLogicActivatesText.text
		)
		else
		(
			setUserProp Obj "logic_type" undefined
			setUserProp Obj "logic_activates" ""
		)
		
		
		-- Export light
		if cbExportLight.checked and  SuperclassOf Obj == light  then
		(
			setUserProp Obj "light_export" cbExportLight.checked
		)
		else
		(
			deleteUserProp Obj "light_export"
		)
		
		-- Export Cinematics
		if cbExportCinematics.checked then
		(
			setUserProp Obj "cinematics_export" true
			setUserProp Obj "cinematics_loop" CinematicsLoopTypes[rbCinematicsLoopType.state]
		)
		else
		(
			setUserProp Obj "cinematics_export" false
			setUserProp Obj "cinematics_loop" ""
		)
		
		--Trigger
		if cbIsTrigger.checked then
		(
			setUserProp Obj "is_trigger" true
			setUserProp Obj "trigger_type" TriggerTypes[rdoTriggerType.state]
			if cbTriggerActivedOnce.checked then
			(
				setUserProp Obj "trigger_active_once" true
			)
			else
			(
				setUserProp Obj "trigger_active_once" false
			)
		)
		else
		(
			setUserProp Obj "is_trigger" undefined
			setUserProp Obj "trigger_type" undefined
			setUserProp Obj "trigger_active_once" ""
		)
		
	)
	
	fn selectedObjectChanged =
	(
		if autoloadCkbx.checked then
		(
			if LoadObjectProps() then
			(
				ToggleInputElements true
			)
		)
	)
	
	----------------------------------------------------
	-- UABPropertyManager: metodo ejecutado al inicio. Carga los ddl.
	----------------------------------------------------
	
	
	on UABPropertyManager open do
	(
		ddlLayerRender.items = #("solid", "alpha_objects","alpha_blend_objects", "particles")
		ddlGeneratePhysics.items = #("Triangle Mesh", "Convex Shape", "Bounding Box", "Bounding Sphere", "Bounding Capsule", "Plane shape (?)", "None")
		ddlLayerPhysics.items=#("PhysicsLayer")
		ddlGenerateLogic.items = #("PlacaPres", "Elevator", "Door", "Palanca", "Salida", "SpawnPlayer")
		txtAnimModel.enabled=false
		
		callbacks.addScript #selectionSetChanged "UABPropertyManager.selectedObjectChanged()" id:#UABProperties
		
		LoadObjectProps()
	)
	on UABPropertyManager close do
	(
		callbacks.removeScripts #selectionSetChanged id:#UABProperties
	)
	on btnGet pressed do
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
	on btnSet pressed do
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
	on cbExportCinematics changed state do
	(
		rbCinematicsLoopType.enabled = state
	)
	on cbIsTrigger changed state do
	(
		rdoTriggerType.enabled = state
		cbTriggerActivedOnce.enabled = state		
	)
	on cbPhysx changed state do
	(
		ddlGeneratePhysics.enabled = state	
	)
	on cbGenerateLogic changed state do 
	(			
			ddlGenerateLogic.enabled = state
			edtLogicActivatesText.enabled = state
	)
)	

-- CreateDialog UABPropertyManag