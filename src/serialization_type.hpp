#pragma once

#include <opencv2/opencv.hpp>
#include <cereal/cereal.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
namespace cv {

	/**
	* Serialise a cv::Mat using cereal.
	*
	* Supports all types of matrices as well as non-contiguous ones.
	*
	* @param[in] ar The archive to serialise to.
	* @param[in] mat The matrix to serialise.
	*/
	template<class Archive>
	void save(Archive& ar, const cv::Mat& mat)
	{
		int rows, cols, type;
		bool continuous;

		rows = mat.rows;
		cols = mat.cols;
		type = mat.type();
		continuous = mat.isContinuous();

		ar & rows & cols & type & continuous;

		if (continuous) {
			const int data_size = rows * cols * static_cast<int>(mat.elemSize());
			auto mat_data = cereal::binary_data(mat.ptr(), data_size);
			ar & mat_data;
		}
		else {
			const int row_size = cols * static_cast<int>(mat.elemSize());
			for (int i = 0; i < rows; i++) {
				auto row_data = cereal::binary_data(mat.ptr(i), row_size);
				ar & row_data;
			}
		}
	};

	/**
	* De-serialise a cv::Mat using cereal.
	*
	* Supports all types of matrices as well as non-contiguous ones.
	*
	* @param[in] ar The archive to deserialise from.
	* @param[in] mat The matrix to deserialise into.
	*/
	template<class Archive>
	void load(Archive& ar, cv::Mat& mat)
	{
		int rows, cols, type;
		bool continuous;

		ar & rows & cols & type & continuous;

		if (continuous) {
			mat.create(rows, cols, type);
			const int data_size = rows * cols * static_cast<int>(mat.elemSize());
			auto mat_data = cereal::binary_data(mat.ptr(), data_size);
			ar & mat_data;
		}
		else {
			mat.create(rows, cols, type);
			const int row_size = cols * static_cast<int>(mat.elemSize());
			for (int i = 0; i < rows; i++) {
				auto row_data = cereal::binary_data(mat.ptr(i), row_size);
				ar & row_data;
			}
		}
	};
}

struct CameraIntrinsic {
	template<class Archive>
	void serialize(Archive & archive)
	{
		archive & camera & distCoeffs & imageWidth & imageHeight;
	}
	cv::Mat camera;
	cv::Mat distCoeffs;
	int imageWidth = 0;
	int imageHeight = 0;
};
