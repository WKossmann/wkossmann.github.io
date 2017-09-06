#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

#define QTD_MEDIAS 20
#define AUM_SATPAD 1.3
#define RANGE 0.3

#define SLIDER_MAX 100


using namespace cv;
using namespace std;

char TrackbarName[50];

float alfa;
int altReg_slider = 0;
int decaim_slider = 0;
int posica_slider = 0;

Mat desfoque, desfoqueInv, imOriginal, imBorrada, imSaturada, resultadoFinal;
float altReg, d, posVert;

int lar, alt;


void geraImagemBorrada(){
	Mat aux1,aux2,passo1,passo2;
	float media[] = { 1,1,1,
				   	  1,1,1,
				   	  1,1,1 };
	Mat mask(3,3,CV_32F, media), mask1;
	scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask1);
	swap(mask, mask1);
	
	aux1 = imOriginal.clone();
	aux1.convertTo(passo1, CV_32F);

	for(int k=0;k<QTD_MEDIAS;k++){
    	filter2D(passo1, passo2, passo1.depth(), mask, Point(1,1), 0);
    	passo2 = abs(passo2);
    	passo1 = passo2.clone();
	}

	aux2 = passo2.clone();
    aux2.convertTo(imBorrada, CV_8U);
}
void aumentaSaturacao(){
	imSaturada = imOriginal.clone();

  	cvtColor(imOriginal  , imOriginal  , CV_RGB2HSV);
	cvtColor(imSaturada, imSaturada, CV_RGB2HSV);

	for(int i=0; i<alt ; i++){
		for(int j=0; j<lar ; j++){
			// imSaturada.at<Vec3b>(i,j)[1] = imOriginal.at<Vec3b>(i,j)[1] * 1.5;
			// if(imSaturada.at<Vec3b>(i,j)[1] > 255){
			// 	imSaturada.at<Vec3b>(i,j)[1] = 255;
			// }
			if(imOriginal.at<Vec3b>(i,j)[1] <= 100){
				imSaturada.at<Vec3b>(i,j)[1] = imOriginal.at<Vec3b>(i,j)[1] * (AUM_SATPAD + RANGE);
			}
			if(imOriginal.at<Vec3b>(i,j)[1] <= 200 && imOriginal.at<Vec3b>(i,j)[1] > 100){
				imSaturada.at<Vec3b>(i,j)[1] = imOriginal.at<Vec3b>(i,j)[1] * AUM_SATPAD;
			}
			if(imOriginal.at<Vec3b>(i,j)[1] > 200){
				imSaturada.at<Vec3b>(i,j)[1] = imOriginal.at<Vec3b>(i,j)[1] * (AUM_SATPAD - RANGE);
			}
		}
	}
	
	cvtColor(imOriginal  , imOriginal  , CV_HSV2RGB);
	cvtColor(imSaturada, imSaturada, CV_HSV2RGB);
}
// Função para a multiplicacao de uma imagem por uma imagem de ponderacao.
void multiplicaPonderacao(){
	for(int i=0; i<alt ; i++){
		for(int j=0; j<lar ; j++){
			imSaturada.at<Vec3b>(i,j)[0] = (int)((float)imSaturada.at<Vec3b>(i,j)[0] * ((float)desfoque.at<uchar>(i,j)/255.0));
			imSaturada.at<Vec3b>(i,j)[1] = (int)((float)imSaturada.at<Vec3b>(i,j)[1] * ((float)desfoque.at<uchar>(i,j)/255.0));
			imSaturada.at<Vec3b>(i,j)[2] = (int)((float)imSaturada.at<Vec3b>(i,j)[2] * ((float)desfoque.at<uchar>(i,j)/255.0));
 		
 			imBorrada.at<Vec3b>(i,j)[0] = (int)((float)imBorrada.at<Vec3b>(i,j)[0] * ((float)desfoqueInv.at<uchar>(i,j)/255.0));
			imBorrada.at<Vec3b>(i,j)[1] = (int)((float)imBorrada.at<Vec3b>(i,j)[1] * ((float)desfoqueInv.at<uchar>(i,j)/255.0));
			imBorrada.at<Vec3b>(i,j)[2] = (int)((float)imBorrada.at<Vec3b>(i,j)[2] * ((float)desfoqueInv.at<uchar>(i,j)/255.0));
 		}
	}
}
// Função que gera a matriz de ponderacao e sua inversa a partir dos dados que serão recebidos pelas barras de rolagem.
void geraPonderacao(){
  	float x;
  	float conta;
  	float altReg2, posVert2, d2;

  	//im_PonderacaoInv = im_Ponderacao.clone();

  	altReg2 = altReg * alt;
  	posVert2 = posVert * alt;
  	d2 = d * alt;
  	float l1 = alt - posVert2 - altReg2/2 ;
  	float l2 = alt - posVert2 + altReg2/2 ;
  	
  	for(int i=0; i<alt ; i++){
		for(int j=0; j<lar ; j++){
			x = i;
			conta = (int)((0.5)*(tanh((x-l1)/d2) - tanh((x-l2)/d2))*255.0);

			desfoque.at<uchar>(i,j)    = conta;
			desfoqueInv.at<uchar>(i,j) = 255 - conta;
		}
	}
}
void alteraImagem(){
	//Prepara a matriz de ponderacao de acordo com a imagem original:
	desfoque    = imOriginal.clone();
	desfoqueInv = imOriginal.clone();
	cvtColor(desfoque   , desfoque   , CV_RGB2GRAY);
	cvtColor(desfoqueInv, desfoqueInv, CV_RGB2GRAY);

	//Ramo 01: Gera a matriz saturada:
	aumentaSaturacao();

	//Ramo 02: Gera a matriz borrada:
	geraImagemBorrada();

	geraPonderacao();
	multiplicaPonderacao();
	addWeighted(imSaturada, 1, imBorrada, 1, 0.0, resultadoFinal);

	imshow("Resultado Final", resultadoFinal);
}
void track_altReg(int, void*){
	alfa = (float) altReg_slider/SLIDER_MAX ;
	cout << alfa << endl;
	
	altReg = alfa;

	alteraImagem();
}
void track_decaim(int, void*){
	alfa = (float) decaim_slider/SLIDER_MAX ;
	cout << alfa << endl;
	
	d = alfa;

	alteraImagem();
}
void track_posica(int, void*){
	alfa = 1 - (float) posica_slider/SLIDER_MAX ;
	cout << alfa << endl;
	
	posVert = alfa;

	alteraImagem();
}


int main(){

	//Leitura da imagem original:
	imOriginal = imread("foto_evento.jpg");
	imshow("Imagem Original",imOriginal);
	lar=imOriginal.size().width;
  	alt=imOriginal.size().height;
	
	

	namedWindow("Resultado Final", 1);

	

	//Config Barras de controle:
	sprintf( TrackbarName, "Tamanho da Regiao x %d", SLIDER_MAX );
	createTrackbar( TrackbarName, "Resultado Final",
				  	&altReg_slider,
				  	SLIDER_MAX,
				  	track_altReg);
	track_altReg(altReg_slider, 0 );

	sprintf( TrackbarName, "Tamanho do Decaimento x %d", SLIDER_MAX );
	createTrackbar( TrackbarName, "Resultado Final",
			  		&decaim_slider,
				  	SLIDER_MAX,
					track_decaim );
	track_decaim(decaim_slider, 0 );

	sprintf( TrackbarName, "Posicao Vertical x %d", SLIDER_MAX );
	createTrackbar( TrackbarName, "Resultado Final",
			  		&posica_slider,
				  	SLIDER_MAX,
					track_posica );
	track_posica(posica_slider, 0 );

	waitKey();
	imwrite("Resultado.png", resultadoFinal);
}