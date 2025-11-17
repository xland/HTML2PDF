#pragma once
#include "Action.h"
class ActionBorder : public Action
{
public:
	ActionBorder();
	~ActionBorder() override;
	void paint(PageContentContext* ctx) override;
public:
	double x2, y2;
	double width;
private:
};

