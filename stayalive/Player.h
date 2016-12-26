#pragma once
class CPlayer
{
public:
	CPlayer();

	void PickUpKey();
	bool HasKey();

	~CPlayer();

private:
	bool m_key = false;
};

