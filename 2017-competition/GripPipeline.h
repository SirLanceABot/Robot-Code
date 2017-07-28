#pragma once
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>
#include <math.h>

class GripPipeline
{
public:
	GripPipeline();
	void process(cv::Mat source0);
	std::vector<double> extraProcessing();
	cv::Mat* gethslThresholdOutput();
	std::vector<std::vector<cv::Point>>* getfindContoursOutput();
	std::vector<std::vector<cv::Point>>* getfilterContoursOutput();

private:
	cv::Mat resizeImageOutput;
	cv::Mat hslThresholdOutput;
	std::vector<std::vector<cv::Point>> findContoursOutput;
	void hslThreshold(cv::Mat &, double [], double [], double [], cv::Mat &);
	void findContours(cv::Mat &, bool , std::vector<std::vector<cv::Point>> &);
	void filterContours(std::vector<std::vector<cv::Point>> &, double , double , double , double , double , double , double [], double , double , double , double , std::vector<std::vector<cv::Point>> &);

	double hslThresholdHue[2]  = {51.0, 99.0};
	double hslThresholdSaturation[2] = {198.0, 255.0};
	double hslThresholdLuminance[2] = {40.0, 113.0};
	bool findContoursExternalOnly;
	//std::vector<std::vector<cv::Point>> filterContoursContours;
	std::vector<std::vector<cv::Point>> filterContoursOutput;
	double filterContoursMinArea;
	double filterContoursMinPerimeter;
	double filterContoursMinWidth;
	double filterContoursMaxWidth;
	double filterContoursMinHeight;
	double filterContoursMaxHeight;
	double filterContoursSolidity[2] = {0, 100};
	double filterContoursMaxVertices;
	double filterContoursMinVertices;
	double filterContoursMinRatio;
	double filterContoursMaxRatio;
	int idNum;
};
