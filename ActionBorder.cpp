#include "ActionBorder.h"

ActionBorder::ActionBorder()
{
}

ActionBorder::~ActionBorder()
{
}

void ActionBorder::paint(PageContentContext* ctx)
{
	AbstractContentContext::GraphicOptions  opt(AbstractContentContext::eStroke,
		AbstractContentContext::eRGB, color);
	//ctx->DrawRectangle(x, y, w, h, opt);
}
