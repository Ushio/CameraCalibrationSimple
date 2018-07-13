#include "ofApp.h"


inline std::vector<cv::Point3f> chessboard_corners(int cornerWidth, int cornerHeight, float size) {
	std::vector<cv::Point3f> points;
	for (int j = 1; j < cornerHeight + 1; ++j) {
		for (int i = 1; i < cornerWidth + 1; ++i) {
			points.push_back(cv::Point3f(i * size, j * size, 0.0f));
		}
	}
	return points;
}



//--------------------------------------------------------------
void ofApp::setup(){
	//cv::Mat img(256, 256, CV_8UC3, cv::Scalar(100, 100, 100));
	//cv::circle(img, cv::Point(128, 128), 100, cv::Scalar(200, 100, 100), 4);
	//imshow("image", img);

	ofDirectory src("src");
	src.allowExt("JPG");
	src.listDir();

	float cameraImageWidth = 0.0f;
	float cameraImageHeight = 0.0f;

	std::vector<std::vector<cv::Point2f>> imagePoints;

	for (int i = 0; i < src.size(); i++) {
		ofFile file(src.getPath(i));
		
		cv::Mat image = cv::imread(file.getAbsolutePath());

		std::string dst = file.getEnclosingDirectory() + "..\\dst\\" + file.getFileName();
		
		std::vector<cv::Point2f> corners;
		if (cv::findChessboardCorners(image, cv::Size(8, 5), corners)) {
			cv::drawChessboardCorners(image, cv::Size(8, 5), corners, true);

			imagePoints.push_back(corners);
			cv::imwrite(dst, image);

			cameraImageWidth = image.cols;
			cameraImageHeight = image.rows;

			printf("[%d] %s done\n", i, src.getPath(i).c_str());
		}
		else {
			printf("[%d] %s is failed...\n", i, src.getPath(i).c_str());
		}
	}
	

	float f = cameraImageWidth * ofDegToRad(45.0f);
	cv::Mat1d cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
	cameraMatrix <<
		f, 0, cameraImageWidth * 0.5,
		0, f, cameraImageHeight * 0.5,
		0, 0, 1;

	// cv::getOptimalNewCameraMatrix(
	cv::Mat distCoeffs = cv::Mat::zeros(8, 1, CV_64F);

	std::vector<cv::Mat> boardRotations, boardTranslations;

	// å≈íËÇÃÇ‡ÇÃÇèÄîı
	std::vector<std::vector<cv::Point3f>> objectPoints;
	for (int i = 0; i < imagePoints.size(); ++i) {
		objectPoints.push_back(chessboard_corners(8, 5, 40.0f));
	}

	int flags = CV_CALIB_USE_INTRINSIC_GUESS;
	double value = cv::calibrateCamera(objectPoints,
		imagePoints,
		cv::Size(cameraImageWidth, cameraImageHeight),
		cameraMatrix,
		distCoeffs,
		boardRotations, boardTranslations,
		flags);

	printf("calibrateCamera: %.4f\n", value);

	printf("-dist coeffs-\n");
	printf("dist rows %d\n", distCoeffs.rows);

	printf("[");
	for (int i = 0; i < distCoeffs.rows; ++i) {
		double value = distCoeffs.at<double>(i, 0);
		printf("%.4f, ", value);
	}
	printf("]\n");

	printf("-camera matrix-\n");
	for (int row = 0; row < 3; ++row) {
		printf("%.4f, %.4f, %.4f\n", cameraMatrix.at<double>(row, 0), cameraMatrix.at<double>(row, 1), cameraMatrix.at<double>(row, 2));
	}

	cv::Mat mapx, mapy;
	cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(), cameraMatrix, cv::Size(cameraImageWidth, cameraImageHeight), CV_32FC1, mapx, mapy);

	for (int i = 0; i < src.size(); i++) {
		ofFile file(src.getPath(i));
		cv::Mat image = cv::imread(file.getAbsolutePath());
		std::string dst = file.getEnclosingDirectory() + "..\\dst\\undistort_" + file.getFileName();
		cv::Mat undistorted;
		cv::remap(image, undistorted, mapx, mapy, cv::INTER_LINEAR);
		cv::imwrite(dst, undistorted);
	}
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
