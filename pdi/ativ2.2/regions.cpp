#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat entrada;
  Vec3b val;
  int larg, altu;
  int p1x, p1y, p2x, p2y;

  //Leitura do arquivo:
  entrada = imread(argv[1],CV_LOAD_IMAGE_COLOR);
  if(!entrada.data){
    cout << "Não foi possivel abrir o arquivo. Encerrando ..." << endl;
  }
  larg = entrada.cols;
  altu = entrada.rows;
  cout << "A imagem é de " << larg << " x " << altu << " pixels." << endl;
  
  //Solicitando os pontos de interesse com travas nescessárias:
  cout << "Digite um ponto: ";
  while(1){
    cin >>  p1x >> p1y;
    if(p1x < larg && p1y < altu) break;
    cout << "O ponto deve estar dentro da imagem .." << endl;
  }
  cout << "Digite outro ponto: ";
  while(1){
    cin >>  p2x >> p2y;
    if(p2x < larg && p2y < altu) break;
    cout << "O ponto deve estar dentro da imagem .." << endl;
  }
  //Ordenando para facilitar preenchimento:
  if(p1x > p2x){
    int aux = p2x;
    p2x = p1x;
    p1x = aux;
  }
  if(p1y > p2y){
    int aux = p2y;
    p2y = p1y;
    p1y = aux;
  }

  //Fazendo o negativo da área determinada pelos pontos:
  for(int i=p1x;i<=p2x;i++){
    for(int j=p1y;j<=p2y;j++){
      entrada.at<Vec3b>(i,j)[0] = 255 - entrada.at<Vec3b>(i,j)[0];
      entrada.at<Vec3b>(i,j)[1] = 255 - entrada.at<Vec3b>(i,j)[2];
      entrada.at<Vec3b>(i,j)[2] = 255 - entrada.at<Vec3b>(i,j)[2];
    }
  }
  
  //Exibindo resultados.
  imshow("Principal", entrada);  
  waitKey();

  return 0;
}
