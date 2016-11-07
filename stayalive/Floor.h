#pragma once
#include "Surface.h"
class Floor :
	public Surface
{
public:
	Floor();

	void Draw()const final;

	~Floor();
};

