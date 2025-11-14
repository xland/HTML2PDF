#include "PDF.h"
#include "DocumentContainer.h"

PDF::PDF()
{
	pdfWriter.StartPDF("allen.pdf", ePDFVersion13);
	PDFPage* page = new PDFPage();
	page->SetMediaBox(PDFRectangle(0, 0, 595, 842)); //A4 纸张尺寸,宽度：595 点 ≈ 210 mm，高度：842 点 ≈ 297 mm
	PageContentContext* contentContext = pdfWriter.StartPageContentContext(page);
	pageContexts.push_back(contentContext);
	pages.push_back(page);
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
	auto twoEdge = edge * 2;
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

void PDF::createFont(const std::string& fontName,const long& fontSize)
{
	auto fontPath = std::format("C:\\Windows\\Fonts\\{}.ttf", fontName);
	font = pdfWriter.GetFontForFile(fontPath); // PDFWriter在析构时，会自动释放所有已加载的字体资源。
	this->fontSize = fontSize;
}
