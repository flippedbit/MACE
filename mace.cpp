#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

/* function written by bluechill */
std::vector<WORD> convertStringToWord(std::string stringToConvert)
{	
	//cout << "Sizeof String:" << stringToConvert.size() << endl;

	//<0>/<1>/<2> <3>:<4> <-- format of the Vector in terms of the date given
	vector<WORD> toReturn;

	for (int i = 0;i < stringToConvert.size();i++)
	{
		int originalI = i;

		while (1)
		{
			i++;

			if (stringToConvert[i] == '/' || stringToConvert[i] == ' ' || stringToConvert[i] == ':')
				break;
		}

		string section = "";

		for (;originalI < i;originalI++)
			section += stringToConvert[originalI];

		toReturn.push_back(atoi(section.c_str()));
	}
	return toReturn;
}
/* end bluechill function */

void usage(char* title)
{
     cout << "Usage: " << title << "  [filename] [options] [time]" << endl;
     cout << "Options:" << endl;
     cout << "-c\tChange file create time" << endl;
     cout << "-w\tChange file last write time" << endl;
     cout << "-a\tChange file last access time" << endl;
     cout << "-h\tDisplay usage" << endl << endl;
     cout << "Example: " << title << " C:\test.txt -c \"21/12/2012 14:11\"" << endl;
}

int main(int argc, char* argv[])
{
    if(argc != 4)
    {
            usage(argv[0]);
    } else {
           string filename = argv[1];
           string option = argv[2];
           string toConvert = argv[3];
           
           
           if(strstr(option.c_str(), "-") != NULL)
           {
                         HANDLE hFile = CreateFile(filename.c_str(), GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                         SYSTEMTIME sTime;
                         FILETIME fTime;
                         
                         if(hFile != INVALID_HANDLE_VALUE)
                         {
                                  GetSystemTime(&sTime);
                                  std::vector<WORD> timestamp = convertStringToWord(toConvert);
                                  sTime.wDay = timestamp[0];
                                  sTime.wMonth = timestamp[1];
                                  sTime.wYear = timestamp[2];
                                  sTime.wHour = timestamp[3];
                                  sTime.wMinute = timestamp[4];
                                  if(sTime.wDay > 0 && sTime.wDay < 31 || sTime.wMonth > 0 && sTime.wMonth < 13 || sTime.wHour < 24 || sTime.wMinute < 60 || sTime.wSecond < 60){
			                                    if(SystemTimeToFileTime(&sTime, &fTime) == 0){
				                                      printf("Unable to convert the system time\n");
				                                      return 0;
		                                        }
                       		      }else{
			                            printf("Input not accepted!\n");
			                            return 0;
	                              }
                             if(strcmp(option.c_str(), "-h") == 0)
                             {
                                  usage(argv[0]);
                             }
                             else if(strcmp(option.c_str(), "-c") == 0)
                             {
                                  if(SetFileTime(hFile, &fTime, &fTime, &fTime) != 0)
                                  {
                                         cout << "Changed creation date for file " << filename << endl;
                                  } else {
                                         cout << "Failed setting creation date." << endl;
                                  }
                             }
                             else if(strcmp(option.c_str(), "-w") == 0)
                             {
                                  if(SetFileTime(hFile, &fTime, &fTime, &fTime) != 0)
                                  {
                                         cout << "Changed last written date for file " << filename << endl;
                                  } else {
                                         cout << "Failed setting last written date." << endl;
                                  }
                             }
                             else if(strcmp(option.c_str(), "-a") == 0)
                             {
                                  if(SetFileTime(hFile, &fTime, &fTime, &fTime) != 0)
                                  {
                                         cout << "Changed last accessed date for file " << filename << endl;
                                  } else {
                                         cout << "Failed last accessed creation date." << endl;
                                  }
                             }
                             else
                             {
                                     cout << option << " is not a valid option." << endl;
                                     usage(argv[0]);
                             }
                         } else { cout << "Could not edit file." << endl; }
                                
           } else { usage(argv[0]); }
    }
    return 0;   
}
