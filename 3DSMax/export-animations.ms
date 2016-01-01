fileIn "file-utils.ms"

fn ExportCameraAnimation cam =
(
	format "____Exporting animation for camera %____\n" cam.name
	MyCreateFile ( AnimationSubPath + cam.name + ".xml" )
	
	WriteXMLPrologToFile()
	
	format "<camera_key_controller name=\"%\">\n" cam.name to:file
	
	local first_kf = 0f
	-- Get First keyframe
	
	if ( at time 1 trackbar.getPreviousKeyTime() ) > 0 then
	(
		first_kf = at time 0 trackbar.getNextKeyTime()
	)
	
	local fps = 1s
	fps = fps.frame
	
	local current_kf = first_kf
	do (
		local transf = at time current_kf cam.transform
		local pos = transf.pos
		local eye = [0, 0, -1] * (transf.rotation as matrix3)
		local up = [0, 1, 0] * (transf.rotation as matrix3)
		local fov = at time current_kf cam.fov
		local near = at time current_kf cam.nearrange
		local far = at time current_kf cam.farrange
		
		local c_time = current_kf.frame / fps
		format "\t<key time=\"%\" pos=\"% % %\" look_at=\"% % %\" up_vector=\"% % %\" fov=\"%\" near_plane=\"%\" far_plane=\"%\"/>\n"\
			c_time pos[1] pos[2] pos[3] eye[1] eye[2] eye[3] up[1] up[2] up[3] fov near far to: file
		
		current_kf = at time current_kf trackbar.getNextKeyTime()
	) while current_kf != first_kf
	
	format "</camera_key_controller>\n" to: file
	MyCloseFile()
	format "____Exported animation for camera %____\n" cam.name
)

fn ExportObjectTrackAnimation obj =
(
	
)
