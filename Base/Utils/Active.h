#ifndef ACTIVE_H
#define ACTIVE_H

class CActive {

protected:
	bool m_active;
public:
	CActive() : m_active(true) {}
	CActive(bool active) : m_active(active) {}
	bool isActive(){ return m_active; }
	void setActive(bool act){ m_active = act; }

};

#endif