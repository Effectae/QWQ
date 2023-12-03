/*
com.dvloper.grannychaptertwo

矩阵 
libunity.so:bss[1] + 0x2F6C4 -> 0xF0 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x2FF14 -> 0xD0 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x3A240 -> 0x1E0 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x3CA48 -> 0x290 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x3CA4C -> 0x2B0 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x4ECC0 -> 0x10 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x4ED80 -> 0x20 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x4FB00 -> 0x30 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x4FED0 -> 0x40 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x54DC0 -> 0x50 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x551C0 -> 0x70 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x55480 -> 0x80 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x55540 -> 0x90 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x59500 -> 0xB0 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x5B000 -> 0xC0 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x5B100 -> 0xD0 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x5B604 -> 0x1A0 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x5B624 -> 0x240 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x5B644 -> 0x220 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x5BC08 -> 0x190 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x5BC14 -> 0x1B0 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x5C2B0 -> 0x2A0 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x5C310 -> 0x280 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x5C3F8 -> 0x1D0 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x5C4E8 -> 0x180 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x5C508 -> 0x1C0 -> 0x54 -> 0xC4
libunity.so:bss[1] + 0x5C6C8 -> 0x230 -> 0x54 -> 0xC4


258D

结构体 libunity.so:bss[1] + 0x3488C -> 0x8 -> 0xFC -> 0x4

34个结构体

结构体 数据偏移
0x60 X坐标
0x74 方向


libunity.so:bss[1] - libunity.so = c51000
*/

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#define 帧数 100
using namespace std;

#include "read_driver.hpp"
float getF(long address,ReadDriver * a ){float result = a->read<float>(address);return result;}
int main()
{
float matrix[20], px, py;
ReadDriver * a = new ReadDriver();
	char module_name[0x100] = "libunity.so"; 
	a->getProcessPid((char*)"com.dvloper.grannychaptertwo");
	long libunity = a->getModuleBase(module_name);
	long libunitybss = libunity + 0xc51000;
	printf("ipid = %d\n", a->getPid<int>());
		printf("libunity.so = %lx\n", libunity);
		printf("libunity.so:bss = %lx\n", libunitybss);
		
	long int Matrix = a->lsp32(a->lsp32(a->lsp32(libunitybss + 0x4ED80) + 0x20) + 0x54) + 0xC4;
    printf("Matrix=%lx\n", Matrix);
    long int CXX = a->lsp32(a->lsp32(a->lsp32(libunitybss + 0x3488C) + 0x8) + 0xFC) + 0x4;
    printf("CXX=%lx\n",CXX);
    FILE *fp = fopen("/sdcard/x", "r");//*fp意思是指针的意思，指向那个文件并且读取fopen为函数
	FILE *fp1 = fopen("/sdcard/y", "r");
	if (fp == NULL || fp1 == NULL)	// 如果没有读取到分辨率文件,则设置以下分辨
	{
		px = 1080.0;
		py = 2400.0;
	}
	else
	{
		fscanf(fp, "%f", &px);
		fscanf(fp1, "%f", &py);
		if (py > px)
		{
			float t = px;
			px = py;
			py = t;
		}
		py = py / 2.0;
		px = px / 2.0;
		fclose(fp);
		fclose(fp1);
	}
	long int XX = 0;
	FILE *F;
	
	
	
	
	while ((F = fopen("/sdcard/stop", "r")) == NULL)	
	{
		char aaa[4096] = "";	// 为变量申请内存
		char b[4096];
		// 获取矩阵
		for (int i = 0; i < 16; i++)
		{
			float matrixaddr = getF(Matrix + i * 4,a);
			matrix[i] = matrixaddr;
		}

		// 获取坐标
			for (int i = 1;i<=35; i++)	// 循环遍历
		{
		//char Name[32] = {'0'};
		//itoa(i,Name,10);
		    XX = a->lsp32(CXX + 0x4*i);
		    int hp = 520;	   		 
			// 敌人坐标
			float d_x = getF(XX + 0x60,a);
			float d_z = getF(XX + 0x4 + 0x60,a);//
			float d_y = getF(XX + 0x8 + 0x60,a);
		    
			// 距离算法
			float camear_z = matrix[3] * d_x + matrix[7] * d_z + matrix[11] * d_y + matrix[15];//相机Z
                                               int jl = camear_z/2;
		// 矩阵
float camear_r = matrix[3] * d_x + matrix[7] * d_z + matrix[11] * d_y + matrix[15];

float r_x = px + (matrix[0] * d_x + matrix[4] *d_z+ matrix[8] * d_y + matrix[12]) / camear_z * px;//视角高

float r_y = py -  (matrix[1] * d_x + matrix[5] * (d_z-0.3) + matrix[9] * d_y + matrix[13]) / camear_z * py;//;视角宽

float r_w = py - (matrix[1] * d_x + matrix[5] * (d_z +1.8) + matrix[9] * d_y + matrix[13]) / camear_z * py;
			
float X=r_x - (r_y - r_w) / 4;
float Y=(r_y) ;
float W=(r_y- r_w) / 2 ;
float H=(r_y - r_w) ;		  

int dt = 0;
int dw =i;//队伍ID
int bot = 1;//人机识别0为人机,则不绘制名字 ,1则绘制名字	
			sprintf(b, "%f,%f,%f,%f,%d,%d,%d,%d,%d,ID=%d\n",
			        X,	// 1.x
					Y,
					W,
					H,
				  jl, //距离               
					hp,// 6.血量
					bot,
					dw,
					dt,
				    dw
				    //CON + 0x4*i
				);
			strcat(aaa, b);
			}
		int fd = open("/sdcard/b.log", O_WRONLY | O_CREAT);
		write(fd, aaa, sizeof(aaa));	// 写入文本
		close(fd);
		usleep(1000000/帧数);
	}
    return 0;
}