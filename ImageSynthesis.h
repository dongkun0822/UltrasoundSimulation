#pragma once
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define afa 0.5
#define beta 0.5

class ImageSynthesis
{
public:
	void Synthesis(char* ReflectionImagePath, char* ScatteringImagePath);
};

