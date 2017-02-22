//////////////////////////////////////////
//
// by shu-n
//
////////////////////////////////////////





#include<iostream>
#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\imgcodecs.hpp>
#include<opencv2\imgproc.hpp>
#include<opencv2\videoio.hpp>
#include<opencv2\opencv.hpp>
#include<vector>

using namespace cv;
using std::cout;
using std::endl;
using std::cin;


int main() {

	VideoCapture capture = VideoCapture(0);

	Mat src_img, initialize_img, sum_mat, bin_img, temp_img, gray_img;
	char comand;
	//BackgroundSubtractorMOG2 backgroundSubtractor;
	cout << "if you want to chang camera, press s(switch)" << endl;
	cout << "if you decide camera, press c(continue)" << endl;

	int cameraIndex = 0;

	while (1) {


		comand = waitKey(1);

		if (comand == 's') {
			cameraIndex++;

			cout << "camera Index is" << cameraIndex << endl;
			capture = VideoCapture(cameraIndex);
		}

		capture >> src_img;

		if (src_img.empty()) {
			cameraIndex = 0;
			capture = VideoCapture(0);
			capture >> src_img;
			continue;
		}


		if (comand == 'c') {
			break;
		}
		imshow("camera_select", src_img);

	}

	destroyAllWindows();

	cout << "final camera Index is" << cameraIndex << endl;



	int frame = 0;
	while (frame < 1) {
		capture >> src_img;
		//cout << src_img << endl;
		initialize_img = src_img;
		frame++;
		cout << frame << endl;
	}
	//initialize_img /= 1;
	//cout << initialize_img << endl;
	//imshow("initialize_img",initialize_img);
	cvtColor(initialize_img, initialize_img, CV_RGB2GRAY);
	cout << "initialize_img.channels " << initialize_img.channels() << endl;
	cout << "src_img,chanele " << src_img.channels() << endl;
	cout << "prees hand show to camera" << endl;
	waitKey(1000);


	int threshold_int = 60;

	cout << "if you want change threshold,Press 'w' or 's'"<< endl;
	cout << "ifyou want to finish this application, press 'q' " << endl;
	while (1) {
		capture >> src_img;

		//temp_img = src_img - ~initialize_img-20;
		cvtColor(src_img, gray_img, CV_RGB2GRAY);
		absdiff(gray_img, initialize_img, temp_img);
		//temp_img = temp_img;
		cv::imshow("Video_initialize", temp_img);

		//cvtColor(temp_img,gray_img,CV_RGB2GRAY);
		threshold(temp_img, bin_img, threshold_int, 255, CV_THRESH_BINARY);


		comand = waitKey(1);

		if (comand == 'q') {
			break;

		}
		if (comand == 'w') {
			threshold_int++;
			cout << "threshold" << threshold_int << endl;
		}

		if (comand == 's') {
			threshold_int--;
			cout << "threshold" << threshold_int << endl;
		}

		std::vector<std::vector<Point>> contours;

		findContours(bin_img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

		if (contours.empty()) {
			imshow("bin", bin_img);
			imshow("Video", src_img);
			continue;
		}

		std::vector<std::vector<Point>>::iterator it = contours.begin();
		std::vector<std::vector<Point>>::iterator maxSizeIt = it;
		std::vector<Point>::iterator inIt;
		double maxAriaSize = 0;
		double areaSize = 0;
		for (; it != contours.end(); ++it) {
			//int count = it -> size();
			//inIt = it->begin();
			areaSize = contourArea(*it);
			if (areaSize > maxAriaSize) {
				maxSizeIt = it;
				maxAriaSize = areaSize;
			}
		}

		inIt = maxSizeIt->begin();

		double x = 0;
		double y = 0;
		for (; inIt != maxSizeIt->end(); ++inIt) {
			x += inIt->x;
			y += inIt->y;
		}

		x /= maxSizeIt->size();
		y /= maxSizeIt->size();
		//inIt = maxSizeIt->begin();

		circle(src_img, Point(x, y), 50, Scalar(0, 0, 255), 3, 4);
		std::vector<std::vector<Point> > subset;
		subset.push_back(*maxSizeIt);
		//drawContours(src_img,subset , -1, Scalar(0, 0, 255), 4);

		imshow("bin", bin_img);
		imshow("Video", src_img);



	}




	return 0;
}
