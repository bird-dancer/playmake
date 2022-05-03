/**
  @author Felix Dumbeck
  Licensed under the GPLv3
  No warranty, provided as is
 */

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <getopt.h>
#include <regex.h>

int regMatch(char* string, char* exp){
	regex_t re;
	if (regcomp(&re, exp, REG_EXTENDED|REG_NOSUB) != 0)	return 0;
	int status = regexec(&re, string, 0, NULL, 0);
	regfree(&re);
	if (status != 0)	return 0;
	return 1;
}

int main(int argc, char** args){
	if(argc < 1 || argc > 8)	return 1;

	int opt = 0;
	char* dirname = NULL;	// directory with playable files
	char* filename = NULL;	// file/pathname for playlist file
	char* info = NULL;	// info about playlist like name-artist etc.
	char* filedir = NULL;	// directory of playlist file
				// handle options
	while((opt = getopt(argc, args, "d:f:i:p:")) != -1){
		if(opt == 'f')	filename = optarg;
		if(opt == 'd')	dirname = optarg;
		if(opt == 'i')	info = optarg;
		if(opt == 'p')	filedir = optarg;
		if(opt != 'f' && opt != 'd' && opt != 'i' && opt != 'p'){
			printf("Usage: playmake\n\t-d\tdirectory with playable files (default: ./)\n\t-f\tfile/pathname for pla    ylist file (    default: playlist.m3u)\n\t-i\tinfo about playlist like name-artist etc. (default: example)\n\t-d\tdirectory of playlist file\n");
			return 1;
		}
	}

	// if no options set, use defaults
	if(!dirname)	dirname = ".";
	if(!filename)	filename = "playlist.m3u";
	if(!info)	info = "example";
	if(!filedir)	filedir = ".";

	FILE* f;	// playlist file
	f = fopen(filename, "w");
	DIR* d;
	d = opendir(dirname);
	struct dirent *dent;
	if(d){
		while((dent = readdir(d)) != NULL){
			if(regMatch(dent->d_name, "(.+)(webm|mkv|flv|ogg|ogv|drc|avi|mov|qt|wmv|wmv|yuv|amv|mp4|mpg|mp3|flac)")){
				fputs(dirname, f);
				fputs("/", f);
				fputs(strcat(dent->d_name, "\n"), f);
			}
		}
		closedir(d);
	}
	fclose(f);

	return 0;
}
