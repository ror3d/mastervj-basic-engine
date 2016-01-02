global UABPropertyManager
rollout UABPropertyManager "UABPropertyManager" width:558 height:531
(
	checkbox cbExportRender "Export" pos:[14,52] width:61 height:19
	checkbox cbVisibleRender "Visible" pos:[14,78] width:68 height:19
	checkbox cbLayerRender "Layer" pos:[14,109] width:54 height:19
	dropdownList ddlExportRender "" pos:[81,49] width:158 height:21
	dropdownList ddlLayerRender "" pos:[81,111] width:158 height:21
	edittext txtAnimModel "Anim. Model " pos:[14,168] width:226 height:23
	checkbox cbGeneratePhysics "Generate" pos:[14,220] width:66 height:19
	checkbox cbLayerPhysics "Layer" pos:[14,254] width:48 height:19
	checkbox cbGenerateLogic "Generate" pos:[14,311] width:65 height:19
	checkbox cbExportLight "Export Light" pos:[14,371] width:92 height:19
	dropdownList ddlGeneratePhysics "" pos:[81,220] width:158 height:21
	dropdownList ddlLayerPhysics "" pos:[81,255] width:158 height:21
	dropdownList ddlGenerateLogic "" pos:[81,310] width:158 height:21
	GroupBox grpRender "Physics" pos:[8,203] width:248 height:88
	GroupBox grpPhysics "Logic" pos:[8,291] width:248 height:58
	GroupBox grpLogic "Render" pos:[8,29] width:532 height:174
	button btnGet "Get" pos:[25,456] width:85 height:30
	button btnSet "Set" pos:[129,456] width:85 height:30
	checkbox autoloadCkbx "Auto load on selection change" pos:[25,500] width:187 height:19
	
	label lblObjectName "ObjectName" pos:[15,10] width:250 height:22
	
	
	GroupBox grp10 "Lights" pos:[8,349] width:248 height:58
	GroupBox grp15 "Cinematics" pos:[261,212] width:279 height:61
	checkbox cbExportCinematics "Export" pos:[268,230] width:56 height:17
	
	local CinematicsLoopTypes = #("Cycle", "Reverse", "One-Shot")
	radiobuttons rbCinematicsLoopType "" pos:[290,250] width:219 height:16 enabled:false labels:CinematicsLoopTypes columns:3
	

	local m_ObjSelected = undefined
	
	
	
	
	
	fn ToggleInputElements enabled =
	(
		cbExportRender.enabled = enabled
		cbVisibleRender.enabled = enabled
		cbLayerRender.enabled = enabled
		cbGeneratePhysics.enabled = enabled
		cbLayerPhysics.enabled = enabled
		cbGenerateLogic.enabled = enabled
		txtAnimModel.enabled = enabled
		txtAnimModel.enabled = enabled
		cbExportLight.enabled = enabled
		cbExportCinematics.enabled = enabled
		rbCinematicsLoopType.enabled = enabled
	)
	
	----------------------------------------------------
	--ResetProps: resetea cb y txt
	----------------------------------------------------
	function ResetProps =
	(
		cbExportRender.checked = false
		cbVisibleRender.checked = false
		cbLayerRender.checked = false
		cbGeneratePhysics.checked = false
		cbLayerPhysics.checked = false
		cbGenerateLogic.checked = false
		txtAnimModel.text = ""
		txtAnimModel.enabled= false
		cbExportLight.checked = false
		
		cbExportCinematics.checked = false
		rbCinematicsLoopType.enabled = false
		rbCinematicsLoopType.state = 1
		
		
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
		local l_ExportRender = getUserProp m_ObjSelected "render_export"
		local l_VisibleRender = getUserProp m_ObjSelected "render_visible"
		local l_LayerRender = getUserProp m_ObjSelected "render_layer"
		local l_AnimModel = getUserProp m_ObjSelected "anim_model_core"
		local l_GeneratePhysics = getUserProp m_ObjSelected "physics_generate"
		local l_LayerPhysics = getUserProp m_ObjSelected "physics_layer"
		local l_GenerateLogic = getUserProp m_ObjSelected "logic_generate"
		local l_ExportLight = getUserProp m_ObjSelected "light_export"
		local l_ExportCinematics = getUserProp m_ObjSelected "cinematics_export"
		local l_CinemLoopType = getUserProp m_ObjSelected "cinematics_loop"
		
		--Export Render
		if l_ExportRender != undefined then
		(
			for i = 1 to ddlExportRender.items.count do
			(
				if ddlExportRender.items[i] == l_ExportRender then
				(
					ddlExportRender.selection = i
					cbExportRender.checked = true
				)
			)
		)
		
		--Visible Render	i
		if l_VisibleRender != undefined then
		(
			cbVisibleRender.checked = l_VisibleRender
		)
		
		--Layer Render
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
		if l_AnimModel != undefined then
		(
			txtAnimModel.text = l_AnimModel
		)
		
		--Generate Physics
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
		
		--Layer Physics 
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
		
		--Generate Logic
		if l_GenerateLogic != undefined then
		(
			for i = 1 to ddlGenerateLogic.items.count do
			(
				if ddlGenerateLogic.items[i] == l_GenerateLogic then
				(
					ddlGenerateLogic.selection = i
					cbGenerateLogic.checked  =true
				)
			)
		)
		
		--todo Export Light
		if l_ExportLight != undefined  and SuperclassOf m_ObjSelected == light then
		(
			cbVisibleRender.enabled=check
		)
		
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
		--Set Export Render Prop to Object
		if cbExportRender.checked then
		(
			setUserProp Obj "render_export" ddlExportRender.selected
		)
		else
		(
			setUserProp Obj "render_export" "undefined"
		)
		
		--Visible Render 
		setUserProp Obj "render_visible" cbVisibleRender.checked
		
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
		
		--Generate Physics
		if cbGeneratePhysics.checked then
		(
			setUserProp Obj "physics_generate" ddlGeneratePhysics.selected
		)
		else
		(
			setUserProp Obj "physics_generate" "undefined"
		)
		
		--Layer Physics 
		if cbLayerPhysics.checked then
		(
			setUserProp Obj "physics_layer" ddlLayerPhysics.selected
		)
		else
		(
			setUserProp Obj "physics_layer" "undefined"
		)
		
		--Generate Logic 
		if cbGenerateLogic.checked then
		(
			setUserProp Obj "logic_generate" ddlGenerateLogic.selected
		)
		else
		(
			setUserProp Obj "logic_generate" "undefined"
		)
		-- Export light
		if cbExportLight.checked and  SuperclassOf Obj == light  then
		(
			setUserProp Obj "light_export" cbExportLight.checked
		)
		else
		(
			setUserProp Obj "light_export" "undefined"
		)
		
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
		ddlExportRender.items =#("Static Mesh","Animated Models")-- l_ExportRender
		ddlLayerRender.items = #("solid", "alpha_objects","alpha_blend_objects", "particles")
		ddlGeneratePhysics.items = #("Triangle Mesh", "Convex Shape", "Bounding Box", "Bounding Sphere", "Bounding Capsule", "Plane shape (?)")
		ddlLayerPhysics.items=#("PhysicsLayer")
		ddlGenerateLogic.items = #("logicGenerate")
		txtAnimModel.enabled=false
		
		callbacks.addScript #selectionSetChanged "UABPropertyManager.selectedObjectChanged()" id:#UABProperties
		
		LoadObjectProps()
	)
	on UABPropertyManager close do
	(
		callbacks.removeScripts #selectionSetChanged id:#UABProperties
	)
	on cbExportRender changed check do
	(
		cbVisibleRender.enabled=check
		cbVisibleRender.enabled=check
		cbLayerRender.enabled=check 
			
	)
	on ddlExportRender selected i do
	(
		if i==2  then
		(
			txtAnimModel.enabled=true
			
		)
		else
		(
			txtAnimModel.enabled=false
			txtAnimModel.text=""
		)
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
)

-- CreateDialog UABPropertyManager

