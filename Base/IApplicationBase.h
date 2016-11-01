#pragma once


class IApplicationBase
{
public:
	IApplicationBase() : m_quit(false) {}
	virtual ~IApplicationBase() {}

	virtual void Update( double _ElapsedTime ) = 0;
	virtual void Render() = 0;

	virtual void Destroy() = 0;

	inline void Quit() { m_quit = true; }

	inline bool ShouldQuit() const { return m_quit; }
private:
	bool m_quit;
};
