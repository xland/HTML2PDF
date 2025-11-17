#pragma once
#include "Action.h"
class ActionBorder : public Action
{
public:
	ActionBorder();
	~ActionBorder() override;
	void paint(PageContentContext* ctx) override;
private:
};

