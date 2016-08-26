fileIn "file-utils.ms"
fileIn "export-scene-utils.ms"

fn GetValidColliderTypeExportString str = (
	if str == "Triangle Mesh" then (
		return "TriangleMesh"
	)
	if str == "Convex Mesh" then (
		return "ConvexMesh"
	)
	if str == "Bounding Sphere" then (
		return "Sphere"
	)
	return "Box"
)

fn createSceneFile sceneName gameObjects = (
	MyCreateFile (SceneSubPath + sceneName + ".xml")
	
	WriteXMLPrologToFile()
	format "<scene name=\"%\">\n" sceneName to:file
	
	for i = 1 to gameObjects.count do
	(
		local gObj = gameObjects[i]
		local Pos = TranslationToRH (gObj.transform.translation)
		local Sc = ScaleToRH gObj.transform.scale
		local Yaw = GetYaw (gObj.transform)
		local Pitch = GetPitch (gObj.transform)
		local Roll = GetRoll (gObj.transform)
		
		format "\t<object name=\"%\" pos=\"% % %\" scale=\"% % %\" rotation=\"% % %\" enabled=\"%\">\n"\
					gObj.name Pos[1] Pos[2] Pos[3] Sc[1] Sc[2] Sc[3] Yaw Pitch Roll (TrueFalse true) to: file
		
		local exportComp = getUserProp gObj "ExportRendererC"
		if exportComp != undefined then (
			local layers = getUserProp gObj "RendererLayer"
			if layers == undefined then (
				layers = "solid"
			)
			local meshTypeSkinned = getUserProp gObj "RendererSkinnedMesh"
			if meshTypeSkinned == undefined then (
				local core = getUserProp gObj "export_core_name"
				format "\t\t<mesh_instance core=\"%\" layers=\"%\" visible=\"%\" />\n" core layers (TrueFalse gObj.visibility) to: file
			) else (
				local core = getUserProp gObj "SkinnedMeshCore"
				format "\t\t<animated_instance core=\"%\" layers=\"%\" visible=\"%\" />\n" core layers (TrueFalse gObj.visibility) to: file
			)
		)
		
		exportComp = getUserProp gObj "ExportColliderC"
		if exportComp != undefined then (
			local type = getUserProp gObj "ColliderCType"
			type = GetValidColliderTypeExportString type
			local core = ""
			if type == "TriangleMesh" or type == "ConvexMesh" then (
				core = getUserProp gObj "export_core_name"
				if core == undefined then (
					core = ""
				)
				core = "core_mesh=\"" + core + "\" "
			)
			local static = (getUserProp gObj "ColliderCStatic") != undefined
			local kinematic = (getUserProp gObj "ColliderCKinematic") != undefined
			format "\t\t<collider collider_type=\"%\" %static=\"%\" kinematic=\"%\" />\n" type core (TrueFalse static) (TrueFalse kinematic) to: file
		)
		
		exportComp = getUserProp gObj "ExportTriggerC"
		if exportComp != undefined then (
			local type = getUserProp gObj "TriggerCType"
			type = GetValidColliderTypeExportString type
			local core = ""
			if type == "TriangleMesh" or type == "ConvexMesh" then (
				core = getUserProp gObj "export_core_name"
				if core == undefined then (
					core = ""
				)
				core = "core_mesh=\"" + core + "\" "
			)
			local static = (getUserProp gObj "TriggerCStatic") != undefined
			local tScale = (getUserProp gObj "TriggerCScale")
			if tScale == undefined then (
				tScale = "1 1 1"
			)
			local tOff = (getUserProp gObj "TriggerCOffset")
			if tOff == undefined then (
				tOff = "0 0 0"
			)
			format "\t\t<trigger collider_type=\"%\" %static=\"%\" scale=\"%\" offset=\"%\" />\n" type core (TrueFalse static) tScale tOff to: file
		
		)
		
		exportComp = getUserProp gObj "ExportCameraC"
		if exportComp != undefined then (
			local cOff = (getUserProp gObj "CameraCOffset")
			if cOff == undefined then (
				cOff = "0 0 0"
			)
			format "\t\t<fps_camera offset=\"%\" />\n" to: file
		)
		
		exportComp = getUserProp gObj "ExportCharContrC"
		if exportComp != undefined then (
			local cOff = (getUserProp gObj "CharContrCOffset")
			if cOff == undefined then (
				cOff = "0 0 0"
			)
			local radius = getUserProp gObj "CharContrCRadius"
			local height = getUserProp gObj "CharContrCHeight"
			format "\t\t<character_controller radius=\"%\" height=\"%\" offset=\"%\" />\n" radius height cOff to: file
		)
		
		exportComp = getUserProp gObj "ExportSpeakerC"
		if exportComp != undefined then (
			local vol = getUserProp gObj "SpeakerCVolume"
			format "\t\t<speaker volume=\"%\" />\n" vol to: file
		)
		
		exportComp = getUserProp gObj "ExportParticleSC"
		if exportComp != undefined then (
			local peClass = getUserProp gObj "ParticleSCClass"
			format "\t\t<particle_emitter class=\"%\" />\n" peClass to: file
		)
		
		exportComp = getUserProp gObj "ExportScripts"
		if exportComp != undefined then (
			local scripts = execute exportComp
			for i = 1 to scripts.count do
			(
				format "\t\t<scripted_component class=\"%\">\n" scripts[i].name to: file
				
				local props = scripts[i].props
				for j = 1 to props.count do
				(
					if props[j].type == "array" then
					(
						format "\t\t\t<property name=\"%\" type=\"array\">\n" props[j].name to: file
						local vals = props[j].val
						for k = 1 to vals.count do
						(
							format "\t\t\t\t<entry type=\"%\" value=\"%\" />\n" vals[k].type vals[k].val to: file
						)
						format "\t\t\t</property>\n" to: file
					)
					else
					(
						format "\t\t\t<property name=\"%\" type=\"%\" value=\"%\" />\n" props[j].name props[j].type props[j].val to: file
					)
				)
				
				format "\t\t</scripted_component>\n" to: file
			)
		)

		
		format "\t</object>\n\n" to: file
	)
	format "</scene>\n" to:file
	MyCloseFile()
	
	format "____Created Scene File %____\n" sceneName
)

fn createRenderableObjectsFile instances = (
	MyCreateFile "renderable_objects.xml"
	
	WriteXMLPrologToFile()
	format "<renderable_objects>\n" to:file
	
	for i = 1 to instances.count do
	(
		local Pos = TranslationToRH (instances[i].transform.translation)
		local Sc = ScaleToRH instances[i].transform.scale
		local Yaw = GetYaw (instances[i].transform)
		local Pitch = GetPitch (instances[i].transform)
		local Roll = GetRoll (instances[i].transform)
		local core = getUserProp instances[i] "export_core_name"
		local PhysxType = getUserProp instances[i] "physics_generate"
		local separatorEnd = ""
		if PhysxType == "undefined" then
		(
			separatorEnd = "/"
		)
		format "\t<mesh_instance name=\"%\" core_name=\"%\" pos=\"% % %\" scale=\"% % %\" yaw=\"%\" pitch=\"%\" roll=\"%\" visible=\"%\"%>\n"\
					instances[i].name core Pos[1] Pos[2] Pos[3] Sc[1] Sc[2] Sc[3] Yaw Pitch Roll instances[i].visibility separatorEnd to:file
		
		
		if PhysxType != "undefined" then
		(
			local trigger = getUserProp instances[i] "is_trigger" 
			local anim = getUserProp instances[i] "cinematics_export"
			local kinematic = "false"
			local static =  getUserProp instances[i] "physics_static"
			if anim == "true" then
			(
				static = "false"
				kinematic = "true"
			)
			format "\t\t<physx collider_type=\"%\" static=\"%\" kinematic=\"%\" trigger=\"%\" />\n"\
					PhysxType static kinematic trigger to:file
			format "\t</mesh_instance>\n" to:file
		)	
		
		
	)

	format "</renderable_objects>\n" to:file 
	format "____CreatedRenderableObjects____\n"
	
	MyCloseFile()
)

fn createLogic instances = (
	MyCreateFile "logic.xml"
	
	WriteXMLPrologToFile()
	format "<logic>\n" to:file
	
	for i = 1 to instances.count do
	(	
		local l_logicType = getUserProp instances[i] "logic_type"
		if l_logicType != "" then
		(				
			
			local l_logicActivates = getUserProp instances[i] "logic_activates"
			if l_logicActivates != "" then 
			(
				format "\t<% name=\"%\" activates=\"%\"/>\n"\
				l_logicType instances[i].name l_logicActivates to:file
			)
			else
			(
				if l_logicType == "SpawnPlayer" then
				(
					local Pos = TranslationToRH (instances[i].transform.translation)
					local Yaw = GetYaw (instances[i].transform)
					local Pitch = GetPitch (instances[i].transform)
					local Roll = GetRoll (instances[i].transform)
					format "\t<% name=\"%\"  pos=\"% % %\"  yaw=\"%\" pitch=\"%\" roll=\"%\" />\n"\
					l_logicType instances[i].name  Pos[1] Pos[2] Pos[3] Yaw Pitch Roll to:file
				)
				else
				(
					format "\t<% name=\"%\" />\n"\
					l_logicType instances[i].name to:file
				)
			)
			
		)		
	)

	format "</logic>\n" to:file 
	format "____CreatedLogic____\n"
	
	MyCloseFile()
)

fn createTriggersFile instances = (
	MyCreateFile "Triggers.xml"
	WriteXMLPrologToFile()
	format "<triggers>\n" to:file
	
	for i = 1 to instances.count do
	(
		if getUserProp instances[i] "is_trigger" == true then
		(
			local l_arr_boundingBox = nodeLocalBoundingBox instances[i]
			local l_sizeBoundingBox = l_arr_boundingBox[2] - l_arr_boundingBox[1]
			local Pos = TranslationToRH (instances[i].transform.translation)
			local Sc = ScaleToRH instances[i].transform.scale
			local Yaw = GetYaw (instances[i].transform)
			local Pitch = GetPitch (instances[i].transform)
			local Roll = GetRoll (instances[i].transform)
			local l_trigger_active_once = getUserProp instances[i] "trigger_active_once"
			
			format "\t<mesh_instance name=\"%\" ActivateTriggerOnce=\"%\" pos=\"% % %\" scale=\"% % %\" rotation=\"% % %\" />\n"\
							instances[i].name l_trigger_active_once  Pos[1] Pos[2] Pos[3] l_sizeBoundingBox[1] l_sizeBoundingBox[3] l_sizeBoundingBox[2] Yaw Pitch Roll  to:file
	
		)
		
	)
	format "</triggers>\n" to:file 
	format "____CreatedTriggers____\n"
	MyCloseFile()
)

