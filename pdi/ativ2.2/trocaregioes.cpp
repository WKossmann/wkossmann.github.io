#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat entrada, saida;
  Vec3b val;
  int larg, altu;

  //Leitura do arquivo:
  entrada = imread(argv[1],CV_LOAD_IMAGE_COLOR);
  if(!entrada.data){
    cout << "Não foi possivel abrir o arquivo. Encerrando ..." << endl;
  }
  larg = entrada.cols;
  altu = entrada.rows;
  cout << "A imagem é de " << larg << " x " << altu << " pixels." << endl;
  
  //Clonando a imagem em outra matriz para alterar os valores sem perder informação.
  saida = entrada.clone();
  
  for(int i=0; i<larg/2;i++){
    for(int j=0;j<altu/2;j++){
      saida.at<Vec3b>(i,j)[0] = entrada.at<Vec3b>(i+larg/2, j+altu/2)[0];
      saida.at<Vec3b>(i,j)[1] = entrada.at<Vec3b>(i+larg/2, j+altu/2)[1];
      saida.at<Vec3b>(i,j)[2] = entrada.at<Vec3b>(i+larg/2, j+altu/2)[2];
    }
  }
  for(int i=0; i<larg/2;i++){
    for(int j=altu/2;j<altu;j++){
      saida.at<Vec3b>(i,j)[0] = entrada.at<Vec3b>(i+larg/2, j-altu/2)[0];
      saida.at<Vec3b>(i,j)[1] = entrada.at<Vec3b>(i+larg/2, j-altu/2)[1];
      saida.at<Vec3b>(i,j)[2] = entrada.at<Vec3b>(i+larg/2, j-altu/2)[2];
    }
  }
  for(int i=larg/2; i<larg;i++){
    for(int j=0;j<altu/2;j++){
      saida.at<Vec3b>(i,j)[0] = entrada.at<Vec3b>(i-larg/2, j+altu/2)[0];
      saida.at<Vec3b>(i,j)[1] = entrada.at<Vec3b>(i-larg/2, j+altu/2)[1];
      saida.at<Vec3b>(i,j)[2] = entrada.at<Vec3b>(i-larg/2, j+altu/2)[2];
    }
  }
  for(int i=larg/2; i<larg;i++){
    for(int j=altu/2;j<altu;j++){
      saida.at<Vec3b>(i,j)[0] = entrada.at<Vec3b>(i-larg/2, j-altu/2)[0];
      saida.at<Vec3b>(i,j)[1] = entrada.at<Vec3b>(i-larg/2, j-altu/2)[1];
      saida.at<Vec3b>(i,j)[2] = entrada.at<Vec3b>(i-larg/2, j-altu/2)[2];
    }
  }

  //Exibindo resultados.
  imshow("Entrada", entrada);
  imshow("Saida", saida);
  imwrite( "resTrocaRegioes.png", saida);  
  waitKey();

  return 0;
}
