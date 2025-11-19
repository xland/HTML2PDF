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
#include <PageContentContext.h>
#include <litehtml.h>
#include "Font.h"

class Action;
class PDF
{
public:
	PDF();
	~PDF();
	void start(const std::string& htmlPath);
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
	int edge{ 26 };

	int viewWidth{ width - edge * 2 };

	int viewHeight{ height - edge * 2 };

	PDFWriter pdfWriter;
	std::vector<Action*> actions;
	float dpi{1.f};
private:
private:
};

