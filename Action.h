#pragma once
#include <litehtml/document_container.h>
#include <PageContentContext.h>
class Action
{
public:
	Action() = default;
	virtual ~Action() = default;
	virtual void paint(PageContentContext* ctx) = 0;
public:
	double x, y;
	int pageIndex;
	unsigned long color;
private:
};

