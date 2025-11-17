#pragma once
#include "Action.h"
class ActionListDot : public Action
{
public:
	ActionListDot();
	~ActionListDot() override;
	void paint(PageContentContext* ctx) override;
public:
	int dotType;
	float size;
private:
};

