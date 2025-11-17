#pragma once
#include "Action.h"
class Font;
class ActionListDot : public Action
{
public:
	ActionListDot();
	~ActionListDot() override;
	void paint(PageContentContext* ctx) override;
public:
	int dotType;
	float size;
	int index;
	Font* font;
private:
};

