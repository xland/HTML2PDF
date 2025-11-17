#pragma once
#include "Action.h"
class ActionBg : public Action
{
	public:
		ActionBg();
		~ActionBg() override;
		void paint(PageContentContext* ctx) override;
	public:
		float w, h;
	private:
};

