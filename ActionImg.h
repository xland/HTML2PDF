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
	float w, h;
	PDF* pdf;
private:
};

