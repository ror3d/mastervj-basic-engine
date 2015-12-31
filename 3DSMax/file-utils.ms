----------------------------------------------------------------------------
----------------------------------------------------------------------------
-- Paths
----------------------------------------------------------------------------
----------------------------------------------------------------------------
global absPath = maxFilePath
global DataPath = "Data\\"
global MeshSubPath = "Meshes\\"
global TextureSubPath = "Textures\\"

fn getDataPath p = (
	return DataPath + p
)

fn getMeshPath p = (
	return getDataPath (MeshSubPath + p)
)

fn getTexturePath p = (
	return getDataPath (TextureSubPath + p)
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
)


----------------------------------------------------------------------------
----------------------------------------------------------------------------
-- File functions
----------------------------------------------------------------------------
----------------------------------------------------------------------------
file = undefined
fileFromFOpen = undefined

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
