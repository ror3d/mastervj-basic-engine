----------------------------------------------------------------------------
----------------------------------------------------------------------------
-- Export utilities
----------------------------------------------------------------------------
----------------------------------------------------------------------------

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

global g_allCores;
global g_allInstances;
global g_allGameObjects;

fn ClearCoreNames = (
	g_allCores = #()
	g_allInstances = #()
	g_allGameObjects = #()
	
	for i = 1 to $*.count do
	(
		if superclassof $*[i] == GeometryClass and classof $*[i] != Targetobject then
		(
			deleteUserProp $*[i] "export_core_name"
		)
	)
)

fn CollectCoresAndInstances = (
	g_allCores = #()
	g_allInstances = #()
	g_allGameObjects = #()
	
	-- Collect all the existing cores
	for i = 1 to $*.count do
	(
		if superclassof $*[i] == GeometryClass and classof $*[i] != Targetobject then
		(
			local l_thisCoreName = getUserProp $*[i] "export_core_name"
			
			-- If we have a name assigned, keep the instance as core
			if l_thisCoreName != undefined then
			(
				local isCore = true
				for j = 1 to g_allCores.count do
				(
					if areNodesInstances g_allCores[j] $*[i] then
					(
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
	
	-- Collect the rest of the geometry and cores
	for i = 1 to $*.count do
	(
		if superclassof $*[i] == GeometryClass and classof $*[i] != Targetobject then
		(
			local gameObject = getUserProp $*[i] "ExportGameObject"
			
			if gameObject != undefined then
			(
				append g_allGameObjects $*[i]
			)
			
			local core = getUserProp $*[i] "export_mesh_instance"
			if core != undefined then
			(
				append g_allInstances $*[i]
				
				local l_thisCoreName = getUserProp $*[i] "export_core_name"
				
				local isCore = true
				for j = 1 to g_allCores.count do
				(
					if areNodesInstances g_allCores[j] $*[i] then
					(
						local l_coreName = getUserProp g_allCores[j] "export_core_name"
						if l_thisCoreName == undefined or l_thisCoreName != l_coreName then
						(
							setUserProp $*[i] "export_core_name" l_coreName
						)
						isCore = false
						break
					)
				)
				if isCore then
				(
					local l_thisCoreName = getUserProp $*[i] "export_core_name"
					if l_thisCoreName == undefined then
					(
						setUserProp $*[i] "export_core_name" $*[i].name
					)
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

fn GetGameObjects = (
	return g_allGameObjects
)


fn GetAnimatedInstances = (
	local ret = #()
	for i = 1 to $*.count do
	(
		local l_ExportCinematics = getUserProp $*[i] "ExportTAnimation"
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
		local export = getUserProp $*[i] "ExportAsLight"
		if export != undefined then (
			if SuperclassOf $*[i] == light then
			(
				append light_obj $*[i]
				-- format "\t<light =\"%\"/>" $*[i]
			)
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
	local eu = quatToEuler transform.rotation order: 4
	return eu.z
	-- return -(transform.rotation as eulerangles).z
)

fn GetPitch transform =
(
	local eu = quatToEuler transform.rotation order: 4
	return -eu.y
	-- return -(transform.rotation as eulerangles).y
)

fn GetRoll transform =
(
	local eu = quatToEuler transform.rotation order: 4
	return eu.x
	-- return (transform.rotation as eulerangles).x
) 

fn ChangeCoordsBase coords =
(
	if classof coords == EulerAngles then
	(
		return eulerAngles  -coords.z -coords.y coords.x
		format "ERROR!!! You should not use this for rotations!!!"
	)
	else if classof coords == Quat then
	(
		return quat coords.x -coords.z -coords.y coords.w
	)
	return point3 -coords[1] coords[3] coords[2]
)

fn TranslationToRH translation =
(
	return ChangeCoordsBase translation
)

fn ScaleToRH sc =
(
	return point3 sc[1] sc[3] sc[2]
)

fn TrueFalse val = (
	if val then (
		return "true"
	) else (
		return "false"
	)
)

global g_tick
fn tik = (
	g_tick = timeStamp()
	return OK
)

fn tok s:"" = (
	local end = timeStamp()
	format "$ % seconds elapsed for task %\n" ((end - g_tick) / 1000.0) s
)

