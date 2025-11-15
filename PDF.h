#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <format>
#include <memory>
#include <string>
#include <litehtml/document.h>
#include <PDFWriter.h>
#include <PDFUsedFont.h>
#include <PDFPage.h>
#include <AbstractContentContext.h>
#include <PageContentContext.h>
#include <litehtml.h>
#include "Font.h"
class PDF
{
public:
	PDF();
	~PDF();
	void start();
	void finish();
	Font* createFont(const litehtml::font_description& descr);
	void drawText(const std::string& text,const litehtml::web_color& color, const litehtml::position& pos,Font* font);
	void drawListMarker(const litehtml::list_marker& marker);
	void drawSolidFill(const litehtml::background_layer& layer, const litehtml::web_color& color);
public:
	/// <summary>
	/// A4 纸张尺寸，宽度：595 点 ≈ 210 mm，
	/// </summary>
	int width{ 595 };
	/// <summary>
	/// A4 纸张尺寸，高度：842 点 ≈ 297 mm
	/// </summary>
	int height{ 842 };
	/// <summary>
	/// 纸张边距
	/// </summary>
	int edge{ 28 };

	int viewWidth{ width - edge * 2 };

	int viewHeight{ height - edge * 2 };
private:
	int getContextIndex(const double& y);
private:
	std::vector<Font*> fonts;
	PDFWriter pdfWriter;
	std::vector<PDFPage*> pages;
	std::vector<PageContentContext*> pageContexts;
};

