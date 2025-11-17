#include "ActionBg.h"

ActionBg::~ActionBg()
{
}

ActionBg::ActionBg()
{
}

void ActionBg::paint(PageContentContext* ctx)
{
	AbstractContentContext::GraphicOptions  opt(AbstractContentContext::eFill,
		AbstractContentContext::eRGB, color);
	ctx->DrawRectangle(x, y, w, h, opt);
}
