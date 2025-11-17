#include "ActionBorder.h"

ActionBorder::ActionBorder()
{
}

ActionBorder::~ActionBorder()
{
}

void ActionBorder::paint(PageContentContext* ctx)
{
	DoubleAndDoublePairList points;
	points.push_back(std::make_pair(x, y));
	points.push_back(std::make_pair(x2, y2));
	AbstractContentContext::GraphicOptions opt(AbstractContentContext::eStroke,
		AbstractContentContext::eRGB, color,width);
	ctx->DrawPath(points, opt);
}
