----------------------------------------------------------------------------
----------------------------------------------------------------------------
-- Export utilities
----------------------------------------------------------------------------
----------------------------------------------------------------------------

fn GetCores = (
	-- TODO use only the selected ones
	local ret = #()
	for i = 1 to $*.count do
	(
		if superclassof $*[i] == GeometryClass and classof $*[i] != Targetobject and not $*[i].isHidden then
		(
			append ret $*[i]
		)
	)
	return ret
)

fn GetInstances = (
	-- TODO use the real instances!
	return GetCores()
)

fn GetAnimatedInstances = (
	local ret = #()
	for i = 1 to $*.count do
	(
		local l_ExportCinematics = getUserProp $*[i] "cinematics_export"
		if l_ExportCinematics != undefined and l_ExportCinematics then
		(
			append ret $*[i]
		)
	)
	return ret
)

fn GetLights = (
	light_obj = #()
	for i =1 to $*.count do
	(
		-- TODO: check userProp to export light
		if SuperclassOf $*[i] == light then
		(
			append light_obj $*[i]
			-- format "\t<light =\"%\"/>" $*[i]
		)
	)
	-- format "<light_obj =\"%\"/>" light_obj
	return light_obj
)

fn Deg2Rad a =
(
	return a * PI / 180
)

fn GetYaw transform =
(
	return -(transform as eulerangles).z
)

fn GetPitch transform =
(
	return -(transform as eulerangles).y
)

fn GetRoll transform =
(
	return (transform as eulerangles).x
) 

fn ChangeCoordsBase coords =
(
	if classof coords == EulerAngles then
	(
		return eulerAngles  (Deg2Rad -coords.z) (Deg2Rad -coords.y) (Deg2Rad coords.x)
	)
	return point3 -coords[1] coords[3] coords[2]
)

fn TranslationToRH translation =
(
	return ChangeCoordsBase translation
)
