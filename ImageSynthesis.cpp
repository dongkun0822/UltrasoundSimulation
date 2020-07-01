#include "pch.h"
#include "ImageSynthesis.h"

void ImageSynthesis::Synthesis(string ReflectionImagePath, string ScatteringImagePath)
{
	Mat ReflectionImage = imread(ReflectionImagePath);
	Mat ScatteringImage = imread(ScatteringImagePath);
	Mat SynthesisImage = ReflectionImage;

	int row = ReflectionImage.rows;
	int col = ReflectionImage.cols * ReflectionImage.channels();

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			int ReflectionIntensity = ReflectionImage.at<uchar>(i, j);
			int ScatteringIntensity = ScatteringImage.at<uchar>(i, j);
			SynthesisImage.at<uchar>(i, j) = ReflectionIntensity * afa + ScatteringIntensity * beta;
		}
	}

	imwrite("SynthesisImage.png", SynthesisImage);
}