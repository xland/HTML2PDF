#pragma once
#include "Action.h"
class PDF;
class ActionImg : public Action
{
public:
	ActionImg();
	~ActionImg() override;
	void paint(PageContentContext* ctx) override;
public:
	std::string imgPath;
	float w, h, wClip{ -1 }, hClip{ -1 }, xClip{ -1 }, yClip{-1};
	PDF* pdf;
private:
};

