#pragma once
#include <Component.h>
class Button;
class Image;

class UIPause : public Component
{
public:
	UIPause();
	~UIPause();

private:
	enum PAUSE
	{
		RESUME,
		OPTION,
		EXIT
	};

public:
	void Awake() override;

public:
	void SetPauseUIActive(bool active);

private:
	void SetResumeIn();
	void SetResumeOut();
	void SetResumeClick();
	void SetOptionIn();
	void SetOptionOut();
	void SetOptionClick();
	void SetExitIn();
	void SetExitOut();
	void SetExitClick();

private:
	Image* PauseImage;
	Button* PauseButton[3];
};

