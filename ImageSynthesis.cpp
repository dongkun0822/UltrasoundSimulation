#include "pch.h"
#include "ImageSynthesis.h"

void ImageSynthesis::Synthesis(char* ReflectionImagePath, char* ScatteringImagePath)
{
	IplImage* ReflectionImage = cvLoadImage(ReflectionImagePath, -1);
	IplImage* ScatteringImage = cvLoadImage(ScatteringImagePath, -1);
	IplImage* SynthesisImage = ReflectionImage;

	int row = ReflectionImage->height;
	int col = ReflectionImage->width;// *ReflectionImage.channels();
	int channel = ReflectionImage->nChannels;
	int step = SynthesisImage->widthStep / sizeof(uchar);
	uchar* data = (uchar*)SynthesisImage->imageData;

	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
		{
			double ReflectionIntensity = cvGet2D(ReflectionImage, i, j).val[0];
			double ScatteringIntensity = cvGet2D(ScatteringImage, i, j).val[0];
			data[i * step + j] = ReflectionIntensity * afa + ScatteringIntensity * beta;
		}

	cvSaveImage("image/SynthesisImage.png", SynthesisImage);
}