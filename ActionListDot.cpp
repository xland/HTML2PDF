#include "ActionListDot.h"

ActionListDot::ActionListDot()
{
}

ActionListDot::~ActionListDot()
{
}

void ActionListDot::paint(PageContentContext* ctx)
{
	AbstractContentContext::GraphicOptions  opt(AbstractContentContext::eFill,
		AbstractContentContext::eRGB, color);
	ctx->DrawCircle(x, y,size, opt);
}
