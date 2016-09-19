#include  <stdio.h>
#include  <stdlib.h>
#include  <io.h>
#include  <assert.h>
#include  <string.h>
#define  RET_SUCCESS   0
#define  RET_FAIL     -1
#define  WIN_FILE_PATH_MAX_LEN   255

int TraversalAndMoveFile(char *szSouPath, char *szDesPath, char *szFileKey, int *pFileNum)
{
	struct _finddata_t  stFileInfo;
	char acSearchFolder[WIN_FILE_PATH_MAX_LEN] = {0};
	char acSearchSonFolder[WIN_FILE_PATH_MAX_LEN] = {0};
	char acMoveCmd[WIN_FILE_PATH_MAX_LEN] = {0};
	long lHandle = 0;

	assert(szSouPath);
	assert(szDesPath);
	assert(szFileKey);
	assert(pFileNum);

	strcpy((char*)acSearchFolder, szSouPath);
	strcat((char*)acSearchFolder, "\\*");

	if (strlen(acSearchFolder) > WIN_FILE_PATH_MAX_LEN)
	{
		printf("the file folder is error!!!\n");
		return RET_FAIL;
	}

	lHandle = _findfirst((char*)acSearchFolder, &stFileInfo);
	if (-1L == lHandle)
	{
		printf("find the folder %s failed!!!\n", szSouPath);
		return RET_FAIL;
	}

	do
	{
		if ((stFileInfo.attrib & _A_SUBDIR) && (strcmp(stFileInfo.name, ".")) 
			&& (strcmp(stFileInfo.name, "..")))
		{
			strcpy((char*)acSearchSonFolder, szSouPath);
			strcat((char*)acSearchSonFolder, "\\");
			strcat((char*)acSearchSonFolder, stFileInfo.name);

			printf("acSearchSonFolder=%s\n", (char*)acSearchSonFolder);

			TraversalAndMoveFile((char*)acSearchSonFolder, szDesPath, szFileKey, pFileNum);
		}
		else if (0 == (stFileInfo.attrib & _A_SUBDIR))
		{
			if (strstr(stFileInfo.name, szFileKey))
			{
				(*pFileNum)++;

				strcpy(acMoveCmd, "move ");
				strcat(acMoveCmd, szSouPath);
				strcat(acMoveCmd, "\\");
				strcat(acMoveCmd, stFileInfo.name);
				strcat(acMoveCmd, "  ");
				strcat(acMoveCmd, szDesPath);
				printf("acMoveCmd=%s\n", acMoveCmd);
				system(acMoveCmd);
			};
		}
	}while (0 == _findnext(lHandle, &stFileInfo));

	_findclose(lHandle);

	return RET_SUCCESS;
}
#if 0
int main()
{
	int nFileNum = 0;
	char a[] = "f:\\code\\TraversalFile";
	char b[] = "vc60";
	char c[] = "f:\\code";

	TraversalAndMoveFile((char*)a, (char*)c, (char*)b, &nFileNum);
	printf("the file num is %d\n", nFileNum);
	return 0;
}
#endif

#if 1
int main(int argc, char** argv)
{
	int nFileNum = 0;

	if (argc < 4)
	{
		printf("please input soupath/despath/filekey!!!\n");
		/*   
		**F:\code\TraversalFile\backupfile\Debug>backupfile.exe F:\code\testa F:\code\aa  a.c
		*/
		return -1;
	}

	if (RET_SUCCESS == TraversalAndMoveFile(argv[1], argv[2], argv[3], &nFileNum))
	{
		printf("the file num is %d\n", nFileNum);
	}

	return 0;
}
#endif 