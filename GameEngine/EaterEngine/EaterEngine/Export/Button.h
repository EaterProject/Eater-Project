#pragma once
#include "Image.h"

class Button : public Image
{
public:
	EATER_ENGINEDLL Button();
	virtual ~Button();

public:
	void Awake() override;
	
public:

};

