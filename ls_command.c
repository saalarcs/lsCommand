/*--------------------------------------------------------------------------------------
# Listing Directory Contents Based on Selected Specifications
#
# Name: ls_command.c
#
# Written By: Saalar Faisal - 2021-02-23 
#
# Purpose: The purpose is list directory files and information similar to
#		   the ls command accepting the paramters of:
#			  1. -o <int> return files older than <int> days
#			  2. -y <int> return files younger than <int> days
#			  3. -l <int> return files larger than <int> bytes.
#		      4. -s <int> return files equal and smaller than <int> bytes.
#			  5. <directory_path> this is the path of the directory we are to
#				 get informations about.
#
#					Additional for directory_path:
#					1. It must be at the end of the parameters.
#					2. It can also be the only parameter provided.
#					3. If its not included, we will use Current Working Directory (CWD)
#					   to get directory information.
#
# Information to be Printed: Similar to the ls command:
#		
# --> We must have inode, type, permission, UID, GID, Modification Date, Size, FileName 
#
# SAMPLE Output:
#
#  inode    Type  Permission  UID   GID           Modification Date     Size    FileName
#  1838615  REG   rw-rw-rw-   root  CSOI-5623  2020-02-23 18:24:00   87489   jewel.jpg
#  1838617  REG   rw-rw-rw-   root  CSOI-5623  2000-07-29 07:35:28   47489   saalar.pdf
#
# ...
#
# To compile: gcc -o ls_command ls_command.c
#
# Usage 1: Return contents for CWD  
#
#		   >> ./ls_command 
#									
#
# Usage 2: Return contents for directory_path provided
#
#			>> ./ls_command <directoyPath>
#
#  			i.e. ./ls_command /home/file
#
#
# *Note: For the following cases we are using the directory path:
# 		 /home/file
#
#		 However, if we were not to specify this, it will automatically be
#		 the CWD.
#
# Usage 3: -o <int> parameter with specified directory
#
#
#			>> ./ls_command -o <int> <directoyPath>
#
#  			i.e. ./ls_command -o 10 /home/file
#  			i.e. ./ls_command -o 10 
#
#
# Usage 4: -y <int> parameter with specified directory
#
#
#			>> ./ls_command -y <int> <directoyPath>
#
#  			i.e. ./ls_command -y 5 /home/file
#  			i.e. ./ls_command -y 5 
#
#
# Usage 5: -l <int> parameter with specified directory
#
#
#			>> ./ls_command -l <int> <directoyPath>
#
#  			i.e. ./ls_command -l 6250 /home/file
#  			i.e. ./ls_command -l 6250
#
#
# Usage 6: -s <int> parameter with specified directory
#
#
#			>> ./ls_command -s <int> <directoyPath>
#
#  			i.e. ./ls_command -s 40000 /home/file
#  			i.e. ./ls_command -s 40000
#
#
# Usage 7: Mutliple parameters with specified directory
#
#
#			>> ./ls_command -o <int> -y <int> -l <int> -s <int> <directoyPath>
#
#  			i.e. ./ls_command -o 3650 -y 9862 -l 40000 -s 10000 /home/file
#  			i.e. ./ls_command -o 3650 -y 9862 -l 40000 -s 10000
#
#----------------------------------------------------------------------------------------*/

#include <dirent.h> // Structures for directory entries 
#include <stdio.h>  // Standard I/O functions 
#include <stdlib.h> // Commonly used library functions
#include <string.h> // Commonly used string-handling functions 
#include <sys/stat.h> // Detailed file information
#include <getopt.h> // Header used for parse command-line options
#include <time.h> // Time conversion library
#include <unistd.h> // Header file that provides access to the POSIX operating system API
#include <limits.h> // Header determines various properties of the various variable types

//True/False booleans
typedef enum { false = 0, true = !false } bool;

// Argument Struct, used for compiling our information
struct Arguments {
  bool o; // label for olderThan Time modification
  bool y; // label for youngerThan Time modification
  bool l; // label for file largerThan in bytes
  bool s; // label for file smallerThan in bytes
  int oVal; //stores values of label
  int yVal; //stores values of label
  int lVal; //stores values of label
  int sVal; //stores values of label
  char* directoryPath; //The directory path given by the user.
};  

// Prototypes
void getParam( int argc, char *argv[], struct Arguments *args ); 
char* getFiletype(struct stat fileStat);
void displayStatInfo(struct stat fileStat, char* filename);
char* formatDate(char* str, time_t val);
char* getFilePermissions(struct stat fileStat);

// main method
int main( int argc, char *argv[] ) {
 
  //Struct to hold our parameters   
  struct Arguments args;  
 // Stores values passed on parameter
 // args is the pointer for our Struct Argument 
  getParam(argc, argv, &args); 

  // store values for current working directory
  char* cwd;
  char buff[PATH_MAX + 1];

  cwd = getcwd( buff, PATH_MAX + 1 );
  
  // If no dirPath provided then we use current directory
  if(args.directoryPath == NULL || args.directoryPath == ""){	  
	args.directoryPath = cwd;
  }

  // Ensure that end of directory has a '/'
  if(args.directoryPath[strlen(args.directoryPath)-1] != '/'){
	  // add it
    strcat(args.directoryPath, "/");
  }
 
 // Set the labels of the value to our pointer args values
  int olderThan = args.oVal;
  int youngerThan = args.yVal;
  int largerThan = args.lVal;
  int smallerThan = args.sVal;

  // Create pointer for Directory
  DIR *dirp;
  // Directory entry                                  
  struct dirent *dp; 
  // Open it
  dirp = opendir(args.directoryPath); 

  //If no directory passed, send error message 
  // return path
  if (dirp == NULL) {                                    
   printf("Directory Not Found: '%s'\n", args.directoryPath); 
   return; 
  }

  //Header for file description
  printf("%-14s%-6d%-5s%-5s%-5s%-10s%-35s%-10s\n", "Permissions", "Inode", "Type", "UID", "GID", "ByteSize", "Filename", "Modification Date");


  //----Read each file----
  
  // Refers to independent file information
  struct stat file_status;
  
  // loop through directory
  while ((dp = readdir(dirp)) != NULL) { 
	// skip names of parent and current directory
    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue; 

    // File Path
    char file[PATH_MAX+1];
	// copy it to the directory
    strcpy(file, args.directoryPath); 
	// add based on filename
    strcat(file, dp->d_name); 

    // Reading file information
	// 0 means it has been compiled successfully
    if(stat(file, &file_status) == 0){
		
		// check if our labels have been called
        if(args.o || args.y || args.l || args.s) {
          
		  //int value of time modification older than
          if(olderThan) {
			  
            //store current time
            time_t currentTime = time(0);
			
			//get differnce based on modification date
            double diff = difftime(currentTime, file_status.st_mtime);
			
			// conversion to break-up time
            int days = diff/24/3600; 
			
			//skip these files that are not as old
            if(days < olderThan) {
              
              continue; } }
			  
          //similar to above, but for younger than
          if(youngerThan) {
			  
            //store current time
            time_t currentTime = time(0);
			
            //get differnce based on modification date
            double diff = difftime(currentTime, file_status.st_mtime); 
			
			// conversion to break-up time
            int days = diff/24/3600; 
			
			//skip these files that are older
            if(days > youngerThan){
              
              continue; } }
		  
		  // if bytes are larger than
		  if(largerThan){
			  
			  // store the file size
			off_t fileSize = file_status.st_size;
			
			//Skip any file that is smaller
			if (fileSize < largerThan)
			{continue;} 
		  }
		  
		  // if bytes are smaller than 
		  if(smallerThan){
			  // store the file size
			 off_t fileSize = file_status.st_size;
			 
			 //Skip any file that is larger
			 if (fileSize > smallerThan)
			{continue;} 
		  }		  	  
        }
		
        // Display the ones that are filtered
        displayStatInfo(file_status, dp->d_name);
		
    } else {
      //Error mesage
      printf("INVALID FILE PATH: %s \n", file);
    }
  }
  // close directory
  closedir(dirp); 

	// successfully completed
  return 0; 
}

// --------------Method "void getParam(..)"----------------
//   Role: Filters our parameter using the subroutine getOpt
//   Ref: https://man7.org/linux/man-pages/man3/getopt.3.html

void getParam(int argc, char *argv[], struct Arguments *args) {
  int c;
  extern char *optarg;
  
  //Loop through with our specified labels in the parameter
  while ((c = getopt(argc, argv, "y:o:l:s:")) != -1)  {
    
      //Case arguments
      switch(c) {
        case 'y':
            args->y = true;
            args->yVal = atoi(optarg);
            break;
        case 'o':
            args->o = true;
            args->oVal = atoi(optarg);
            break;
		case 'l':
            args->l = true;
            args->lVal = atoi(optarg);
            break;
		case 's':
            args->s = true;
            args->sVal = atoi(optarg);
            break;
        case '?':
          printf("Unrecognized paramater(s).. \n");
          break;
		  
        default:
          printf("Undefiend Error!... \n");
          break;
      }   
  }
  
  // Set last non-label value as directoryPath
  if (optind < argc) {
    args->directoryPath = argv[optind];
	} 
}

//--------- Method "char* getFiletype"------------
// Gets file type using Stat

char* getFiletype(struct stat fileStat){
  char* filetype;

 // print file types
  switch (fileStat.st_mode & S_IFMT) 
  {
    case S_IFREG:
      filetype = "REG";
    break;
    case S_IFDIR:
      filetype = "DIR";
      break;
    case S_IFCHR:
      filetype = "C_DEV";
    break;
    case S_IFBLK:
      filetype = "B_DEV";
    break;
    case S_IFLNK:
      filetype = "LINK";
    break;
    case S_IFIFO:
      filetype = "FIFO";
    break;
    case S_IFSOCK:
      filetype = "SOCK";
    break;
    default:
      filetype = "UNKN";
    break;
  }
  return filetype;
}

//-------------Method: char* formatDate(...) ---------------
// Converts linux time date into break down readable date

char* formatDate(char* str, time_t val){
  strftime(str, 40, "%d.%m.%Y %H:%M:%S", localtime(&val));
  return str;
}

//--------- Method "void displayStatInfo(...)"----------
// Role: Prints out file information), uses Stat header
// Param: fileStat : Holds file information loaded 
//        filename : Name of the file to display

void displayStatInfo(struct stat fileStat, char* filename){
  char* filetype = getFiletype(fileStat);
  char* permissions = getFilePermissions(fileStat);
  printf("%-14s", permissions); //file permissions
  printf("%-6ld", (long) fileStat.st_ino); // print the file's inode info
  printf("%-5s", filetype); //prints filetype
  printf("%-5ld", (long) fileStat.st_uid); //print the files owner user id
  printf("%-5ld", (long) fileStat.st_gid); //print the files owner group id
  printf("%-10lld", (long long) fileStat.st_size); // print the file size
  printf("%-35s", filename); //name of the file
  printf("%-10s", ctime(&fileStat.st_mtime)); // last modified time
}

//--------------Method "char* getFilePermissions(...)--------------
// Uses Stat to give us file permission

char* getFilePermissions(struct stat fileStat){
	// max memory alloc
  char permissions[PATH_MAX+1]; 

  // prints type of file
  // Is it a Link
  if(S_ISLNK(fileStat.st_mode)){

    strcpy(permissions, S_ISLNK(fileStat.st_mode) ? "l" : "-");
  } else {
    //Is it a Directory
    strcpy(permissions, S_ISDIR(fileStat.st_mode) ? "d" : "-");
  }

  //USER PERMS
  strcat(permissions, (fileStat.st_mode & S_IRUSR) ? "r" : "-");
  strcat(permissions, (fileStat.st_mode & S_IWUSR) ? "w" : "-");
  strcat(permissions, (fileStat.st_mode & S_IXUSR) ? "x" : "-");
  //GROUP PERMS
  strcat(permissions, (fileStat.st_mode & S_IRGRP) ? "r" : "-");
  strcat(permissions, (fileStat.st_mode & S_IWGRP) ? "w" : "-");
  strcat(permissions, (fileStat.st_mode & S_IXGRP) ? "x" : "-");
  //WORLD PERMS
  strcat(permissions, (fileStat.st_mode & S_IROTH) ? "r" : "-");
  strcat(permissions, (fileStat.st_mode & S_IWOTH) ? "w" : "-");
  strcat(permissions, (fileStat.st_mode & S_IXOTH) ? "x" : "-");

  return permissions;
}
