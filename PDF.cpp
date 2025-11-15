#include "PDF.h"
#include "DocumentContainer.h"


#include <iostream>


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
		AbstractContentContext::eRGB,
		(color.red << 16) | (color.green << 8) | (color.blue));
	auto y = (index + 1) * height - edge - (layer.border_box.y + layer.border_box.height);
	pageContexts[index]->DrawRectangle(edge + layer.border_box.x, y, layer.border_box.width, layer.border_box.height, opt);
}


std::vector<std::string> SplitUTF8IntoChars(const std::string& str) {
	std::vector<std::string> result;
	for (size_t i = 0; i < str.size(); ) {
		unsigned char c = str[i];
		size_t charLen;
		if ((c & 0x80) == 0) charLen = 1;          // ASCII
		else if ((c & 0xE0) == 0xC0) charLen = 2;  // 2-byte
		else if ((c & 0xF0) == 0xE0) charLen = 3;  // 3-byte (most CJK)
		else if ((c & 0xF8) == 0xF0) charLen = 4;  // 4-byte
		else charLen = 1; // invalid, skip
		auto s = str.substr(i, charLen);
		//LOG(s);
		//LOG(charLen);
		result.push_back(s);
		i += charLen;
	}
	return result;
}

void PDF::drawText(const std::string& text, const litehtml::web_color& color, const litehtml::position& pos, Font* font)
{
	PDFUsedFont::TextMeasures textDimensions = font->font->CalculateTextDimensions(text, font->size);
	auto index = getContextIndex(pos.y - font->size);
	auto y = (index + 1)*height - edge - pos.y - font->size;
	AbstractContentContext::TextOptions textOptions(font->font, font->size, AbstractContentContext::eRGB, 0);
	textOptions.colorValue = (color.red << 16) | (color.green << 8) | (color.blue);
	float r{ color.red / 255.f }, g{ color.green / 255.f }, b{ color.blue / 255.f };
	auto& ctx = pageContexts[index];
	pageContexts[index]->WriteText(edge + pos.x, y, text, textOptions);
	//std::vector<std::string> chars = SplitUTF8IntoChars(text);
	//ctx->BT(); 
	//ctx->rg(r, g, b); 
	//ctx->Tf(font->font, font->size);
	//double currentX = edge + pos.x;
	//for (const auto& ch : chars)
	//{
	//	ctx->Tm(1, 0, 0, 1, currentX, y);
	//	ctx->Tj(ch);		
	//	currentX = currentX + font->font->CalculateTextDimensions(ch, font->size).width + 6;
	//}
	//ctx->ET();
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
			//contentContext->WriteFreeCode("2.0 Tc\n");
			pageContexts.push_back(contentContext);
			pages.push_back(page);
		}
	}
	return pageIndex;
}
