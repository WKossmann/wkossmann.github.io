#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat entrada, saida;
  int larg, altu;
  int nobjects;
  CvPoint p;
  
  //Leitura do arquivo:
  entrada = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  if(!entrada.data){
    cout << "Não foi possivel abrir o arquivo. Encerrando ..." << endl;
    return(-1);
  }
  larg = entrada.cols;
  altu = entrada.rows;
  cout << "A imagem é de " << larg << " x " << altu << " pixels." << endl;


  // Retirar elementos das bordas:
  for(int i=0; i<altu; i++){
    if(entrada.at<uchar>(i,0) == 255){
      p.x=0;
      p.y=i;
      floodFill(entrada,p,0);
    }
    if(entrada.at<uchar>(i,larg-1) == 255){
      p.x=larg-1;
      p.y=i;
      floodFill(entrada,p,0);
    }
  }
  for(int j=0; j<larg; j++){
    if(entrada.at<uchar>(0,j) == 255){
      p.x=j;
      p.y=0;
      floodFill(entrada,p,0);
    }
    if(entrada.at<uchar>(altu-1,j) == 255){
      p.x=j;
      p.y=altu-1;
      floodFill(entrada,p,0);
    }
  }
  imshow("Passo 1", entrada);
  imwrite("resContaBuracos1.png", entrada);

  p.x=0;
  p.y=0;

  // Busca por bolha c ou sem buraco
  nobjects=0;
  for(int i=0; i<altu; i++){
    for(int j=0; j<larg; j++){
      if(entrada.at<uchar>(i,j) == 255){
  		  // achou um objeto
  		  nobjects++;
  		  p.x=j;
  		  p.y=i;
  		  floodFill(entrada,p,nobjects);
  	  }
  	}
  }
  imshow("Passo 2", entrada);
  imwrite("resContaBuracos2.png", entrada);

  // Preenche o fundo com com o ultimo ton de cinza encontrado +1 para poder encontrar os buracos posteriormente
  p.x = 0;
  p.y = 0;
  floodFill(entrada, p, nobjects+1);
  imshow("Passo 3", entrada);
  imwrite("resContaBuracos3.png", entrada);

  //Busca por buracos nas bolhas.
  int nburaco = 0;
  int pixAnterior=0;
  bool temBuraco=false;
  for(int k=nobjects; k>0; k--){
    for(int i=0; i<altu; i++){
      for(int j=0; j<larg; j++){
        if(pixAnterior==k){
          if(entrada.at<uchar>(i,j) == 0){
            temBuraco=true;
            p.x=j;
            p.y=i;
            floodFill(entrada,p,k+nobjects+1);
          }
        }
        pixAnterior = entrada.at<uchar>(i,j);
      }
    }
    if(temBuraco){
      nburaco++;
    }
    temBuraco=false;
  }

  cout << "Numero de bolhas total     : " << nobjects << endl;
  cout << "Numero de bolhas c/ buracos: " << nburaco  << endl;
  cout << "Numero de bolhas s/ buracos: " << nobjects - nburaco << endl;

  imshow("Passo   4", entrada);
  imwrite("resContaBuracos4.png", entrada);
  waitKey();
  return 0;
}