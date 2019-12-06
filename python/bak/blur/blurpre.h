#include <opencv2/opencv.hpp>
#include <stdio.h>

//   input: a 3 channels or 1 channel image
//   output: return 0 means sharp image, return 1 means blur image
int blurpre(cv::Mat &src)
{
	cv::Mat gray;
	if (src.channels() == 3) {
		cv::cvtColor(src, gray, CV_RGB2GRAY);
	}
	else if (src.channels() == 1) {
		gray = src.clone();
	}
	else{
		printf("input image error!");
		return 0;
	}

	cv::Mat lap;
	cv::Laplacian(gray, lap, CV_64FC1, 3);

	lap = abs(lap);

	cv::Mat mean, std;
	cv::meanStdDev(lap, mean, std);
	double s = std.at<double>(0, 0);
    printf("value: %f\n", s);

	int ret = s < 5 ? 1 : 0;

	return ret;
}
