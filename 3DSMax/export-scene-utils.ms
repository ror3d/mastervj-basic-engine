----------------------------------------------------------------------------
----------------------------------------------------------------------------
-- Export utilities
----------------------------------------------------------------------------
----------------------------------------------------------------------------
global g_allCores;
global g_allInstances;
fn CollectCoresAndInstances = (
	g_allCores = #()
	g_allInstances = #()
	for i = 1 to $*.count do
	(
		if superclassof $*[i] == GeometryClass and classof $*[i] != Targetobject then
		(
			local core = getUserProp $*[i] "render_export_rend_obj"
			if core != undefined then
			(
				append g_allInstances $*[i]
				
				local isCore = true
				for j = 1 to g_allCores.count do
				(
					if areNodesInstances g_allCores[j] $*[i] then
					(
						setUserProp $*[i] "export_core_name" g_allCores[j].name
						isCore = false
						break
					)
				)
				if isCore then
				(
					append g_allCores $*[i]
				)
			)
		)
	)
)

fn GetCores = (
	
	return g_allCores
)

fn GetInstances = (
	return g_allInstances
)


fn deleteUserProp node prop = if getUserProp node prop != undefined do
(
	buff = (getUserPropBuffer node) as stringStream 
	newb = stringStream ""
	while not eof buff do
	(
		str = readLine buff
		if str != "" and not matchpattern str pattern:("*" + prop + "*=*") do format "%\n" str to:newb
	)
	setUserPropBuffer node (replace_LF_with_CRLF (newb as string))
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
