//attempt #2 w/ RA
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <unistd.h>
using namespace cv;
using namespace std;

int main(int argc, char** argv){
    //get image
    String imageName("pix.png");
    Mat og = imread(imageName, IMREAD_COLOR);
    cvtColor(og,og,COLOR_BGR2GRAY);
    cout << (int)og.at<uchar>(0,0) << endl;
    cout << (int)og.at<uchar>(1,0) << endl;//y,x
    cout << (int)og.at<uchar>(0,1) << endl;
    cout << (int)og.at<uchar>(1,1) << endl;
    namedWindow("test",0);
    imshow("test",og);
    waitKey(0);
  	return 0;
}
