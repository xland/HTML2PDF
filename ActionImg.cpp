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
		if (hClip < 0 && wClip < 0) {
			ctx->DrawImage(x, y, imgPath, opt);
			return;
		}
		else {
			ctx->q(); // 保存图形状态
			ctx->re(xClip, yClip, wClip, hClip); //设置裁剪区域 创建矩形路径：x, y, width, height
			ctx->W(); // 设置裁剪路径（Clip）
			ctx->n(); // 结束路径（不描边不填充）
			ctx->DrawImage(x, y, imgPath, opt); // 左下角对齐裁剪区		
			ctx->Q(); // 恢复图形状态（取消裁剪）
		}
	//}
	//else {
	//	PDFImageXObject* imageXObject = pdf->pdfWriter.CreateImageXObjectFromJPGFile(imgPath);
	//	ctx->DrawImage(x, y, imageXObject);
	//}
	
}
