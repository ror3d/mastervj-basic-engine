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
		
		local exportFreeCam = getUserProp gObj "ExportCameraC"
		if exportFreeCam != undefined and classof(gObj) == Targetcamera then
		(
			local tgt = gObj.target
			Pos = TranslationToRH (tgt.transform.translation)
		)
		
		local disabled = getUserProp gObj "ExportGODisabled"
		if disabled != undefined then
		(
			disabled = (disabled == true) or (disabled == "true")
		)
		else
		(
			disabled = false
		)

		format "\t<object name=\"%\" pos=\"% % %\" scale=\"% % %\" rotation=\"% % %\" enabled=\"%\">\n"\
					gObj.name Pos[1] Pos[2] Pos[3] Sc[1] Sc[2] Sc[3] Yaw Pitch Roll (TrueFalse (not disabled)) to: file
		
		local exportSpawn = getUserProp gObj "ExportAsSpawn"
		if exportSpawn != undefined then (
			local nameSkinnedMeshCore = getUserProp gObj "SkinnedMeshCore"
			format "\t\t<spawn name_to_spawn=\"%\" />\n" nameSkinnedMeshCore to: file
		)
		
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
				local playAtStart = getUserProp gObj "PlayAtStartSkinnedMesh"
				local startTime = -1
				if playAtStart != undefined then
				(
					startTime = 0
				)
				format "\t\t<animated_instance core=\"%\" layers=\"%\" visible=\"%\" startTime=\"%\" />\n" core layers (TrueFalse gObj.visibility) startTime to: file
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
		
		if exportFreeCam != undefined then (
			local cOff = "0 0 0"
			local fwd = "1 0 0"
			local fov = ""
			
			if classof(gObj) == Targetcamera then
			(
				local tgt = gObj.target
				local ss = stringstream ""
				local v = ChangeCoordsBase(gObj.position - tgt.position)
				format "% % %" v.x v.y v.z to: ss
				cOff = ss as string
				
				ss = stringstream ""
				v = - normalize v
				format "% % %" v.x v.y v.z to: ss
				fwd = ss as string
				
				local exportFov = getUserProp gObj "ExportCameraCFov"
				if exportFov != undefined then (
					ss = stringstream ""
					format "fov=\"%\" " gObj.fov to:ss
					fov = ss as string
				)
			)
			
			format "\t\t<free_camera forward=\"%\" offset=\"%\" %/>\n" fwd cOff fov to: file
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
			local collide = getUserProp gObj "ParticleSCCollisions"
			local collisions_step = ""
			if collide != undefined and (collide as integer) != 0 then
			(
				collisions_step = "collisions_step=\"" + (collide as string) + "\" "
			)
			format "\t\t<particle_emitter class=\"%\" %/>\n" peClass collisions_step to: file
		)
		
		exportComp = getUserProp gObj "ExportTAnimation"
		if exportComp != undefined  and exportComp == true then (
			local cinem_playAtStart = getUserProp gObj "TAnimationPlayStart"
			if cinem_playAtStart == undefined then
			(
				cinem_playAtStart = false
			)
			format "\t\t<cinematic name=\"%\" playAtStart=\"%\" />\n" gObj.name cinem_playAtStart to: file
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

