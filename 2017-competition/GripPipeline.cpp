#include "GripPipeline.h"

GripPipeline::GripPipeline()
{
	findContoursExternalOnly = false;
	filterContoursMinArea = 15.0;
	filterContoursMinPerimeter = 0.0;
	filterContoursMinWidth = 0.0;
	filterContoursMaxWidth = 1000.0;
	filterContoursMinHeight = 0.0;
	filterContoursMaxHeight = 1000.0;
	filterContoursMaxVertices = 1000000.0;
	filterContoursMinVertices = 0.0;
	filterContoursMinRatio = 0.0;
	filterContoursMaxRatio = 1000.0;
	idNum = 0;
}

void GripPipeline::process(cv::Mat source0)
{

	cv::Mat hslThresholdInput = source0;

	hslThreshold(hslThresholdInput, hslThresholdHue, hslThresholdSaturation, hslThresholdLuminance, this->hslThresholdOutput);

	cv::Mat findContoursInput = hslThresholdOutput;

	findContours(findContoursInput, findContoursExternalOnly, this->findContoursOutput);

	std::vector<std::vector<cv::Point> > filterContoursContours = findContoursOutput;
	filterContours(filterContoursContours, filterContoursMinArea, filterContoursMinPerimeter, filterContoursMinWidth, filterContoursMaxWidth, filterContoursMinHeight, filterContoursMaxHeight, filterContoursSolidity, filterContoursMaxVertices, filterContoursMinVertices, filterContoursMinRatio, filterContoursMaxRatio, this->filterContoursOutput);
}

cv::Mat* GripPipeline::gethslThresholdOutput()
{
	return &(this->hslThresholdOutput);
}

std::vector<std::vector<cv::Point>>* GripPipeline::getfindContoursOutput()
{
	return &(this->findContoursOutput);
}

std::vector<std::vector<cv::Point>>* GripPipeline::getfilterContoursOutput()
{
	return &(this->filterContoursOutput);
}


void GripPipeline::hslThreshold(cv::Mat &input, double hue[], double sat[], double lum[], cv::Mat &out)
{
	cv::cvtColor(input, out, cv::COLOR_BGR2HLS);
	cv::inRange(out, cv::Scalar(hue[0], lum[0], sat[0]), cv::Scalar(hue[1], lum[1], sat[1]), out);
}

void GripPipeline::findContours(cv::Mat &input, bool externalOnly, std::vector<std::vector<cv::Point>> &contours)
{
	std::vector<cv::Vec4i> hierarchy;
	contours.clear();
	int mode = externalOnly ? cv::RETR_EXTERNAL : cv::RETR_LIST;
	int method = cv::CHAIN_APPROX_SIMPLE;
	cv::findContours(input, contours, hierarchy, mode, method);
}

void GripPipeline::filterContours(std::vector<std::vector<cv::Point>> &inputContours, double minArea, double minPerimeter, double minWidth, double maxWidth, double minHeight, double maxHeight, double solidity[], double maxVertexCount, double minVertexCount, double minRatio, double maxRatio, std::vector<std::vector<cv::Point>> &output)
{
	//std::vector<cv::Point> hull;
	output.clear();
	for (std::vector<cv::Point> contour: inputContours)
	{
		//cv::Rect bb = boundingRect(contour);
		//if (bb.width < minWidth || bb.width > maxWidth) continue;
		//if (bb.height < minHeight || bb.height > maxHeight) continue;
		double area = cv::contourArea(contour);
		if (area < minArea) continue;
		/*
		if (arcLength(contour, true) < minPerimeter) continue;
		cv::convexHull(cv::Mat(contour, true), hull);
		double solid = 100 * area / cv::contourArea(hull);
		if (solid < solidity[0] || solid > solidity[1]) continue;
		if (contour.size() < minVertexCount || contour.size() > maxVertexCount)	continue;
		double ratio = bb.width / bb.height;
		if (ratio < minRatio || ratio > maxRatio) continue;
		*/
		output.push_back(contour);
	}
}

std::vector<double> GripPipeline::extraProcessing()
{
	cv::Rect br;
	std::vector<double> buff;
	double xSum = 0;
	for (std::vector<cv::Point> contour: this->filterContoursOutput)
	{
		br = boundingRect(contour);
		buff.push_back(br.x + (br.width / 2.0));
		buff.push_back(br.height);
		xSum += (br.x + (br.width / 2.0) + br.height);
	}
	idNum++;
	xSum += idNum;
	buff.push_back(idNum);
	buff.push_back(xSum);
	return buff;
}


