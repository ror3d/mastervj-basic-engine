----------------------------------------------------------------------------
----------------------------------------------------------------------------
-- Paths
----------------------------------------------------------------------------
----------------------------------------------------------------------------
global absPath = maxFilePath
global DataPath = "Data\\"
global MeshSubPath = "Meshes\\"
global TextureSubPath = "Textures\\"
global AnimationSubPath = "Animations\\"
global SceneSubPath = "Scenes\\"

fn getDataPath p = (
	return DataPath + p
)

fn getMeshPath p = (
	return getDataPath (MeshSubPath + p)
)

fn getTexturePath p = (
	return getDataPath (TextureSubPath + p)
)

fn getAnimationPath p = (
	return getDataPath (AnimationSubPath + p)
)

fn getScenePath p = (
	return getDataPath (SceneSubPath + p)
)

fn setAbsolutePath p = (
	absPath = p
	if absPath[absPath.count] != "\\" then
	(
		absPath = absPath + "\\"
	)
)

fn getAbsolutePath p = (
	return absPath + p
)

fn getAbsoluteDataPath p = (
	return absPath + (getDataPath p)
)

fn getAbsTexturePath p = (
	return absPath + (getTexturePath p)
)

fn getAbsMeshPath p = (
	return absPath + (getMeshPath p)
)

fn getAbsAnimationPath p = (
	return absPath + (getAnimationPath p)
)

fn getAbsScenePath p = (
	return absPath + (getScenePath p)
)

fn createDirs = (
	if not doesFileExist (getAbsoluteDataPath("")) then
	(
		makeDir (getAbsoluteDataPath("")) all:true
	)
	if not doesFileExist (getAbsTexturePath("")) then
	(
		makeDir (getAbsTexturePath(""))
	)
	if not doesFileExist (getAbsMeshPath("")) then
	(
		makeDir (getAbsMeshPath(""))
	)
	if not doesFileExist (getAbsAnimationPath("")) then
	(
		makeDir (getAbsAnimationPath(""))
	)
	if not doesFileExist (getAbsScenePath("")) then
	(
		makeDir (getAbsScenePath(""))
	)
)


----------------------------------------------------------------------------
----------------------------------------------------------------------------
-- File functions
----------------------------------------------------------------------------
----------------------------------------------------------------------------
if file != undefined then
(
	if fileFromFOpen then
	(
		fflush file
		fclose file
	)
	else
	(
		close file
	)
)
global file = undefined
global fileFromFOpen = undefined

fn MyCloseFile = (
	if file != undefined then
	(
		if fileFromFOpen then
		(
			fflush file
			fclose file
		)
		else
		(
			close file
		)
		file = undefined
	)
)

fn MyCreateFile path = (
	MyCloseFile()
	createDirs()
	file = createfile (getAbsoluteDataPath(path))
	fileFromFOpen = false
)

fn MyFOpen path mode = (
	MyCloseFile()
	createDirs()
	file = fopen (getAbsoluteDataPath(path)) mode
	fileFromFOpen = true
)

fn WriteXMLPrologToFile = (
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
)
