#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, imageEqualizada;
  VideoCapture cap;

  cap.open(0);

  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }

  while(1){
    cap >> image;

    /// Convert to grayscale
    cvtColor( image, image, CV_BGR2GRAY );

    /// Apply Histogram Equalization
    equalizeHist( image, imageEqualizada );

    imshow( "Original", image );
    imshow( "Equalizada", imageEqualizada );

    if(waitKey(30) >= 0) break;
  }
  imwrite("Original.png",image);
  imwrite("Equalizada.png", imageEqualizada);

  return 0;
}
