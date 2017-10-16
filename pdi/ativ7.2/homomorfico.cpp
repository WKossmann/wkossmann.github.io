#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

#define RADIUS 20

using namespace cv;
using namespace std;

Mat filter, tmp;
int N, M;
char key;

int G0_NOW = 1;
int G0_MAX = 100;
int G1_NOW = 1;
int G1_MAX = 100;
int C0_NOW = 1;
int C0_MAX = 100;
int D0_NOW = 1;
int D0_MAX = 2000;

void deslocaDFT(Mat& image ){
  Mat A, B, C, D;

  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols/2;
  int cy = image.rows/2;

  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);
  C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B);
}

void atualizaFiltro(){
	float D2, G0, G1, D0;
	G0 = G0_NOW/10.0;
  G1 = G1_NOW/10.0;
	D0 = D0_NOW/10.0;

	tmp = Mat(M, N, CV_32F);
	for(int i=0; i<M ;i++){
		for(int j=0; j<N ;j++){
				D2 = ((float)i-M/2.0)*((float)i-M/2.0) + ((float)j-N/2.0)*((float)j-N/2.0);
				tmp.at<float>(i,j) = (G0-G1)*(1.0-exp(-1.0*(float)C0_NOW*(D2/(D0*D0))))+ G1;
			}
	}
  imshow("Filtro.png",tmp);

  Mat comps[]= {tmp, tmp};
  merge(comps, 2, filter);
}



int main(int argc , char** argv){
  Mat entrada, imag_compl, padded, filtered;
  int altu, larg;
  Mat_<float> realInput, zeros;
  vector<Mat> planes;

	if(argc != 2){
		cout << "Nenhum aquivo especificado. Encerrando ..." << endl;
    return(-1);
  }
	entrada = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  if(!entrada.data){
    cout << "Não foi possivel abrir o arquivo. Encerrando ..." << endl;
    return(-1);
  }
  imshow("Original", entrada);
  larg = entrada.cols;
  altu = entrada.rows;
  cout << "A imagem é de " << larg << " x " << altu << " pixels." << endl;
  
  //Cria bordas para calculo da DFT
  M = getOptimalDFTSize(altu);
  N = getOptimalDFTSize(larg);  
  copyMakeBorder(entrada, padded, 0, M - altu, 0, N - larg, BORDER_CONSTANT, Scalar::all(0));
  imag_compl = Mat(padded.size(), CV_32FC2, Scalar(0));
  zeros = Mat_<float>::zeros(padded.size());
  filter = imag_compl.clone();

  //Cria Original para controle do filtro:
  namedWindow("Original");
  createTrackbar( "G0", "Original",&G0_NOW,G0_MAX,NULL);
  createTrackbar( "G1", "Original",&G1_NOW,G1_MAX,NULL);
  createTrackbar( "C0", "Original",&C0_NOW,C0_MAX,NULL);
  createTrackbar( "D0", "Original",&D0_NOW,D0_MAX,NULL);
	atualizaFiltro();
  
  //Prepara para calculo da dft da imagem fonte (nescessario apenas uma vez)
  planes.clear();
  realInput = Mat_<float>(padded);
  log(realInput, realInput);

  planes.push_back(realInput);
  planes.push_back(zeros);

  merge(planes, imag_compl);

  //Calculo DFT:
  dft(imag_compl, imag_compl);
  deslocaDFT(imag_compl);
  

  while(true){
    //Multiplca a imagem com o filtro, ambos no dominio da frequencia (convolução no real)
    mulSpectrums(imag_compl,filter,filtered,0);
    deslocaDFT(filtered);

    //DTF inversa:
    idft(filtered, filtered);

    planes.clear();
    split(filtered, planes);

    normalize(planes[0], planes[0], 0, 1, CV_MINMAX);
    cv::exp(planes[0], planes[0]);
    normalize(planes[0], planes[0], 0, 1, CV_MINMAX);

    imshow("Filtrada", planes[0]);    

    if( (char)waitKey(30) == 27 ){
      break;
    }
      
    atualizaFiltro();
  }
  return 0;
}