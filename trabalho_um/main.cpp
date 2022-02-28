#include <iostream>
#include <cstdlib>
#include <locale>
#include <opencv2/opencv.hpp>
#include "cvui.h"

using namespace cv;
using namespace std;

Mat3b rotacionar_imagem_colorida(Mat3b src, int choice)// 0-> vertical | 1 -> horizontal
{
	Mat3b dst(src.rows, src.cols);

	if (choice == 0)
	{
		for (int a = 0; a < dst.rows; ++a) {
			for (int b = 0; b < dst.cols; ++b) {
				dst(a, b) = src(a, src.cols - 1 - b); //inverte os pixels de cada coluna com a lógica de que o negativo será o número máximo disponivel(lado oposto)
			}
		}
		return dst;
	}
	else
	{
		src = src.t(); // faz a transposição da matriz
		for (int a = 0; a < dst.rows; ++a) {
			for (int b = 0; b < dst.cols; ++b) {
				dst(a, b) = src(a, src.cols - 1 - b); //inverte os pixels de cada coluna com a lógica de que o negativo será o número máximo disponivel(lado oposto)
			}
		}
		src = src.t(); // retorna a transposição da matriz (precisei retornar, pois gerava bugs se ignorasse)
		dst = dst.t(); // retorna a transposição da matriz

		return dst;
	}
}

Mat1b rotacionar_imagem_cinza(Mat1b src, int choice)// 0-> vertical | 1 -> horizontal
{
	Mat1b dst(src.rows, src.cols);

	if (choice == 0)
	{
		for (int a = 0; a < dst.rows; ++a) {
			for (int b = 0; b < dst.cols; ++b) {
				dst(a, b) = src(a, src.cols - 1 - b); //inverte os pixels de cada coluna com a lógica de que o negativo será o número máximo disponivel(lado oposto)
			}
		}
		return dst;
	}
	else
	{
		src = src.t(); // faz a transposição da matriz
		for (int a = 0; a < dst.rows; ++a) {
			for (int b = 0; b < dst.cols; ++b) {
				dst(a, b) = src(a, src.cols - 1 - b); //inverte os pixels de cada coluna com a lógica de que o negativo será o número máximo disponivel(lado oposto)
			}
		}
		src = src.t(); // retorna a transposição da matriz (precisei retornar, pois gerava bugs se ignorasse)
		dst = dst.t(); // retorna a transposição da matriz

		return dst;
	}
}

Mat1b transformar_em_cinza(Mat img)
{
	if (img.channels() == 3)
	{
		Mat3b src = img;
		Mat1b dst(src.rows, src.cols);

		for (int a = 0; a < dst.rows; ++a) {
			for (int b = 0; b < dst.cols; b++)
			{
				dst[a][b] = src[a][b][0] * 0.299 + src[a][b][1] * 0.587 + src[a][b][2] * 0.114;
			}
		}

		return dst;
	}
	else
	{
		Mat1b dst = img;
		return dst;
	}
}

Mat1b quantizacao_cinza(Mat imagem, int n)
{
	Mat1b img = imagem;
	Mat1b dst(img.rows, img.cols);
	int divisor = 256 / n;
	int max_quantized_value = 255 / divisor;

	for (int a = 0; a < img.rows; ++a) {
		for (int b = 0; b < img.cols; b++)
		{
			dst[a][b] = ((img[a][b] / divisor) * 255) / max_quantized_value;
		}
	}

	return dst;

}

Mat3b quantizacao_colorido(Mat imagem, int n)
{
	Mat3b img = imagem;
	Mat3b dst(img.rows, img.cols);
	int divisor = 256 / n;
	int max_quantized_value = 255 / divisor;

	for (int a = 0; a < img.rows; ++a) {
		for (int b = 0; b < img.cols; b++)
		{
			dst[a][b][0] = ((img[a][b][0] / divisor) * 255) / max_quantized_value;
			dst[a][b][1] = ((img[a][b][1] / divisor) * 255) / max_quantized_value;
			dst[a][b][2] = ((img[a][b][2] / divisor) * 255) / max_quantized_value;
		}
	}
	return dst;
}




int main(int argc, char** argv)
{
	setlocale(LC_ALL, "Portuguese"); // evita erros
	Mat img;
	bool imagem_base_aberta = false; // dita se uma imagem foi fornecida ou nao ao programa

	if (argc > 2)	// caso abrimos mais de uma imagem com o programa
	{
		cerr << endl << "ERRO::Abra somente uma imagem com o programa!\n";
		fflush(stdin);
		getchar();
		exit(EXIT_FAILURE);
	}
	if (argc < 2) // caso executamos o programa sem imagem
	{
		cerr << endl << "ERRO::Executavel aberto sem imagens. A imagem base sera utilizada.";
		img = imread("./lenna.png", 1);
		imagem_base_aberta = true;
	}
	else // programa sendo executado com abertura de imagem
	{
		string nome_arquivo = argv[1];
		img = imread(samples::findFile(nome_arquivo), IMREAD_COLOR);
	}


	if (img.data == nullptr)   // imagem nula
	{
		cerr << endl << "ERRO::Arquivo de invalido. Nao e uma imagem!\n";
		fflush(stdin);
		getchar();
		exit(EXIT_FAILURE);
	}


	Mat img_base = img;   // copia da imagem principal -> utilizada para "ter para onde voltar"
	int count = 2;	      // contador para quantizacao


	Mat frame_menu(250, 400, CV_8UC3);
	Mat frame_segunda_imagem(img.rows, img.cols, CV_8UC3); //frames


	cvui::init("menu");


	while (true) {
		frame_menu = Scalar(49, 52, 49);                // pintando o frame

		Mat1b imagem_auxiliar_1_canais(img.rows, img.cols);
		Mat1b imagem_branca_1_canais(img.rows, img.cols);

		Mat3b imagem_auxiliar_3_canais(img.rows, img.cols);
		Mat3b imagem_branca_3_canais(img.rows, img.cols);


		if (cvui::button(frame_menu, 10, 10, "Copiar Imagem"))        // copia a imagem base para o quadro de edicoes
		{
			img = img_base;
			img.copyTo(frame_segunda_imagem);

		}

		if (cvui::button(frame_menu, 10, 40, "Rotacionar Vertical"))
		{
			if (img.channels() == 3) { // garante que só imagens coloridas acessarão a função
				imagem_auxiliar_3_canais = rotacionar_imagem_colorida(img, 0);
				imagem_auxiliar_3_canais.copyTo(frame_segunda_imagem);

				img = imagem_auxiliar_3_canais;				// -> alteram para a proxima função, pois todas funções são inicializadas com img
				imagem_auxiliar_3_canais = imagem_branca_3_canais;
			}
			else
			{ // garante que só imagens em tons de cinza acessarão a função
				imagem_auxiliar_1_canais = rotacionar_imagem_cinza(img, 0);
				imagem_auxiliar_1_canais.copyTo(frame_segunda_imagem);

				img = imagem_auxiliar_1_canais;				// -> alteram para a proxima função, pois todas funções são inicializadas com img
				imagem_auxiliar_1_canais = imagem_branca_1_canais;
			}
		}

		if (cvui::button(frame_menu, 10, 70, "Rotacionar Horizontal"))
		{
			if (img.channels() == 3) { // garante que só imagens coloridas acessarão a função
				imagem_auxiliar_3_canais = rotacionar_imagem_colorida(img, 1);
				imagem_auxiliar_3_canais.copyTo(frame_segunda_imagem);

				img = imagem_auxiliar_3_canais;				// -> alteram para a proxima função, pois todas funções são inicializadas com img
				imagem_auxiliar_3_canais = imagem_branca_3_canais;
			}
			else
			{ // garante que só imagens em tons de cinza acessarão a função
				imagem_auxiliar_1_canais = rotacionar_imagem_cinza(img, 1);
				imagem_auxiliar_1_canais.copyTo(frame_segunda_imagem);

				img = imagem_auxiliar_1_canais;				// -> alteram para a proxima função, pois todas funções são inicializadas com img
				imagem_auxiliar_1_canais = imagem_branca_1_canais;
			}
		}

		if (cvui::button(frame_menu, 10, 100, "Converter para tons de cinza"))
		{
			if (img.channels() == 3) { // garante que só imagens coloridas acessarão a função
				imagem_auxiliar_1_canais = transformar_em_cinza(img);
				imagem_auxiliar_1_canais.copyTo(frame_segunda_imagem);
				img = imagem_auxiliar_1_canais;				// -> alteram para a proxima função, pois todas funções são inicializadas com img
				imagem_auxiliar_1_canais = imagem_branca_1_canais;
			}
		}

		cvui::counter(frame_menu, 150, 130, &count); // Contador para a quantizacao
		if (count > 255)
			count = 255;
		if (count < 2)
			count = 2;

		if (cvui::button(frame_menu, 10, 130, "Quantizacao"))
		{
			if (img.channels() == 3) { // garante que só imagens coloridas acessarão a função
				imagem_auxiliar_3_canais = quantizacao_colorido(img, count);
				imagem_auxiliar_3_canais.copyTo(frame_segunda_imagem);


				img = imagem_auxiliar_3_canais;				// -> alteram para a proxima função, pois todas funções são inicializadas com img
				imagem_auxiliar_3_canais = imagem_branca_3_canais;

				/*img_quantizacao_alterada = img;
				img_quantizacao_alterada = quantizacao_colorido(img_quantizacao_alterada, count);
				img_quantizacao_alterada.copyTo(frame_segunda_imagem);*/

			}
			else
			{ // garante que só imagens em tons de cinza acessarão a função
				imagem_auxiliar_1_canais = quantizacao_cinza(img, count);
				imagem_auxiliar_1_canais.copyTo(frame_segunda_imagem);

				img = imagem_auxiliar_1_canais;				// -> alteram para a proxima função, pois todas funções são inicializadas com img
				imagem_auxiliar_1_canais = imagem_branca_1_canais;
			}
		}

		if (cvui::button(frame_menu, 10, 160, "Salvar imagem em jpg"))
		{
			imwrite("imagem_saida.jpg", img);
		}

		if (cvui::button(frame_menu, 200, 160, "Salvar imagem em png"))
		{
			imwrite("imagem_saida.png", img);
		}

		if (cvui::button(frame_menu, 10, 190, "Finalizar programa"))
		{
			break;
		}

		if (imagem_base_aberta)
			cvui::text(frame_menu, 10, 230, "Programa executando com imagem base", 0.4, 0xff0000);
		else
			cvui::text(frame_menu, 10, 230, "Programa executando com imagem fornecida pelo usuario", 0.4, 0xff0000);


		cvui::update();

		imshow("menu", frame_menu);
		imshow("imagem principal", img_base);
		imshow("imagem copiada", frame_segunda_imagem);
		fflush(stdin);
		waitKey(20);
	}

	return 0;
}
