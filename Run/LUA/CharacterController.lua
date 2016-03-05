m_activeMovAnim = false

function CharTest()
	Strafe = 0
	Forward = 0
	
	elapsedTime = timeManager:GetElapsedTime()	

	if CInputManager:GetAxis("STATICMOUSEAxis") ~= 1 then --TECLA F CONGELA MOV		
		velMultiplier = 0.1
		Strafe = CInputManager:GetAxis("STRAFE")
		Forward = CInputManager:GetAxis("MOVE_FWD")
		m_Yaw = fpsCamera:GetYaw()

		camX = (Forward*(math.cos(m_Yaw)) + Strafe*(math.cos(m_Yaw + math.pi*0.5)))*velMultiplier
		camZ = (Forward*(math.sin(m_Yaw)) + Strafe*(math.sin(m_Yaw + math.pi*0.5)))*velMultiplier
		camY = CInputManager:GetAxis("JUMPAxis")*velMultiplier

		local cameraPosition = Vect3f.new(camX, camY, camZ) --pasarle values
		cameraPosition = CPhysicsManager:moveCharController(cameraPosition, fpsCamera:GetUp(), elapsedTime, "main")
		fpsCamera:SetPosition(cameraPosition)
	
		fpsCamera:Update(elapsedTime)
		objectModel:SetPosition(cameraPosition)
		objectModel:SetYaw(-m_Yaw + math.pi*0.5) --DEFORMED
	end
	
	if Strafe == 0 and Forward == 0 then
		inMovement = false	
	else
		inMovement = true
	end
	
	if inMovement and m_activeMovAnim then	--To Idle
		animatedModel:ClearCycle(0, 0.05)
		animatedModel:BlendCycle(1, 1.0, 0.2)
		m_activeMovAnim = false		
	elseif not inMovement and not m_activeMovAnim then	
		animatedModel:ClearCycle(1, 0.2)
		animatedModel:BlendCycle(0, 1.0, 0.4)
		
		m_activeMovAnim = true
	end
end
