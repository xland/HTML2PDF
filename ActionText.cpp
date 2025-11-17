#include "ActionText.h"
#include "Font.h"

ActionText::ActionText(const std::string& text, Font* font):
	text{text}, font{font}
{
	
}

ActionText::~ActionText()
{
}

void ActionText::paint(PageContentContext* ctx)
{
	AbstractContentContext::TextOptions textOptions(font->font, font->size, AbstractContentContext::eRGB, color);
	ctx->WriteText(x, y, text, textOptions);
}
