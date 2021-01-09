#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
// #include "DISK_driver.h"

struct PARTITION
{
	int total_blocks;
  int block_size;
};

struct FAT
{
  char *filename;
  int file_length;
  int blockPtrs[10];
  int current_location;
};

//each array corresponds to a FAT and the int stored refers
//to the index of the fp
int helperTable[20];
char *block_buffer;
FILE *fp[5];

struct PARTITION *partition;
struct FAT *fat[20];

void initIO()
{
	partition = (struct PARTITION*)malloc(sizeof(struct PARTITION));
	partition->total_blocks = 0;
	partition->block_size = 0;

	for(int i = 0; i < 20; i++)
	{
		fat[i] = (struct FAT*)malloc(sizeof(struct FAT));
		fat[i]->filename = (char *)malloc(sizeof(char)*50);
		fat[i]->filename = NULL;
		fat[i]->file_length = 0;
		for(int j = 0; j < 10; j++)
		{
			fat[i]->blockPtrs[j] = 0;
		}
		fat[i]->current_location = -1;
	}

	block_buffer = NULL;
	for(int i = 0; i < 5; i++)
		fp[i] = NULL;
}

int partitionMethod(char *name, int blocksize, int totalblocks)
{
	FILE *file;
	char *filename[100];
	partition->block_size = blocksize;
	partition->total_blocks = totalblocks;

	mkdir("PARTITION", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	sprintf(filename, "%s%s", "PARTITION/", name);
	file = fopen(filename, "wb");
	if(file == NULL)
	{
		printf("Could not create partition file.\n");
		return 0; //failed
	}
	else
	{
		//writing in information following PARTITION:FAT:BLOCKS
		//PARTITION PART
		fwrite(partition, sizeof(struct PARTITION), 1, file);
		//FAT PART
		for(int i = 0; i < 20; i++)
		fwrite(fat[i], sizeof(struct FAT), 1, file);
		//BLOCK PART
		for(int i = 0; i < (blocksize*totalblocks); i++)
		fputc("0", file);

		fclose(file);
		return 1;
	}
}

int mount(char *name)
{
	FILE *file;
	char *filename[100];
	sprintf(filename, "%s%s", "PARTITION/", name);

	file = fopen(filename, "rb");
	if(file == NULL)
	{
		printf("Could not mount file.\n");
		return 0;			//failed
	}
	else
	{
		//PARTITION PART
		fread(partition, sizeof(*partition), 1, file);

		//FAT PART
		for(int i = 0; i < 20; i++)
		fread(fat[i], sizeof(*fat[i]), 1, file);

		//BLOCK malloc
		block_buffer = (char*)malloc(sizeof(char)*partition->block_size);

		fclose (file);
		return 1;
	}
}

int searchFP()
{
	for(int i = 0; i < 5; i++)
	{
		if(fp[i] == NULL) return i;
	}
	return -1;
}

int searchFAT()
{
    for(int i = 0; i < 20; i++)
    {
        if(fat[i]->filename == NULL) return i;
    }
    return -1;
}

int assignBlock(FILE *file, int index)
{
	char c;

	//return to beginning of file
	rewind(file);
  while (!isalpha(c = fgetc(file)) && !isdigit(c))

	//once we find the blockPtrs go back 1 char to reach pointer
	//start point

  fseek(file, -1, SEEK_CUR);

  for (int i = 0; i < index; i++)
	{
  	int errCode = fseek(file, partition->block_size, SEEK_CUR);
    	if (errCode == -1) return -1;
  }

    return 1;


}
int openfile(char *name)
{
	FILE *file;
    int fpIndex, fatIndex;
	char *filename[100];

	int size = 0;

	// Look for through FAT filenames
	for(int i = 0; i < 20; i++)
	{
		if(strcmp(fat[i]->filename, name) == 0)
		{
			fpIndex = searchFP();

			if(fpIndex == -1)
			{
				return -1;
			}
			else
			{
				file = fopen(filename, "r");
				if(file == NULL) return -1;
			}
            
            assignBlock(file, 0);
            fp[fpIndex] = file;
            helperTable[i] = fpIndex;
			return i;
		}
	}
	//Exact filename was not found
    file = fopen(name, "r+b");
    if(file == NULL) return -1;
    
    fatIndex = searchFAT;
    assignBlock(file, 0);
    fp[fpIndex] = file;
    helperTable[fatIndex] = fpIndex;
    
    fat[fatIndex]->filename = name;
    fat[fatIndex]->current_location = 0;
    fat[fatIndex]->file_length = (partition->total_blocks * partition->block_size);
    fat[fatIndex]->blockPtrs[0] = 0;
    
    return fatIndex;
}

int readBlock(int file)
{
	if(file < 0 || file > 19) return -1;

	int i;
	char string[partition->block_size+1];
	char c;
	int currentBlock = fat[file]->current_location;
	int currentByte = fat[file]->blockPtrs[currentBlock];

	FILE *f = fp[helperTable[file]];

	int errCode = assignBlock(f, currentBlock);

	if(errCode == - 1) return -1;

	for(i = 0; i < partition->block_size; i++)
	{
		c = fgetc(f);
		string[i] = c;
	}
	string[i+1] = '\0';

	block_buffer = strdup(string);

	fat[file]->current_location++;

	return 1;
}

int writeBlock(int file, char *data)
{
	if(file < 0 || file > 19) return -1;

	FILE *f;
	int currentBlock = fat[file]->current_location;
	int currentByte = fat[file]->blockPtrs[currentBlock];

	f = fp[helperTable[file]];

	int errCode = assignBlock(f, currentBlock);

	if(errCode == -1) return -1;

	//writing in new characters
	for(int i = 0; data[i] != '\0'; i++)
	{
		fprintf(f, "%c", data[i]);
	}

	fat[file]->current_location++;

	return 1;
}
