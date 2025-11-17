#include "PDF.h"
#include "DocumentContainer.h"
#include <iostream>
#include "Action.h"
#include "Helper.h"


PDF::PDF()
{
	pdfWriter.StartPDF("allen.pdf", ePDFVersion13);
}

PDF::~PDF()
{
}

void PDF::start()
{
	std::stringstream ss{};
	std::ifstream("demo2.html", std::ios::binary) >> ss.rdbuf();
	auto html = ss.str();
	auto docContainer = new  DocumentContainer(this);
	auto doc = litehtml::document::createFromString(html, docContainer);
	int best_width = doc->render(viewWidth);
	litehtml::position clip(0, 0, viewWidth, INT_MAX);
	doc->draw((litehtml::uint_ptr)nullptr, 0, 0, &clip);

	std::vector<Action*> temp;
	auto pageIndex{ 0 };
	std::copy_if(actions.begin(), actions.end(), std::back_inserter(temp),
		[&pageIndex](Action* a) {return a->pageIndex == pageIndex; });
	if (temp.empty()) {
		LOG("没有发现需要写入PDF的元素");
		return;
	}
	while (!temp.empty()) {
		PDFPage* page = new PDFPage();
		page->SetMediaBox(PDFRectangle(0, 0, width, height)); //A4 纸张尺寸,宽度：595 点 ≈ 210 mm，高度：842 点 ≈ 297 mm
		PageContentContext* ctx = pdfWriter.StartPageContentContext(page);
		for (size_t i = 0; i < temp.size(); i++)
		{
			temp[i]->paint(ctx);
		}
		pdfWriter.EndPageContentContext(ctx);
		pdfWriter.WritePageAndRelease(page);
		pageIndex += 1;
		temp.clear();
		std::copy_if(actions.begin(), actions.end(), std::back_inserter(temp),
			[&pageIndex](Action* a) {return a->pageIndex == pageIndex; });
	}
}

void PDF::finish()
{
	pdfWriter.EndPDF();
}
