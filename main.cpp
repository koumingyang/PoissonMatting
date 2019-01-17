#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include "color.h"
#include "image.h"
#include "gauss.h"

using namespace std;

double alpha=10;

//variables controlling the quantization based acceleration.
bool quantize = false;
bool quick_mode = true;
int q_colors = 64;

//theta is in radians.
double theta = 3.14/4.0;

double sum_gauss = 0.0;
int gauss_n = 9;
int r = 0;

const double d2r = 3.14159 / 180.0;
const double r2d = 180.0 / 3.14159;

void gauss_init(void)
{
	int i, j;
	for (i = -gauss_n / 2; i <= gauss_n / 2; i++)
	{
		for (j = -gauss_n / 2; j <= gauss_n / 2; j++)
			sum_gauss += gauss(i, j);
	}
	cout << sum_gauss << endl;
	for (i = -gauss_n / 2; i <= gauss_n / 2; i++)
	{
		for (j = -gauss_n / 2; j <= gauss_n / 2; j++)
				printf("%.5lf\t", gauss(i, j, 5.0, 1.5) / sum_gauss);
		printf("\n");
	}
}

double rgb_dis(rgb a, rgb b)
{
	int t1 = max(max(a.r, a.g), a.b);
	int t2 = max(max(b.r, b.g), b.b);
	return t1 - t2;
}

void gaussian_blur(GrayImage &dst, const GrayImage &src)
{
	int i, j, x, y, xx, yy, w, h, N;
	double sum;
	int m = gauss_n / 2;
	w = dst.w = src.w; h = dst.h = src.h; N = dst.N = src.N;
	double *data = new double[N]; dst.data = data;

	for (i = 0; i < w; i++)
		for (j = 0; j < h; j++)
		{
			sum = 0.0;
			for (x = -m; x <= m; x++)
				for (y = -m; y <= m; y++)
				{
					xx = i + x; yy = j + y;
					if (xx >= 0 && xx < w && yy >= 0 && yy < w)
						sum += gauss(x, y);
				}
			int t = i + w * j;
			data[t] = 0.0;
			for (x = -m; x <= m; x++)
				for (y = -m; y <= m; y++)
				{
					xx = i + x; yy = j + y;
					if (xx >= 0 && xx < w && yy >= 0 && yy < w)
						data[t] += src.data[xx + w * yy] * gauss(x, y);
				}
			data[t] /= sum;
		}
}

int main(int argc, char * argv[]) 
{
	gauss_init();

	const char * fInputName = argc >= 2 ? argv[1] : "input.ppm";
	const char * fTrimapName = argc >= 3 ? argv[2] : "trimap.ppm";
	const char * fOutName = argc >= 4 ? argv[3] : "output.ppm";
	const char * quick = argc >= 5 ? argv[4] : "quick";

	if (quick[0] == 's')	quick_mode = false;
	else	quick_mode = true;
	
	//load the image
	ColorImage srcInput;
	srcInput.load(fInputName);

	ColorImage srcTrimap;
	srcTrimap.load(fTrimapName);
	GrayImage dstTrimap(srcTrimap);
	printf("load over\n");

	if (!quick_mode)
	{
		double *d = srcTrimap.calc_d();
		dstTrimap.complete_solve(d);
		printf("complete solve over\n");
		dstTrimap.post_solve(srcTrimap);
		printf("post solve over\n");
		delete [] d;
	}

	GrayImage trimapBlur;
	gaussian_blur(trimapBlur, dstTrimap);

	freopen("log.out", "w", stdout);
	int i, j;
	for (i = 0; i < dstTrimap.w; i++)
	{
		for (j = 0; j < dstTrimap.h; j++)
			printf("%.2lf\t", srcTrimap.data[i + trimapBlur.w * j].l);
		printf("\n");
	}
	printf("\n");
	printf("\n");
	printf("\n");

	for (i = 0; i < dstTrimap.w; i++)
	{
		for (j = 0; j < dstTrimap.h; j++)
			printf("%.2lf\t", dstTrimap.data[i + trimapBlur.w * j]);
		printf("\n");
	}
	printf("\n");
	printf("\n");
	printf("\n");

	for (i = 0; i < trimapBlur.w; i++)
	{
		for (j = 0; j < trimapBlur.h; j++)
			printf("%.2lf\t", trimapBlur.data[i + trimapBlur.w * j]);
		printf("\n");
	}

	int iter, times = 5;
	for (iter = 0; iter < times; iter++)
	{

	}

		
	return 0;
}
