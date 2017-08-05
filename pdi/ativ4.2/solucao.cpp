#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat entrada, saida;
  int larg, altu;
  int nobjects,r,g,b;
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

  //Procedimento de rotulação dos objetos utilizando as 3 cores:
  cvtColor(entrada, saida, CV_GRAY2RGB);
  p.x=0;
  p.y=0;
  nobjects=0;
  r = 0;
  g = 0;
  b = 0;
  for(int i=0; i<altu; i++){
    for(int j=0; j<larg; j++){
      if( saida.at<Vec3b>(i,j)[0] == 255 && 
          saida.at<Vec3b>(i,j)[1] == 255 && 
          saida.at<Vec3b>(i,j)[2] == 255){
        //Achou um objeto nao rotulado:
        p.x=j;
  	  	p.y=i;
        nobjects++;
        g++;
        if(g>255){
          r++;
          g=0;
        }
        if(r>255){
          b++;
          r=0;
        }
        floodFill(saida,p,CV_RGB(r,g,b));
  	  }
  	}
  }

  //Exibindo resultados:
  cout << "A quantidade de bolhas foram: " << nobjects << endl;
  imshow("Entrada", entrada);
  imshow("Saida  ", saida);
  imwrite("resSolucao.png", saida);
  waitKey();
  return 0;
}
