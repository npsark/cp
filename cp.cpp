#include <iostream>
#include <fstream>

#include <string>

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#include "Timer.h"

using namespace std;

void cpFStream(string in, string out){
	ifstream inFile;
	ofstream outFile;

	inFile.open(in.c_str());

	if(!(inFile.is_open())){
		cerr << "failed to open." << endl;
		exit(1);
	}

	outFile.open(out.c_str(), ofstream::trunc);

	if(outFile.fail()){
		cerr << "failed to open." << endl;
	}


	while(inFile.good()){
		char c = inFile.get();
		if(inFile.good()){
			outFile.put(c);
		}
	}

	inFile.close();
	outFile.close();
}


void cpRDWR(string in, string out, int buffSize){
	struct stat statBuff;
	statBuff.st_mode |= S_IRUSR;	


	int infd = open(in.c_str(), O_RDONLY);
	if(infd == -1){
		perror("open");
		exit(1);
	}

	int outfd = open(out.c_str(), O_RDWR|O_CREAT|O_TRUNC, statBuff.st_mode);
	if(outfd == -1){
		perror("open");
		exit(1);
	}	

	char *buff = new char[buffSize];
	int readVal = read(infd, buff, buffSize);
	while( 1 ){
		if(readVal == -1){
			perror("read");
			exit(1);
		}else if(readVal == 0){
			break;
		}
		int writeVal = write(outfd, buff, readVal);
		if(writeVal == -1){
			perror("write");
			exit(1);
		}
		readVal = read(infd, buff, buffSize);
	}

	if(close(infd) == -1){
		perror("close");
		exit(1);
	}
	if(close(outfd) == -1){
		perror("close");
		exit(1);
	}
}


bool fileExists(string file){
	ifstream f( file.c_str() );
	if(f.good()){
		f.close();
		return true;
	}else{
		f.close();
		return false;
	}
}



int main(int argc, char **argv){
	
	string in, out, report = "-0";

	int fileCount = 0;
	int i;
	for(i=1; argv[i]; i++){
		if(argv[i][0] == '-'){
			report = argv[i];
		}else{
			if(fileCount >= 2){
				cerr << "too many file paths." << endl;
				exit(1);
			}
	
			if(fileCount == 0){
				in = argv[i];
				fileCount++;
			}else{
				out = argv[i];
				fileCount++;
			}
		}
	}
	if(i < 3){
		cerr << "not enough arguments." << endl;
		exit(1);
	}

	struct stat statBuff;
	if(stat(in.c_str(), &statBuff) == -1){
		perror("stat");
		exit(1);
	}

	if( S_ISDIR( statBuff.st_mode) ){
		cerr << "input is a directory" << endl;
		return 1;
	}




	if(fileExists(out)){
		cerr << "output file already exists" << endl;
		return 1;
	}else{

		if(report[1] == '0'){
			cpRDWR(in, out, BUFSIZ);
		}else{
			Timer t;
			double eTime;
		
			if( t.start() == -1){
				cerr << "Timer::start()" << endl;
				return 1;
			}
			cpFStream(in, out);
			if( t.elapsedWallclockTime(eTime) == -1){
				cerr << "Timer::elapsedWallclockTime" << endl;
				return 1;
			}
			cout << "Method 1:\tWall Clock:\t" << eTime << endl;
			
			if( t.elapsedUserTime(eTime) == -1){
				cerr << "Timer::elapsedUserTime" << endl;
				return 1;
			}
			cout << "\t\tUser Time:\t" << eTime << endl;
			
			if( t.elapsedSystemTime(eTime) == -1){
				cerr << "Timer::elapsedSystemTime" << endl;
				return 1;
			}
			cout << "\t\tSystem Time:\t" << eTime << endl << endl;
			
			//=======================================================
			//=======================================================

			if( t.start() == -1){
				cerr << "Timer::start()" << endl;
				return 1;
			}
			cpRDWR(in, out, 1);
			if( t.elapsedWallclockTime(eTime) == -1){
				cerr << "Timer::elapsedWallclockTime" << endl;
				return 1;
			}
			cout << "Method 2:\tWall Clock:\t" << eTime << endl;
			
			if( t.elapsedUserTime(eTime) == -1){
				cerr << "Timer::elapsedUserTime" << endl;
				return 1;
			}
			cout << "\t\tUser Time:\t" << eTime << endl;
			
			if( t.elapsedSystemTime(eTime) == -1){
				cerr << "Timer::elapsedSystemTime" << endl;
				return 1;
			}
			cout << "\t\tSystem Time:\t" << eTime << endl << endl;
			
			//======================================================
			//======================================================

			if( t.start() == -1){
				cerr << "Timer::start()" << endl;
				return 1;
			}
			cpRDWR(in, out, BUFSIZ);	
			if( t.elapsedWallclockTime(eTime) == -1){
				cerr << "Timer::elapsedWallclockTime" << endl;
				return 1;
			}
			cout << "Method 3:\tWall Clock:\t" << eTime << endl;
			
			if( t.elapsedUserTime(eTime) == -1){
				cerr << "Timer::elapsedUserTime" << endl;
				return 1;
			}
			cout << "\t\tUser Time:\t" << eTime << endl;
			
			if( t.elapsedSystemTime(eTime) == -1){
				cerr << "Timer::elapsedSystemTime" << endl;
				return 1;
			}
			cout << "\t\tSystem Time:\t" << eTime << endl;
			
		}	

	}






	return 0;
}
