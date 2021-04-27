# lsCommand
A C program to list directory files and information similar to the ls command accepting the parameters


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
