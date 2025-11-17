#pragma once
#include "Action.h"
class Font;
class ActionText : public Action
{
public:
	ActionText(const std::string& text, Font* font);
	~ActionText();
	void paint(PageContentContext* ctx) override;
private:
	std::string text;	
	Font* font;
};

