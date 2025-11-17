#include "ActionListDot.h"
#include "Font.h"

ActionListDot::ActionListDot()
{
}

ActionListDot::~ActionListDot()
{
}

void ActionListDot::paint(PageContentContext* ctx)
{
	if (dotType == 1) {
		AbstractContentContext::GraphicOptions  opt(AbstractContentContext::eStroke, AbstractContentContext::eRGB, color);
		ctx->DrawCircle(x, y, size, opt);
	}
	else if(dotType == 3){
		AbstractContentContext::GraphicOptions  opt(AbstractContentContext::eFill, AbstractContentContext::eRGB, color);
		ctx->DrawRectangle(x-size, y-size, size*2, size*2,opt);
	}
	else if (dotType == 6) {
		AbstractContentContext::TextOptions textOptions(font->font, font->size, AbstractContentContext::eRGB, color);
		ctx->WriteText(x - size, y - size+font->size, std::to_string(index), textOptions);
	}
	else {
		AbstractContentContext::GraphicOptions  opt(AbstractContentContext::eFill, AbstractContentContext::eRGB, color);
		ctx->DrawCircle(x, y, size, opt);
	}
}
