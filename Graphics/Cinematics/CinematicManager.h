#ifndef CINEMATIC_MANAGER_H
#define CINEMATIC_MANAGER_H

#include <string>
#include <vector>

class CCameraKeyController;
class CCinematicObject;

class CCinematicManager
{
public:
	CCinematicManager();
	~CCinematicManager();

	void LoadFilesInDir( std::string dirPath );
	void Load(std::string nameFile);
	void Play();
	void Play(std::string nameToRun);
	void Pause(std::string nameToRun);
	void Stop(std::string nameToRun);
	void destroy();
	void Update(float _elapsedTime);

private:
	std::vector<CCameraKeyController*> m_cinematicsCameras;
	std::vector<CCinematicObject*> m_cinematicsObjects;

};



#endif