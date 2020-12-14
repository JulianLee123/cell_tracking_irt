#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <unistd.h>

using namespace cv;
using namespace std;

#define columns 996
#define rows 738

int intensity(int y, int x, Mat im){
  return (int)im.at<uchar>(Point(x,y));
}

/*void findPeaks(int pixVal, Mat im){//for now pixVal is the y value, averaging together 11 values: 11 = minimum width of airy ring
  //change 15, 11, 101, thresh
  Mat im2=im.clone();
  int numSmall = 11, numLarge = 101;
  namedWindow("og",0);
  imshow("og",im);
  for(pixVal = 0; pixVal < rows - 1; pixVal++){
    int thresh = 5;
    long integral[columns] = {0}, average = 0;//average of all values
    for(int i = 0; i < columns; i++)
      average += intensity(i,pixVal,im);
    average /= columns;
    integral[0] = intensity(0,pixVal,im);
    for(int i = 1; i < columns; i++){
      integral[i] = intensity(i,pixVal,im) + integral[i-1];
    }
   //sum cancels noise as in grainy, largeSum cancels lighting noise (eg. bright spot)
    int sum[columns] = {0};//sum is sum of 11 integers around n, first and last 11/2 undefined
    long long largeSum[columns] = {0};// largeSum is sum of 101 integers around n, makes up for deviations in background
    for(int i = 0; i < numSmall; i++){
      sum[numSmall/2] += (int)im.at<uchar>(pixVal,i);
    }
    for(int i = numSmall/2+1; i < columns - numSmall/2; i++){
      sum[i] = sum[i-1] - (int)im.at<uchar>(pixVal,i-numSmall/2-1) + (int)im.at<uchar>(pixVal,i+numSmall/2);
    }
    for(int i = 0; i < numLarge; i++){
      largeSum[numLarge/2] += (int)im.at<uchar>(pixVal,i);
    }
    for(int i = numLarge/2+1; i < columns - numLarge/2; i++){
      largeSum[i] = largeSum[i-1] - (int)im.at<uchar>(pixVal,i-numLarge/2-1) + (int)im.at<uchar>(pixVal,i+numLarge/2);
    }
    for(int i = numLarge/2+1; i < columns - numLarge/2-1; i++){//testing each point
      if(sum[i-1] <= sum[i] && sum[i+1] <= sum[i]){//some kind of peak
        //cout << i << " " << intensity(i,pixVal,im) -largeSum[i]/101 << endl;//the larger this is, the stronger the peak
        if(sum[i]/numSmall -largeSum[i]/numLarge > thresh){//intensity(i,pixVal,im)
           im.at2<uchar>(pixVal,i) = 255;
           cout << i << " " << sum[i]/numSmall << " " << largeSum[i]/numLarge << endl;
         }
           //if(pixVal == 460)
            //circle(im, Point(i,pixVal),1,255);
      }
      else if(sum[i]/11 > average + 15){//for long flat curves
         //im.at<uchar>(pixVal,i) = 255;
        //if(pixVal == 460)
        //circle(im,Point(i,pixVal),1,255);
      }
  }
  //circle(im, Point(280,500),10,255);
  //rectangle(im, Point(0,500), Point(columns,500), 255);
}
  namedWindow("a",0);
  imshow("a",im2);
}*/

void findPeaks(int pixVal, Mat im, int numSmall = 11, int numLarge = 101, float thresh = 5){//for now pixVal is the y value, averaging together 11 values: 11 = minimum width of airy ring
  //borderline case marked when adjacent pixels = 255
  float vals[rows][columns];
  Mat im2=im.clone();
  namedWindow("og",0);
  imshow("og",im);
  for(pixVal = 0; pixVal < rows - 1; pixVal++){
    long integral[columns] = {0}, average = 0;//average of all values
    for(int i = 0; i < columns; i++)
      average += (int)im.at<uchar>(pixVal,i);
    average /= columns;
    integral[0] = (int)im.at<uchar>(pixVal,0);
    for(int i = 1; i < columns; i++){
      integral[i] = (int)im.at<uchar>(pixVal,i) + integral[i-1];
    }
   //sum cancels noise as in grainy, largeSum cancels lighting noise (eg. bright spot)
    int sum[columns] = {0};//sum is sum of 11 integers around n, first and last 11/2 undefined
    long long largeSum[columns] = {0};// largeSum is sum of 101 integers around n, makes up for deviations in background
    for(int i = 0; i < numSmall; i++){
      sum[numSmall/2] += (int)im.at<uchar>(pixVal,i);
    }
    for(int i = numSmall/2+1; i < columns - numSmall/2; i++){
      sum[i] = sum[i-1] - (int)im.at<uchar>(pixVal,i-numSmall/2-1) + (int)im.at<uchar>(pixVal,i+numSmall/2);
    }
    for(int i = 0; i < numLarge; i++){
      largeSum[numLarge/2] += (int)im.at<uchar>(pixVal,i);
    }
    for(int i = numLarge/2+1; i < columns - numLarge/2; i++){
      largeSum[i] = largeSum[i-1] - (int)im.at<uchar>(pixVal,i-numLarge/2-1) + (int)im.at<uchar>(pixVal,i+numLarge/2);
    }
    for(int i = numLarge/2+1; i < columns - numLarge/2-1; i++){//testing each point
      if(sum[i-1] <= sum[i] && sum[i+1] <= sum[i]){//some kind of peak: change?
        if(/*intensity(i,pixVal,im)*/sum[i]/numSmall -largeSum[i]/numLarge > thresh){//change?
           im2.at<uchar>(pixVal,i) = 255;
           //cout << i << " " << sum[i]/numSmall << " " << largeSum[i]/numLarge << endl;
         }
      }
    }
  }
  for(pixVal = 0; pixVal < columns - 1; pixVal++){
    long integral[rows] = {0}, average = 0;//average of all values
    for(int i = 0; i < rows; i++)
      average += (int)im.at<uchar>(i,pixVal);
    average /= rows;
    integral[0] = (int)im.at<uchar>(0,pixVal);
    for(int i = 1; i < rows; i++){
      integral[i] = (int)im.at<uchar>(i,pixVal) + integral[i-1];
    }
   //sum cancels noise as in grainy, largeSum cancels lighting noise (eg. bright spot)
    int sum[rows] = {0};//sum is sum of 11 integers around n, first and last 11/2 undefined
    long long largeSum[rows] = {0};// largeSum is sum of 101 integers around n, makes up for deviations in background
    for(int i = 0; i < numSmall; i++){
      sum[numSmall/2] += (int)im.at<uchar>(i,pixVal);
    }
    for(int i = numSmall/2+1; i < rows - numSmall/2; i++){
      sum[i] = sum[i-1] - (int)im.at<uchar>(i-numSmall/2-1,pixVal) + (int)im.at<uchar>(i+numSmall/2,pixVal);
    }
    for(int i = 0; i < numLarge; i++){
      largeSum[numLarge/2] += (int)im.at<uchar>(i,pixVal);
    }
    for(int i = numLarge/2+1; i < rows - numLarge/2; i++){
      largeSum[i] = largeSum[i-1] - (int)im.at<uchar>(i-numLarge/2-1,pixVal) + (int)im.at<uchar>(i+numLarge/2,pixVal);
    }
    for(int i = numLarge/2+1; i < rows - numLarge/2-1; i++){//testing each point
      if(sum[i-1] <= sum[i] && sum[i+1] <= sum[i]){//some kind of peak
        float x = /*intensity(i,pixVal,im)*/sum[i]/(float)numSmall -largeSum[i]/(float)numLarge;
        if(x > thresh){
           im2.at<uchar>(i,pixVal) = 255;
           //cout << i << " " << sum[i]/numSmall << " " << largeSum[i]/numLarge << endl;
         }
         //else if(x > thresh - 3 && im2.at<uchar>(i,pixVal) )
      }
    }
  }
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < columns; c++){
      if(im2.at<uchar>(r,c) != 255){
        im2.at<uchar>(r,c) = 0;
      }
    }
  }
  namedWindow("no blur",0);
  imshow("no blur",im2);
  GaussianBlur(im2,im2,Size(9,9),0);


  namedWindow("a",0);
  imshow("a",im2);
}

int main(int argc, char** argv){
    ofstream fout;
    fout.open("data.csv");
    String imageName("unnamed.png");
    Mat og = imread(imageName, IMREAD_COLOR);
    cvtColor(og,og,COLOR_BGR2GRAY);//grayscale
    Mat newIm=og.clone();
    //GaussianBlur(og,og,Size(9,9),0);
    //namedWindow("gray",0);
    //imshow("gray",image);
    //make sure input is valid
    if( og.empty() )
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    Mat image=og.clone();
    //Canny(image,image,300,300,5);
    //findPeaks(500);
    for(int x = 0; x < og.cols; x++){
      fout << (int)og.at<uchar>(250,x) << endl;
    }
    cout << endl << endl;
    findPeaks(500,og,11,101,4.5);
    //findPeaks(500,newIm,"b");
    //namedWindow("test",0);
    //imshow("test",image);
    waitKey(0);
  	return 0;
}
