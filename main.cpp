#include <iostream>
#include <w32api/libloaderapi.h>
#include <w32api/fileapi.h>
#include <w32api/handleapi.h>
#include <windows.h>
#include <fstream>

#define BUFFER_SIZE 150

void process(char *filename);
void aggregate (char *);
using namespace std;

char ** extensions;
int nExtensions;
char * localPath;
ofstream outputFile;

int main(int argc, char ** argv) {
    if(argc < 2){
        cerr<<"Need arguments"<<endl;
        cout<<"fileAggregator cpp .c h .java "<<endl;
        cout<<"Program output will be named AggregatorOutFCI.txt"<<endl;
        return 1;
    }
    extensions = new char *[argc - 1];
    nExtensions = argc - 1;
    for(int i = 1; i < argc; i++){
        if(argv[i][0] != '.') {
            extensions[i - 1] = argv[i];
        } else {
            extensions[i - 1] = &(argv[i][1]);
        }
    }


    char buffer [BUFFER_SIZE];
    localPath = new char[260];
    GetCurrentDirectoryA(BUFFER_SIZE,buffer);

    strcpy(localPath,(const char *)buffer);

    strcat(buffer, "\\*");
    //extract extensions from argument
    //read through folder files
    //when we find a file with matching extension pass to function that reaps contents
    //Looping through each file
    WIN32_FIND_DATA data;
    HANDLE hFind = FindFirstFile(buffer, &data);
    if ( hFind != INVALID_HANDLE_VALUE ) {
        do { //data loops through each file/folder in local directory
            process(data.cFileName);
        } while (FindNextFile(hFind, &data));
        FindClose(hFind);
    }

    delete [] extensions;
    delete [] localPath;
    return 0;
}

void process(char *filename) {
    //extract file extension
    //if file name matches
    //    copy contents to created file;

    if (filename[0] == '.') return;
    int n = 1;
    while ((n < 260) && (filename[n] != '.')) {
        if(n >= 258){cout<<"error at process first loop"<<endl;}
        n++;
    }
    //n points to '.' +1

    n++;
    for(int i = 0; i < nExtensions; i++){
        bool matches = true;
        for(int j = 0; ;j++){
            if(extensions[i][j] != filename[n + j]){
                matches = false;
                break;
            }
            if((extensions[i][j] == 0) || (filename[n + j] == 0)){
                break;
            }
        }
        if(matches){
            aggregate(filename);
        }
    }
}

void aggregate (char * filename) {
    char filepath[260];
    strcpy(filepath, localPath);
    strcat(filepath, filename);
    ifstream reader;
    reader.open(filename);

    string line;
    if (reader.is_open()) {
        if(!outputFile.is_open()){
            outputFile.open("AggregatorOutFCI.txt", ios_base::out);
        }

        outputFile<<endl<<endl<<"**************************************************"<<endl;
        outputFile<<filename<<endl;
        outputFile<<"**************************************************"<<endl<<endl;

        while (getline(reader, line)) {
            outputFile << line << endl;
        }

        reader.close();
    } else {
        cout<<"Could not open: "<< filename<<endl;
    }
}

