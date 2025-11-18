#include "ActionImg.h"
#include "PDF.h"


ActionImg::ActionImg()
{
}

ActionImg::~ActionImg()
{
}

void ActionImg::paint(PageContentContext* ctx)
{
	//if (imgPath.ends_with(".png")) {
		AbstractContentContext::ImageOptions opt;
		opt.transformationMethod = AbstractContentContext::eFit;
		opt.boundingBoxWidth = w;
		opt.boundingBoxHeight = h;
		ctx->DrawImage(x, y, imgPath, opt);


		// 在当前页裁剪图像（仅显示可见部分）
		ctx->q(); // save
		ctx->Rectangle(0, 0, pageWidth, pageHeight);
		ctx->Clip(); // W n
		ctx->DrawImage(x, y, imgPath, opt);
		ctx->Q(); // restore


	//}
	//else {
	//	PDFImageXObject* imageXObject = pdf->pdfWriter.CreateImageXObjectFromJPGFile(imgPath);
	//	ctx->DrawImage(x, y, imageXObject);
	//}
	
}
