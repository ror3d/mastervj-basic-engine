fileIn "file-utils.ms"
fileIn "export-scene-utils.ms"

fn getControllers obj ctrls:#() =
(
    for item in refs.dependsOn obj do   
    (
        if isController item AND item.keyable do
            append ctrls item

        getControllers item ctrls:ctrls
    )
    ctrls
)

fn getAllKeys controllers =
(
	local realControllers = #()
	
	for i = 1 to controllers.count do
	(
		local ctrls = #()
		getControllers controllers[i] ctrls:ctrls
		if (isController controllers[i]) and controllers[i].keyable then
		(
			append ctrls controllers[i]
		)
		realControllers = realControllers + ctrls
	)
	
	local keys = #()
	for i = 1 to realControllers.count do
	(
		for j = 1 to numkeys realControllers[i] do
		(
			append keys (getkeytime realControllers[i] j)
		)
	)
	makeUniqueArray keys
	return sort keys
)

fn ExportCameraAnimation cam =
(
	format "____Exporting animation for camera %____\n" cam.name
	MyCreateFile ( AnimationSubPath + cam.name + ".xml" )
	
	WriteXMLPrologToFile()
	
	format "<camera_key_controller name=\"%\">\n" cam.name to:file
	
	local keys = getAllKeys #(cam)
		
	if keys.count == 0 then
	(
		keys = #(0f)
	)
	
	local fps = 1s
	fps = fps.frame
	
	for i = 1 to keys.count do
	(
		local current_kf = keys[i]
		local transf = at time current_kf cam.transform
		local pos = ChangeCoordsBase transf.pos
		local eye = ChangeCoordsBase ([0, 0, -1] * (transf.rotation as matrix3))
		local up = ChangeCoordsBase ([0, 1, 0] * (transf.rotation as matrix3))
		local fov = at time current_kf cam.fov
		local near = at time current_kf cam.nearrange
		local far = at time current_kf cam.farrange
		
		local c_time = current_kf.frame / fps
		-- TODO Change coordinate system
		format "\t<key time=\"%\" pos=\"% % %\" look_at=\"% % %\" up_vector=\"% % %\" fov=\"%\" near_plane=\"%\" far_plane=\"%\"/>\n"\
			c_time pos[1] pos[2] pos[3] eye[1] eye[2] eye[3] up[1] up[2] up[3] fov near far to: file
		
	)
	
	format "</camera_key_controller>\n" to: file
	MyCloseFile()
	format "____Exported animation for camera %____\n" cam.name
)

fn ExportAllObjectCinematics =
(
	local fname = (getFilenameFile maxFileName)
	format "____Exporting animation tracks file %.xml____\n" fname
	MyCreateFile ( AnimationSubPath + fname + ".xml" )
	
	WriteXMLPrologToFile()
	
	local objs = GetAnimatedInstances()
	
	local fps = 1s
	fps = fps.frame
	
	local endTime = animationrange.end.frame / fps
	
	format "<cinematic name=\"%\" duration=\"%\">\n" fname endTime to: file
	
	
	for i = 1 to objs.count do
	(
		local obj = objs[i]
		
		format "\t____Exporting animation tracks for object %____\n" obj.name
		
		if (superclassof obj) != GeometryClass then
		(
			format "\n$$$$ Alert: % should not be included in the track animation export list!\n\n" obj.name
			continue
		)
		
		format "\t<cinematic_object resource=\"%\">\n" obj.name to: file
		local keys = getAllKeys #(obj.position.controller, obj.rotation.controller, obj.scale.controller)
		
		if keys.count == 0 then
		(
			keys = #(0f)
		)
		append keys (endTime as Time)
		keys = makeUniqueArray keys
		
		for i = 1 to keys.count do
		(
			local t = keys[i].frame / fps
			local transf = at time keys[i] obj.transform
			local pos = ChangeCoordsBase transf.position
			local rot = ChangeCoordsBase(transf as eulerAngles)
			-- TODO: should change coordinates?
			local sca = transf.scale
			format "\t\t<cinematic_object_key_frame time=\"%\" pos=\"% % %\" rotation=\"% % %\" scale=\"% % %\" />\n"\
					t pos[1] pos[2] pos[3] rot.x rot.y rot.z sca[1] sca[2] sca[3] to: file
		)
		format "\t</cinematic_object>\n" to: file
	)
	
	format "</cinematic>\n" to: file
	
	MyCloseFile()
	format "____Exported animation tracks file____\n"
)
