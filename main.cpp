#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int fd;
int color_threshold = 30, location_threshold = 40, angle_threshold = 2;
int h1 = 0, s1 = 166, v1 = 59;
int h2 = 16, s2 = 141, v2 = 255;

Mat src, src_gray;
int thresh = 100, max_thresh = 255;
RNG rng(12345);

void thresh_callback(int, void *);
/*
 * Controls to move the arduino:
 * w := forward
 * s := backward
 * d := right
 * a := left
 */
void sendCommand(const char *command) {
	write(fd, command, 1);
}

int connectArduino(const char *port) {
	fd = open(port, O_RDWR | O_NOCTTY);
	printf("Started the connection to arduino\n");
	return 0;
}

int main(int argc, char **argv)
{
	VideoCapture cap(1);
	if (!cap.isOpened()) {
		printf("ERROR!\n");
		return -1;
	}
	namedWindow("Trackbars");
	createTrackbar("h1", "Trackbars", &h1, 255);
	createTrackbar("s2", "Trackbars", &s2, 255);
	createTrackbar("v1", "Trackbars", &v1, 255);
	createTrackbar("h2", "Trackbars", &h2, 255);
	createTrackbar("s2", "Trackbars", &s2, 255);
	createTrackbar("v2", "Trackbars", &v2, 255);
	connectArduino(argv[1]);

	while (1) {
		Mat imageHsv;
		Mat finalImg;
		const char *source_window = "Source";

		cap >> src;
		cvtColor(src, imageHsv, CV_BGR2HSV);
		inRange(imageHsv, Scalar(h1, s1, v1), Scalar(h2, v2, s2), src_gray);
		imshow("ragna", src_gray);

		blur(src_gray, src_gray, Size(3, 3));
		namedWindow(source_window, CV_WINDOW_AUTOSIZE);
		imshow(source_window, src);

		createTrackbar(" Threshold: ", "Source", &thresh, max_thresh,
					   thresh_callback);
		thresh_callback(0, NULL);

		waitKey(27);
	}
	close(fd);

	return 0;
}

void thresh_callback(int a, void *b)
{
	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Point center_rect;
	float area = INT_MAX;
	int img_x, obj_x;

	/* Detect edges using Threshold */
	threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);

	/* Find contours */
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE,
				 CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/* Approximate contours to polygons + get bounding rects and circles */
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect;
	for (int i = 0; i < contours.size(); i++) {
		if (contourArea(contours[i]) > 10000 &&
			contourArea(contours[i]) < 50000) {
				approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
				boundRect.push_back(boundingRect(Mat(contours_poly[i])));
				area = contourArea(contours[i]);
		}
	}
	if (boundRect.size() == 0) {
		/* Send character 's' to device */
		sendCommand("s");
	}

	/* Draw polygon contour + bounding rects + circles */
	Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
	for (int i = 0; i < boundRect.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
							  rng.uniform(0, 255));
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2,
				  8, 0);
		center_rect = (boundRect[i].tl() + boundRect[i].tl());
		center_rect.x /= 2;
		center_rect.y /= 2;
		circle(drawing, center_rect, 10, Scalar(255, 0, 0), CV_FILLED, 8, 0);
	}

	/* Main logic */
	img_x = src.cols / 2;
	obj_x = center_rect.x;
	if (img_x - 100 > obj_x && 0 < obj_x) {
		cout << "Go left" << endl;
		sendCommand("a");
	}
	else if (img_x + 100 < obj_x) {
		cout << "Go right" << endl;
		sendCommand("d");
	}
	else {
		if (area < 30000) {
			cout << "Straight" << endl;
			sendCommand("w");
		}
		else {
			sendCommand("s");
		}
	}

	/* Show in a window */
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
	waitKey(150);
}
