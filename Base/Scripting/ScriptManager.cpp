#include "ScriptManager.h"

#include <selene.h>

#include <fstream>
#include <sstream>

#include <Core/Engine/Engine.h>

/*
 * This class captures errors printed by lua to redirect them as we wish.
 */
class LuaErrorCapturedStdout {
public:
	LuaErrorCapturedStdout() {
		_old = std::cout.rdbuf(_out.rdbuf());
	}

	~LuaErrorCapturedStdout() {
		std::cout.rdbuf(_old);
		OutputDebugStringA(_out.str().c_str());
	}

	std::string Content() const {
		return _out.str();
	}

private:
	std::stringstream _out;
	std::streambuf *_old;
};

CScriptManager::CScriptManager()
	: m_state(nullptr)
{
}


CScriptManager::~CScriptManager()
{
	if (m_state)
	{
		delete m_state;
	}
}

void CScriptManager::Initialize()
{
	LuaErrorCapturedStdout errorCapture;
	m_state = new sel::State{ true };
}

void CScriptManager::Destroy()
{
}

void CScriptManager::RunCode(const std::string &code)
{
	LuaErrorCapturedStdout errorCapture;
	(*m_state)(code.c_str());
}

void CScriptManager::RunFile(const std::string &fileName)
{
	LuaErrorCapturedStdout errorCapture;
	std::ifstream f(fileName);
	std::string code((std::istreambuf_iterator<char>(f)),
					std::istreambuf_iterator<char>());
	(*m_state)(code.c_str());
}

void CScriptManager::Load(const std::string &xmlFile)
{
	LuaErrorCapturedStdout errorCapture;
	// TODO
}


class TestClass {
public:
	float x;
	std::string y;
	std::string z;
	TestClass() : x(0) {}
	TestClass(float _x) : x(_x), y("y"), z("z") {}
	void test(const std::string& _y, float _x)
	{
		x = _x;
		y = _y;
	}
};


class TestClass2 {
public:
	float x;
	std::string y;
	std::string z;
	TestClass2(float _x) : x(_x), y("y"), z("z") {}
	TestClass2(float _x, const std::string& y) : x(_x), y(y), z("z") {}
	void test(const std::string& _z, float _x)
	{
		x = _x;
		z = _z;
	}
};

void CScriptManager::RegisterLUAFunctions()
{
	LuaErrorCapturedStdout errorCapture;
	// TODO

	(*m_state)["CScriptManager"]
		.SetObj(*this,
				"RunCode", &CScriptManager::RunCode,
				"RunFile", &CScriptManager::RunFile,
				"Load", &CScriptManager::Load);
	(*m_state)["Vect3f"]
		.SetClass<Vect3f, float, float, float>("x", &Vect3f::x,
					"y", &Vect3f::y,
					"z", &Vect3f::z);
	(*m_state)["CRenderableObjectsManager"]
		.SetObj(*CEngine::GetSingleton().getRenderableObjectManager(),
		"AddMeshInstance", static_cast<void(CRenderableObjectsManager::*)(const std::string&, const std::string&, const Vect3f &)>(&CRenderableObjectsManager::AddMeshInstance));

	{
		LuaErrorCapturedStdout errorCapture;

		TestClass t;
		(*m_state)["t"]
			.SetObj(t, "test", &TestClass::test,
						"x", &TestClass::x);
		(*m_state)("t:test('a', 3.0);");
		(*m_state)("x = t:x()");
		float x = (*m_state)["x"];

		(*m_state)["TC2"]
			.SetClass<TestClass2, float, std::string>("x", &TestClass2::x,
					"test", static_cast<void(TestClass2::*)(const std::string&, float)>(&TestClass2::test));

		(*m_state)("t2 = TC2.new(3.0, 'b')");
		(*m_state)("t2:test('p', 1.0);");
		(*m_state)("x2 = t2:x()");
		float x2 = (*m_state)["x2"];

	}
}
