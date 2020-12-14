#include <opencv2/opencv.hpp>
#include <iostream>
#include <unistd.h>
using namespace cv;
using namespace std;

const int max_lowThreshold = 100;
const int ratio_ = 3;
const int kernel_size = 5;

int main(int argc, char** argv){
    //get image
    String imageName("unnamed.png");
    Mat og = imread(imageName, IMREAD_COLOR);
    cvtColor(og,og,COLOR_BGR2GRAY);//grayscale
    //namedWindow("gray",0);
    //imshow("gray",image);
    //make sure input is valid
    if( og.empty() )
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    int min, max;
    /*while(true){
      cin >> min >> max;
      if(min == -1 || max == -1)
        break;
      Mat image=og.clone();
      threshold(image,image,min,max,THRESH_BINARY_INV);//first param input, 2nd param output
      namedWindow("thresh",0);
      imshow("thresh",image);
      waitKey(0);
    }*/
     GaussianBlur(og,og,Size(21,21),0,0);
     Canny( og, og, max_lowThreshold, /*max_lowThreshold*ratio_*/250, kernel_size );
     namedWindow("canny",0);
     imshow("canny",og);
     waitKey(0);
  	return 0;
}
