----------------------------------------------------------------------------
----------------------------------------------------------------------------
-- Export utilities
----------------------------------------------------------------------------
----------------------------------------------------------------------------

fn GetCores = (
	-- TODO
	return $*
)

fn GetInstances = (
	return $*
)

fn GetAnimatedInstances = (
	-- TODO
	return $*
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
