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
		if superclassof $*[i] == GeometryClass and classof $*[i] != Targetobject then
		(
			append ret $*[i]
		)
	)
	return ret
)

fn GetInstances = (
	return $*
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
	return point3 -coords.x coords.z coords.y
)

fn TranslationToRH translation =
(
	return ChangeCoordsBase translation
)
