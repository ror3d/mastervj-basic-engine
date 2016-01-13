fileIn "file-utils.ms"
fileIn "export-scene-utils.ms"



fn createLights = (
	format "____ExportingLights____\n"
	MyCreateFile "lights.xml"
	
	local all_lights = GetLights()
	
	WriteXMLPrologToFile()
	format "<lights>\n" to:file
	
	for i=1 to all_lights.count do
	(
		local l = all_lights[i]
		if  classof l == Omnilight then
		(
			-- Omni (omni)
			format "\t<light name=\"%\" type=\"%\" pos=\"%\" color=\"%\" att_start_range=\"0.0\" att_end_range=\"0.0\" intensity=\"%\"/>\n" \
					l.name l.type l.pos l.rgb l.farAttenStart l.farAttenEnd l.multiplier to:file
		
		)
		else if classof l == TargetDirectionallight then
		(
			-- Direcional (targetDirect)
			local dir=($.target.pos-$.pos)
			format "\t<light name=\"%\" type=\"%\" pos=\"%\" dir=\"%\" color=\"%\" att_start_range=\"0.0\" att_end_range=\"0.0\" intensity=\"%\"/>\n" \
					l.name l.type l.pos dir l.rgb l.farAttenStart l.farAttenEnd l.multiplier to:file
		
		)
		else if classof l == targetSpot then
		(
			--Spot  (targetSpot)
			local dir=($.target.pos-$.pos)
			format "\t<light name=\"%\" type=\"%\" pos=\"%\" dir=\"%\" color=\"%\" att_start_range=\"0.0\" att_end_range=\"0.0\" intensity=\"%\"/>\n" \
					l.name l.type l.pos dir l.rgb l.farAttenStart l.farAttenEnd l.multiplier to:file
		)
	)
	
	format "</lights>\n" to:file
	format "____ExportedLights____\n"
	
	MyCloseFile()	
)
