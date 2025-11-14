#include "PDF.h"
#include "DocumentContainer.h"

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
	pdfWriter.EndPageContentContext(pageContexts[0]);
	pdfWriter.WritePageAndRelease(pages[0]);
	pdfWriter.EndPDF();
}

Font* PDF::createFont(const std::string& fontName,const long& fontSize)
{
	auto it = std::find_if(fonts.begin(), fonts.end(),
		[&fontName](const Font& f) { return f.name == fontName; });
	if (it == fonts.end()) {
		auto fontPath = std::format("C:\\Windows\\Fonts\\{}.ttf", fontName);
		Font f;
		f.font = pdfWriter.GetFontForFile(fontPath); // PDFWriter在析构时，会自动释放所有已加载的字体资源。
		f.size = fontSize;
		f.name = fontName;
		fonts.push_back(std::move(f));
		return &fonts[fonts.size() - 1];
	}
	else if(it->size != fontSize){
		Font f;
		f.font = it->font;
		f.size = fontSize;
		f.name = fontName;
		fonts.push_back(std::move(f));
		return &fonts[fonts.size() - 1];
	}
	return &(*it);
}

void PDF::drawText(const std::string& text, const litehtml::web_color& color, const litehtml::position& pos, Font* font)
{
	PDFUsedFont::TextMeasures textDimensions = font->font->CalculateTextDimensions(text, font->size);
	AbstractContentContext::TextOptions textOptions(font->font, font->size, AbstractContentContext::eRGB, 0);
	auto y = 842 - 32 - pos.y + textDimensions.yMax;
	auto context = getContext(y);
	context->WriteText(32+pos.x, y, text, textOptions);
}

PageContentContext* PDF::getContext(const double& y)
{
	int pageIndex = (long)y / viewHeight;
	if (pages.size() < pageIndex+1)
	{
		auto count = pageIndex + 1 - pages.size();
		for (size_t i = 0; i < count; i++)
		{
			PDFPage* page = new PDFPage();
			page->SetMediaBox(PDFRectangle(0, 0, 595, 842)); //A4 纸张尺寸,宽度：595 点 ≈ 210 mm，高度：842 点 ≈ 297 mm
			PageContentContext* contentContext = pdfWriter.StartPageContentContext(page);
			pageContexts.push_back(contentContext);
			pages.push_back(page);
		}
	}
	return pageContexts[pageIndex];
}
