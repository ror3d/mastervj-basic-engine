fileIn "file-utils.ms"


fn GetLights = (
	light_obj = #()
	for i =1 to $*.count do
	(
		if SuperclassOf $*[i] == light then
		(
			append light_obj $*[i]
			format "\t<light =\"%\"/>" $*[i]
		)
	)
	format "<light_obj =\"%\"/>" light_obj
	return light_obj --Mirar el classof
)


fn createLights = (
	MyCreateFile "lights.xml"
	
	local all_lights = GetLights()
	
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<lights>\n" to:file
	
	for i=1 to all_lights.count do
	(
		if  classof all_lights[i] == Omnilight then
		(
			-- Omni (omni)
			format "\t<light name=\"%\" type=\"%\" pos=\"%\" color=\"%\" att_start_range=\"0.0\" att_end_range=\"0.0\" intensity=\"%\"/>\n" all_lights[i].name all_lights[i].type all_lights[i].pos all_lights[i].rgb all_lights[i].farAttenStart all_lights[i].farAttenEnd all_lights[i].multiplier to:file
		
		)
		else if classof all_lights[i] == TargetDirectionallight then
		(
			-- Direcional (targetDirect)
			local dir=($.target.pos-$.pos)
			format "\t<light name=\"%\" type=\"%\" pos=\"%\" dir=\"%\" color=\"%\" att_start_range=\"0.0\" att_end_range=\"0.0\" intensity=\"%\"/>\n" all_lights[i].name all_lights[i].type all_lights[i].pos dir all_lights[i].rgb all_lights[i].farAttenStart all_lights[i].farAttenEnd all_lights[i].multiplier to:file
		
		)
		else if classof all_lights[i] == targetSpot then
		(
			--Spot  (targetSpot)
			local dir=($.target.pos-$.pos)
			format "\t<light name=\"%\" type=\"%\" pos=\"%\" dir=\"%\" color=\"%\" att_start_range=\"0.0\" att_end_range=\"0.0\" intensity=\"%\"/>\n" all_lights[i].name all_lights[i].type all_lights[i].pos dir all_lights[i].rgb all_lights[i].farAttenStart all_lights[i].farAttenEnd all_lights[i].multiplier to:file
		)
	)
	
	format "</lights>\n" to:file
	format "____CreatedRenderableObjects____\n"
	
	MyCloseFile()	
)
