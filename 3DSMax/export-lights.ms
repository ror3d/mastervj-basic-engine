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
			local type = "omni"
			local concpos = l.pos[1] as string + " " + l.pos[2]as string + " " + l.pos[3] as string
			local rgba = l.rgb/255
			local concrgba = rgba.r as string + " " + rgba.g as string + " " + rgba.b as string + " " + rgba.a as string
			
			format "\t<light name=\"%\" type=\"%\" pos=\"%\" color=\"%\" att_start_range=\"%\" att_end_range=\"%\" intensity=\"%\"/>\n" \
					l.name type concpos concrgba l.farAttenStart l.farAttenEnd l.multiplier to:file
		
		)
		else if classof l == TargetDirectionallight then
		(
			-- Direcional (targetDirect)
			local dir=normalize(l.target.pos-l.pos)
			local type = "directional"
			local concpos = l.pos[1] as string + " " + l.pos[2]as string + " " + l.pos[3] as string
			local concdir = dir[1] as string + " " + dir[2]as string + " " + dir[3] as string
			local rgba = l.rgb/255
			local concrgba = rgba.r as string + " " + rgba.g as string + " " + rgba.b as string + " " + rgba.a as string
			
			format "\t<light name=\"%\" type=\"%\" pos=\"%\" dir=\"%\" color=\"%\" att_start_range=\"%\" att_end_range=\"%\" intensity=\"%\"/>\n" \
					l.name type concpos concdir concrgba l.farAttenStart l.farAttenEnd l.multiplier to:file
		
		)
		else if classof l == targetSpot then
		(
			--rmat $.pos
			--Spot  (targetSpot)
			local dir=normalize(l.target.pos-l.pos)
			local type = "spot"
			local lightangle= (l.hotspot*pi)/180
			local lightfalloff = (l.falloff*pi)/180
			local concpos = l.pos[1] as string + " " + l.pos[2]as string + " " + l.pos[3] as string
			local concdir = dir[1] as string + " " + dir[2]as string + " " + dir[3] as string
			local rgba = l.rgb/255
			local concrgba = rgba.r as string + " " + rgba.g as string + " " + rgba.b as string + " " + rgba.a as string
			
			format "\t<light name=\"%\" type=\"%\" pos=\"%\" dir=\"%\" color=\"%\" angle=\"%\" falloff=\"%\" att_start_range=\"%\" att_end_range=\"%\" intensity=\"%\"/>\n" \
					l.name type concpos concdir concrgba lightangle lightfalloff l.farAttenStart l.farAttenEnd l.multiplier to:file
		)
	)
	
	format "</lights>\n" to:file
	format "____ExportedLights____\n"
	
	MyCloseFile()	
)
