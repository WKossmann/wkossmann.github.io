#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes;
  Mat histP, histA;
  int nbins = 64;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;

  cap.open(0);
  
  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }
  
  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  cap >> image;
  split (image, planes);

  calcHist(&planes[0], 1, 0, Mat(), histA, 1,
           &nbins, &histrange,
           uniform, acummulate);
  int cnt = 0;
  while(1){
    cap >> image;
    split (image, planes);

    calcHist(&planes[0], 1, 0, Mat(), histP, 1,
             &nbins, &histrange,
             uniform, acummulate);

    long int soma = 0;
    for(int i=0; i<nbins; i++){
      soma += abs(histP.at<float>(i) - histA.at<float>(i));
    }
    if(soma/nbins > 220){
      cout << "ALERTA " << cnt << endl;
      cnt++;
      if(cnt>10000) cnt=0;
    }

    histA = histP.clone();

    imshow("image", image);
    if(waitKey(30) >= 0) break;
  }
  return 0;
}
