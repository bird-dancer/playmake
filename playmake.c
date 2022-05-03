/**
  @author Felix Dumbeck
  Licensed under the GPLv3
  No warranty, provided as is
 */

#define _DEFAULT_SOURCE

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

int createPlaylist(FILE* f, char* dirname, int recursive){
	DIR* d = opendir(dirname);
	if(!d){
		printf("failed to open directory\n");
		return 1;
	}

	struct dirent* dent;
	while((dent = readdir(d)) != NULL){
		if(recursive == 1 && dent->d_type == DT_DIR && strncmp(dent->d_name, ".", 2) && strncmp(dent->d_name, "..", 3))
			createPlaylist(f, dent->d_name, recursive);
		if(regMatch(dent->d_name, "(.+)(webm|mkv|flv|ogg|ogv|drc|avi|mov|qt|wmv|m4p|yuv|amv|mp4|mpg|mp3|flac)")){
			fputs(dirname, f);
			fputs("/", f);
			fputs(strcat(dent->d_name, "\n"), f);
		}
	}
	closedir(d);
	return 0;
}

int main(int argc, char** args){
	if(argc < 1 || argc > 8)	return 1;

	int opt = 0;
	char* dirname = NULL;	// directory with playable files
	char* filename = NULL;	// file/pathname for playlist file
	char* info = NULL;	// info about playlist like name-artist etc.
	int recursive = 0;	// folders should be crawled recursivly	
				// handle options
	while((opt = getopt(argc, args, "d:f:i:rh")) != -1){
		switch(opt){
			case 'f':
				filename = optarg;
			case 'd':
				dirname = optarg;
			case 'i':
				info = optarg;
			case 'r':
				recursive = 1;
			case 'h':
				printf("Usage: playmake\n\t-d\tdirectory with playable files (default: .)\n\t-f\tfile/pathname for playlist file (default: playlist.m3u)\n\t-i\tinfo about playlist like name-artist etc. (default: example)\n\t-r\tcrawl subdirectories recursivly\n\t-h\thelp\n");
		}
	}


	// if no options set, use defaults
	if(!dirname)	dirname = ".";
	if(!filename)	filename = "playlist.m3u";
	if(!info)	info = "example";

	FILE* f;	// playlist file
	f = fopen(filename, "w");
	if(!f){
		printf("failed to open file\n");
		return 1;
	}

	createPlaylist(f, dirname, recursive);

	fclose(f);
	return 0;
}
