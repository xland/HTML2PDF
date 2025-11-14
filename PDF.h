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
class PDF
{
public:
	PDF();
	~PDF();
	void start();
	void finish();
	void createFont(const std::string& fontName, const long& fontSize);
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
	int edge{ 36 };

	int viewWidth{ width - edge * 2 };

	int viewHeight{ height - edge * 2 };

	PDFUsedFont* font;
	long fontSize;
private:
	PDFWriter pdfWriter;
	std::vector<PDFPage*> pages;
	std::vector<PageContentContext*> pageContexts;
};

