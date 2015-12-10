rollout UABSetProperties "UABSetProperties" width:264 height:481
(
	checkbox cbExportRender "Export" pos:[14,29] width:61 height:26
	checkbox cbVisibleRender "Visible" pos:[14,61] width:68 height:28
	checkbox cbLayerRender "Layer" pos:[14,92] width:54 height:34
	dropdownList ddlExportRender "" pos:[81,32] width:158 height:21
	dropdownList ddlLayerRender "" pos:[81,95] width:158 height:21
	edittext txtAnimModel "Anim. Mode   " pos:[13,151] width:226 height:23
	checkbox cbGeneratePhysics "Generate" pos:[14,208] width:66 height:22
	checkbox cbLayerPhysics "Layer" pos:[14,242] width:48 height:22
	checkbox cbGenerateLogic "Generate" pos:[14,311] width:65 height:22
	checkbox cbExportLight "Export" pos:[14,379] width:92 height:27
	dropdownList ddlGeneratePhysics "" pos:[81,208] width:158 height:21
	dropdownList ddlLayerPhysics "" pos:[81,243] width:158 height:21
	dropdownList ddlGenerateLogic "" pos:[81,310] width:158 height:21
	GroupBox grpRender "Physics" pos:[8,191] width:248 height:88
	GroupBox grpPhysics "Logic" pos:[8,291] width:248 height:58
	GroupBox grpLogic "Render" pos:[8,13] width:248 height:174
	button btnGet "Get" pos:[25,439] width:85 height:30
	button btnSet "Set" pos:[129,439] width:85 height:30

	local m_ObjSelected = undefined
	
	
	----------------------------------------------------
	-- UABSetProperties: metodo ejecutado al inicio. Carga los ddl.
	----------------------------------------------------
	
	
	
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
			--todo light
			if cbExportLight.checked and  SuperclassOf Obj == light  then
			(
				setUserProp Obj "light_export" cbExportLight.checked
			)
			else
			(
				setUserProp Obj "light_export" "undefined"
			)
	)
	

	----------------------------------------------------
	--btnSet 
	----------------------------------------------------


	on UABSetProperties open do
	(	
		ddlExportRender.items =#("Static Mesh","Animated Models")-- l_ExportRender
		ddlLayerRender.items = #("solid", "alpha_objects","alpha_blend_objects", "particles");
		ddlGeneratePhysics.items = #("malla triangular", "convex shape", "box shape", "sphere shape", "plane shape", "capsule shape")
		ddlLayerPhysics.items=#("PhysicsLayer")
		ddlGenerateLogic.items = #("logicGenerate")
		txtAnimModel.enabled=false
		
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
	----------------------------------------------------
	--btnGet
	----------------------------------------------------
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
			m_ObjSelected = $
			UABSetProperties.title=m_ObjSelected.name
			local l_ExportRender = getUserProp $ "render_export"
			local l_VisibleRender = getUserProp $ "render_visible"
			local l_LayerRender = getUserProp $ "render_layer"
			local l_AnimModel = getUserProp $ "anim_model_core"
			local l_GeneratePhysics = getUserProp $ "physics_generate"
			local l_LayerPhysics = getUserProp $ "physics_layer"
			local l_GenerateLogic = getUserProp $ "logic_generate"
			local l_ExportLight = getUserProp $ "light_export"
			
			--Reset de cb y text
			ResetProps()
			
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
			
			--Visible Render			i
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
)
createDialog UABSetProperties