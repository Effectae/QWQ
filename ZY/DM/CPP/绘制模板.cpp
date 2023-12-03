#include <stdio.h>
#include <conio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/socket.h>
#include <malloc.h>
#include <math.h>
#include <thread>
#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <locale>
#include <string>
#include <codecvt>
// 文字转码
#ifndef __UTF_H__
#define __UTF_H__
#define FALSE 0
#define TRUE 1
#define halfShift 10
#define UNI_SUR_HIGH_START (UTF32)0xD800
#define UNI_SUR_HIGH_END (UTF32)0xDBFF
#define UNI_SUR_LOW_START (UTF32)0xDC00
#define UNI_SUR_LOW_END (UTF32)0xDFFF
// Some fundamental constants
#define UNI_REPLACEMENT_CHAR (UTF32)0x0000FFFD
#define UNI_MAX_BMP (UTF32)0x0000FFFF
#define UNI_MAX_UTF16 (UTF32)0x0010FFFF
#define UNI_MAX_UTF32 (UTF32)0x7FFFFFFF
#define UNI_MAX_LEGAL_UTF32 (UTF32)0x0010FFFF
typedef unsigned char boolean;
typedef unsigned int CharType;
typedef char UTF8;
typedef unsigned short UTF16;
typedef unsigned int UTF32;

static const UTF32 halfMask = 0x3FFUL;
static const UTF32 halfBase = 0x0010000UL;
static const UTF8 firstByteMark[7] = {0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC};
static const UTF32 offsetsFromUTF8[6] =
	{0x00000000UL, 0x00003080UL, 0x000E2080UL, 0x03C82080UL, 0xFA082080UL, 0x82082080UL};
static const char trailingBytesForUTF8[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5};

typedef enum
{
	strictConversion = 0,
	lenientConversion
} ConversionFlags;
typedef enum
{
	conversionOK,	 // conversion successful
	sourceExhausted, // partial character in source,but hit end
	targetExhausted, // insuff. room in target for conversion
	sourceIllegal,	 // source sequence is illegal/malformed
	conversionFailed
} ConversionResult;
#endif

int Utf16_To_Utf8(const UTF16 *sourceStart, UTF8 *targetStart, size_t outLen,
				  ConversionFlags flags)
{
	int result = 0;
	const UTF16 *source = sourceStart;
	UTF8 *target = targetStart;
	UTF8 *targetEnd = targetStart + outLen;
	if ((NULL == source) || (NULL == targetStart))
	{
		// printf("ERR,Utf16_To_Utf8:source=%p,targetStart=%p\n",source,targetStart);
		return conversionFailed;
	}

	while (*source)
	{
		UTF32 ch;
		unsigned short bytesToWrite = 0;
		const UTF32 byteMask = 0xBF;
		const UTF32 byteMark = 0x80;
		const UTF16 *oldSource = source; // In case we have to back up
		// because of target overflow.
		ch = *source++;
		// If we have a surrogate pair,convert to UTF32 first.
		if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END)
		{
			// If the 16 bits following the high surrogate are in the source
			// buffer...
			if (*source)
			{
				UTF32 ch2 = *source;
				// If it's a low surrogate,convert to UTF32.
				if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END)
				{
					ch = ((ch - UNI_SUR_HIGH_START) << halfShift) + (ch2 - UNI_SUR_LOW_START) +
						 halfBase;
					++source;
				}
				else if (flags == strictConversion)
				{			  // it's an unpaired high surrogate
					--source; // return to the illegal value itself
					result = sourceIllegal;
					break;
				}
			}
			else
			{ // We don't have the 16 bits following the
				// high surrogate.
				--source; // return to the high surrogate
				result = sourceExhausted;
				break;
			}
		}
		else if (flags == strictConversion)
		{
			// UTF-16 surrogate values are illegal in UTF-32
			if (ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END)
			{
				--source; // return to the illegal value itself
				result = sourceIllegal;
				break;
			}
		}
		// Figure out how many bytes the result will require
		if (ch < (UTF32)0x80)
		{
			bytesToWrite = 1;
		}
		else if (ch < (UTF32)0x800)
		{
			bytesToWrite = 2;
		}
		else if (ch < (UTF32)0x10000)
		{
			bytesToWrite = 3;
		}
		else if (ch < (UTF32)0x110000)
		{
			bytesToWrite = 4;
		}
		else
		{
			bytesToWrite = 3;
			ch = UNI_REPLACEMENT_CHAR;
		}
		target += bytesToWrite;
		if (target > targetEnd)
		{
			source = oldSource; // Back up source pointer!
			target -= bytesToWrite;
			result = targetExhausted;
			break;
		}
		switch (bytesToWrite)
		{ // note: everything falls through.
		case 4:
			*--target = (UTF8)((ch | byteMark) & byteMask);
			ch >>= 6;
		case 3:
			*--target = (UTF8)((ch | byteMark) & byteMask);
			ch >>= 6;
		case 2:
			*--target = (UTF8)((ch | byteMark) & byteMask);
			ch >>= 6;
		case 1:
			*--target = (UTF8)(ch | firstByteMark[bytesToWrite]);
		}
		target += bytesToWrite;
	}
	return result;
}

typedef char PACKAGENAME;
int getPID(const char *packageName)
{
	int id = -1;
	DIR *dir;
	FILE *fp;
	char filename[64];
	char cmdline[64];
	struct dirent *entry;
	dir = opendir("/proc");
	while ((entry = readdir(dir)) != NULL)
	{
		id = atoi(entry->d_name);
		if (id != 0)
		{
			sprintf(filename, "/proc/%d/cmdline", id);
			fp = fopen(filename, "r");
			if (fp)
			{
				fgets(cmdline, sizeof(cmdline), fp);
				fclose(fp);
				if (strcmp(packageName, cmdline) == 0)
				{
					return id;
				}
			}
		}
	}
	closedir(dir);
	return -1;
}

long int get_module_base(int pid, const char *module_name)
{
	FILE *fp;
	long addr = 0;
	char *pch;
	char filename[64];
	char line[1024];
	snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
	fp = fopen(filename, "r");
	if (fp != NULL)
	{
		while (fgets(line, sizeof(line), fp))
		{
			if (strstr(line, module_name))
			{
				pch = strtok(line, "-");
				addr = strtoul(pch, NULL, 16);
				if (addr == 0x8000)
					addr = 0;
				break;
			}
		}
		fclose(fp);
	}
	return addr;
}

long int handle;
float getFloat(long int addr)
{
	float var = 0;
	pread64(handle, &var, 4, addr);
	return var;
}

int getDword(long int addr)
{
	int var = 0;
	pread64(handle, &var, 4, addr);
	return var;
}

long int getZZ(long int addr)
{
	long int var = 0;
	pread64(handle, &var, 4, addr);
	return var;
}

// 获取名字
void getCharacterName(UTF8 *transcoding, long int addr) // 传入指针
{
	int classname;
	int m = 0;
	UTF8 buf88[256] = "";
	long int namepy = getZZ(addr + 0x40);
	UTF16 buf16[34] = {
		0};
	int hex[2] = {
		0};
	for (int i = 0; i < 4; i++)
	{
		classname = getZZ(namepy + 0xC + i * 4);
		hex[0] = (classname & 0xfffff000) >> 16;
		hex[1] = classname & 0xffff;
		buf16[m] = hex[1];
		buf16[m + 1] = hex[0];
		m += 2;
	}
	Utf16_To_Utf8(buf16, buf88, sizeof(buf88), strictConversion);
	sprintf(transcoding, "%s", buf88);
}

int main(int argc, char **argv)
{
	// 判断是否root用，如果开启后面记得加反括号

	// 读取进程
	int ipid = getPID("包名"); // 填写包名

	printf("要调试的进程PID是16进制=%d\n", ipid); // 值是16进制

	// 读取mem文件
	char lj[64];
	sprintf(lj, "/proc/%d/mem", ipid);
	handle = open(lj, O_RDWR);
	if (handle == 0)
	{
		puts("获取mem失败!");
		exit(1);
	}
	float matrix[50], px, py, angle;
	int scwq;

	// 分辨率获取
	FILE *fp = fopen("/sdcard/分辨率文件一", "r"); //*fp意思是指针的意思，指向那个文件并且读取fopen为函数
	FILE *fp1 = fopen("/sdcard/分辨率文件二", "r");
	if (fp == NULL || fp1 == NULL) // 如果没有读取到分辨率文件,则设置以下分辨
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
	// 读取基址
	puts("\n基址遍历\n");

	char mname[] = "入口模块一";
	long int libbase = get_module_base(ipid, mname);
	char mname2[] = "入口模块二";
	long int libbase2 = get_module_base(ipid, mname2);

	long int Matrix = getZZ(getZZ(getZZ(libbase2 + 矩阵主地址) + 层一) + 层二) + 层三; // 矩阵地址
	printf("矩阵地址=%lx\n", Matrix);

	FILE *F;
	while ((F = fopen("/sdcard/停止", "r")) == NULL)
	{
		char aaa[30720] = ""; // 为变量申请内存
		char b[256];

		long int Uworld = (libbase + 数量主地址);
		long int Uleve = getZZ(Uworld + 数量层一);
		long int Uleve2 = getZZ(Uleve + 数量层二);
		long int arrayaddr = getZZ(Uleve2 + 数量层三);
		long int count = getDword(arrayaddr + 数量层四); // 数量

		printf("数量=%lx\n", count);

		// 获取矩阵
		for (int i = 0; i < 16; i++)
		{
			float matrixaddr = getFloat(Matrix + i * 4);
			matrix[i] = matrixaddr;
		}

		// 获取坐标
		for (int i = 0; i < count; i++) // 想要不绘制自己,count-1即可
		{
			long int objaddrzz = getZZ(arrayaddr + 0x10 + i * 4); // 数组
			long int zsjgt = getZZ(getZZ(arrayaddr + 0x10 + 4 * (count - 1)) + 0x40);

			// 敌人坐标
			float d_x = getFloat(objaddrzz + 坐标一);
			float d_z = getFloat(objaddrzz + 坐标二);
			float d_y = getFloat(objaddrzz + 坐标三);
			分辨率文件。

			if (team == team2)
			{
				continue;
			}

			// 距离算法
			float camear_z = matrix[3] * d_x + matrix[7] * d_z + matrix[11] * d_y + matrix[15]; // 相机Z
			int jl = camear_z / 1;
			// 矩阵
			float camear_r = matrix[3] * d_x + matrix[7] * d_z + matrix[11] * d_y + matrix[15];

			float r_x = px + (matrix[0] * d_x + matrix[4] * d_z + matrix[8] * d_y + matrix[12]) / camear_z * px; // 视角高

			float r_y = py - (matrix[1] * d_x + matrix[5] * (d_z - 0.3) + matrix[9] * d_y + matrix[13]) / camear_z * py; //;视角宽

			float r_w = py - (matrix[1] * d_x + matrix[5] * (d_z + 1.8) + matrix[9] * d_y + matrix[13]) / camear_z * py;

			float X = r_x - (r_y - r_w) / 4;
			float Y = (r_y);
			float W = (r_y - r_w) / 2;
			float H = (r_y - r_w);

			int dt = 0;
			int dw = 1;	 // 队伍ID
			int bot = 1; // 人机识别0为人机,则不绘制名字 ,1则绘制名字

			UTF8 Name[32] = "";

			getCharacterName(Name, objaddrzz);

			sprintf(b, "%f,%f,%f,%f,%d,%d,%d,%d,%d,%s,\n",
					X + 42, // 1.x
					Y,
					W,
					H,
					jl, // 距离
					hp, // 6.血量
					bot,
					dw,
					dt,
					Name,
					objaddrzz);
			strcat(aaa, b);
		}

		int fd = open("/sdcard/b.log", O_WRONLY | O_CREAT);
		write(fd, aaa, sizeof(aaa)); // 写入文本
		close(fd);
		usleep(100);
	}
}
