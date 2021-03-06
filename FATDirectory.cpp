﻿// FATDirectory.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *fp;
	int i;
	unsigned char buf[4048] = { 0 };
	char entries[10][32];
	int datasection;
	unsigned char temp[30];
	

	fopen_s(&fp,"fat32.dd","rb");
	
	for ( i = 0;i < 512;i++)
		buf[i] = fgetc(fp);

	temp[0] = buf[14];
	temp[1] = buf[15];
	temp[2] = 0;
	temp[3] = 0;
	datasection = *((int *)temp)*512;
	
	memcpy(temp , buf+36 , 4);
	datasection += *((int *)temp) * 2 * 512;

	fseek(fp,datasection,SEEK_SET );
	printf("\n========================================================\n");

	while (1)
	{
		for (i = 0;i < 32;i++)
			buf[i] = fgetc(fp);

		if (buf[11] == 0x00)
		{
			return 0;
		}
		else if (buf[11] == 0x0f)
		{
			fseek(fp, -32, SEEK_CUR);
			int j;
			for ( j = 0;;j++)
			{
				for (i = 0;i < 32;i++)
					buf[i] = entries[j][i] = fgetc(fp);
				if (entries[j][11] != 0x0f)
					break;
			}
			printf("FileName : ");
			for (j--; j != -1;j--)
			{
				fwrite( entries[j]+1,5,2,stdout);
				fwrite(entries[j] + 14, 6, 2, stdout);
				fwrite(entries[j] + 28, 2, 2, stdout);
				
			}
			printf("\n");
			printf("FileType : %s\n", (buf[11] >= 0x20) ? "file" : "directory");
			memcpy(temp, buf + 26, 4);
			printf("ClusterOffset : %x\n", *((int *)temp));
			printf("State : %s\n", (buf[0] == 0xE5) ? "erased" : "normal");
			printf("Filsize : %x\n", *((int*)(buf + 28)));
		}
		else if (buf[11] == 0x10 || buf[11] == 0x20)
		{
			printf("FileName : ");
			fwrite(buf, 8, 1, stdout);
			putchar('.');
			fwrite(buf, 3, 1, stdout);
			putchar('\n');
			printf("FileType : %s\n", (buf[11] >= 0x20) ? "file" : "directory");
			memcpy(temp, buf + 26, 4);
			printf("ClusterOffset : %x\n", *((int *)temp));
			printf("State : %s\n",(buf[0]==0xE5)?"erased":"normal");
			printf("Filesize : %x\n", *((int*)(buf + 28)));
		}
		else
			continue;
		printf("\n========================================================\n");
	}
	fclose(fp);
    return 0;

}

