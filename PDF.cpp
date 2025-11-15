#include "PDF.h"
#include "DocumentContainer.h"
#include <iostream>

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
	litehtml::position clip(0, 0, viewWidth, viewHeight);
	doc->draw((litehtml::uint_ptr)nullptr, 0, 0, &clip);
}

void PDF::finish()
{
	for (size_t i = 0; i < pages.size(); i++)
	{
		pdfWriter.EndPageContentContext(pageContexts[i]);
		pdfWriter.WritePageAndRelease(pages[i]);
	}
	pdfWriter.EndPDF();
}

Font* PDF::createFont(const litehtml::font_description& descr)
{
	auto it = std::find_if(fonts.begin(), fonts.end(),
		[&descr](const Font* f) { return f->name == descr.family; });
	if (it == fonts.end()) {
		//auto fontPath = std::format("C:\\Windows\\Fonts\\{}.ttf", descr.family);
		auto fontPath = std::format("C:\\Windows\\Fonts\\{}.ttc", descr.family);
		auto f = new Font();
		f->font = pdfWriter.GetFontForFile(fontPath);
		f->size = descr.size;
		f->name = descr.family;
		f->weight = descr.weight;
		fonts.push_back(f);
		return f;
	}
	else if((*it)->size != descr.size || (*it)->weight != descr.weight){
		auto f = new Font();
		f->font = (*it)->font;
		f->size = descr.size;
		f->name = descr.family;
		f->weight = descr.weight;
		fonts.push_back(std::move(f));
		return f;
	}
	return (*it);
}



void PDF::drawListMarker(const litehtml::list_marker& marker)
{
	auto index = getContextIndex(marker.pos.y);
	AbstractContentContext::GraphicOptions opt(AbstractContentContext::eFill,
		AbstractContentContext::eRGB,
		(marker.color.red << 16) | (marker.color.green << 8) | (marker.color.blue));
	auto font = (Font*)marker.font;
	auto y = (index + 1) * height - edge - marker.pos.y + marker.pos.height-font->size*2/3;
	pageContexts[index]->DrawCircle(edge + marker.pos.x, y, marker.pos.height / 2,opt);
}

void PDF::drawSolidFill(const litehtml::background_layer& layer, const litehtml::web_color& color)
{
	auto index = getContextIndex(layer.border_box.y+layer.border_box.height);
	AbstractContentContext::GraphicOptions opt(AbstractContentContext::eFill,
		AbstractContentContext::eRGB, (color.red << 16) | (color.green << 8) | (color.blue));
	auto y = (index + 1) * height - edge - (layer.border_box.y + layer.border_box.height);
	pageContexts[index]->DrawRectangle(edge + layer.border_box.x, y, layer.border_box.width, layer.border_box.height, opt);
}

void PDF::drawText(const std::string& text, const litehtml::web_color& color, const litehtml::position& pos, Font* font)
{
	PDFUsedFont::TextMeasures textDimensions = font->font->CalculateTextDimensions(text, font->size);
	auto index = getContextIndex(pos.y - font->size);
	LOG(text, "__", index);
	auto y = (index + 1)*height - edge - pos.y - font->size;
	auto x = edge + pos.x;
	if (pos.y+font->size > clipY + clipH || pos.x > clipX + clipW) return;
	AbstractContentContext::TextOptions textOptions(font->font, font->size, AbstractContentContext::eRGB, 0);
	textOptions.colorValue = (color.red << 16) | (color.green << 8) | (color.blue);
	auto& ctx = pageContexts[index];
	ctx->WriteText(x, y, text, textOptions);
}

int PDF::getContextIndex(const double& y)
{
	int pageIndex = (long)y / viewHeight;
	if (pages.size() < pageIndex+1)
	{
		auto count = pageIndex + 1 - pages.size();
		for (size_t i = 0; i < count; i++)
		{
			PDFPage* page = new PDFPage();
			page->SetMediaBox(PDFRectangle(0, 0, width, height)); //A4 纸张尺寸,宽度：595 点 ≈ 210 mm，高度：842 点 ≈ 297 mm
			PageContentContext* contentContext = pdfWriter.StartPageContentContext(page);
			pageContexts.push_back(contentContext);
			pages.push_back(page);
		}
	}
	return pageIndex;
}
