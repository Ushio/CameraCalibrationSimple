#pragma once

#include "ofMain.h"
#include <opencv2/opencv.hpp>

class Chessboard {
public:
	Chessboard(int width, int height, float size) {
		_width = width;
		_height = height;
		_size = size;
	}
	void drawGL() const {
		for (int i = 0; i < _width + 1; ++i) {
			for (int j = 0; j < _height + 1; ++j) {
				if ((i + j) % 2 == 0) {
					ofSetColor(0);
				}
				else {
					ofSetColor(255);
				}
				ofDrawRectangle(i * _size, j * _size, _size, _size);
			}
		}
	}
	std::vector<cv::Point2f> points() const {
		std::vector<cv::Point2f> points;
		for (int j = 1; j < _height + 1; ++j) {
			for (int i = 1; i < _width + 1; ++i) {
				points.push_back(cv::Point2f(i * _size, j * _size));
			}
		}
		return points;
	}
	std::vector<cv::Point2f> cornerOutside() const {
		std::vector<cv::Point2f> points;
		points.push_back(cv::Point2f(0.0f, 0.0f));
		points.push_back(cv::Point2f(widthSize(), 0.0f));
		points.push_back(cv::Point2f(0.0f, heightSize()));
		points.push_back(cv::Point2f(widthSize(), heightSize()));
		return points;
	}
	float widthSize() const {
		return (_width + 1) * _size;
	}
	float heightSize() const {
		return (_height + 1) * _size;
	}
private:
	int _width;
	int _height;
	float _size;
};