#include "P5bHeaders.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void checkedOpen(int iNode)
{
    if(iNode == -1)
    {
        fprintf(stderr,"Could not open(system call) the file");
        exit(1);
    }
}
void checkedClose(int fd)
{
    if(fd == -1)
    {
        fprintf(stderr,"Could not close(system call) the file");
    }
}

/* archive the given file list,
 * save the archive into the path of a file. */
void archive(const char pathName[], char *files[], char count)//double pointer for the array of files
{
  int i;
  struct stat st;
  char content[1024];
  /* opens our target file(archive) to write into with the desired permissions */
  int outputF = open(pathName, O_CREAT | O_TRUNC | O_WRONLY, 0666);//number is in octal
  
   checkedOpen(outputF);
  /* save the number of files */
  write(outputF, &count, 1);
  for (i = 0; i < count; i++) 
  {
      
    unsigned char L = strlen(files[i]);
      
    int Z;
    /* get the size of file using the stat structure */
    if (stat(files[i], &st) == -1) 
    {
      fprintf(stderr, "Could not use the stat and check size of file '%s'\n", files[i]);
      Z = 0;
    } 
    else 
    {
      Z = st.st_size;
    }
    
      /* write the three values of L/S/Z of the file into the archive file */
    write(outputF, &L, 1);
    write(outputF, files[i], L);
    write(outputF, &Z, sizeof(int));
  }
  
  /* now save each file content into the archive file */
  for (i = 0; i < count; i++) 
  {
     int iNode = open(files[i], O_RDONLY);
    
      checkedOpen(iNode);//to check if it has been openned
      
      /* copy the file content */
      ssize_t bytes;
      while ((bytes = read(iNode, content, sizeof(content))) > 0)//checking for all the file
      {
        write(outputF, content, bytes);//writing the size of each of them
      }
    
    close(iNode);//closing the individual file
      checkedClose(iNode);
  }

  close(outputF);//closing the archive
    checkedClose(outputF);
}


/* extracts the archive file into current directory */
void extract(const char pathName[])
{
  int iNode;// to open the file
  char count;
  int i, j;
  off_t indexOffset = 1;
  off_t dataOffset;
  char buf[1024];
  
  iNode = open(pathName, O_RDONLY);
  //we first check if we could open the file properly
  checkedOpen(iNode);
  /* read the number of files from the archive */
  read(iNode, &count, 1);
  /* calculate the offset of the file data section
   * in the archive file */
  for (i = 0; i < count; i++) 
  {
    /* read L S Z */
    unsigned char L;
    
    int Z;
    read(iNode, &L, 1);
    /* we can skip the file name after having L */
      lseek(iNode, (long)L, SEEK_CUR);
      /* read the file size */
    read(iNode, &Z, sizeof(int));
  }
  
  dataOffset = lseek(iNode, 0, SEEK_CUR);
  /* now rewind the archive file
   * and save each file into current folder */
  for (i = 0; i < count; i++) 
  {
    /* read L, S, Z, */
    unsigned char L;
    char *S;
    int Z;
    int output;
    lseek(iNode, indexOffset, SEEK_SET);//rewinding
      
    read(iNode, &L, 1);//reading the number of bytes
      
    S = (char *)calloc(L + 1, 1);
    /* read the name and size of the file */
    read(iNode, S, L);
    read(iNode, &Z, sizeof(int));
    /* save our indexOffset for the next file */
      
    indexOffset = lseek(iNode, 0, SEEK_CUR);
    /* create an output file to save into */
    output = open(S, O_CREAT | O_TRUNC | O_WRONLY, 0666);
    
    checkedOpen(output);
    free(S);
    /* move to the data section of the file that we have stored */
    lseek(iNode, dataOffset, SEEK_SET);
      
    /* read the file and save into the output file */
    while (Z > 0) 
    {
      int len = sizeof(buf);
      int bytes;
      if (len > Z)
          len = Z;
      bytes = read(iNode, buf, len);
      if (bytes > 0) 
      {
        Z -= bytes;
        if (output != -1) { /* write onto the output file */
          write(output, buf, bytes);
        }
      } 
      else 
      {
        break;
      }
    }
    if (output != -1) 
    {
      close(output);
        checkedClose(output);
    }
    /* now we save the offset for the data section for the next file */
    dataOffset = lseek(iNode, 0, SEEK_CUR);
  }
  close(iNode);
    checkedClose(iNode);
}

/* walk through the files in the archive file,
 * print the name and file size with the prefix in the file name. */
void printFiles(const char pathName[], const char prefix[])
{
  int iNode;//archive file
  char count;//number of files
  int i;
  int number = 0;//to hold the number of matches
    
  iNode = open(pathName, O_RDONLY);
  checkedOpen(iNode);
  /* read the number of files from the archive */
  read(iNode, &count, 1);
  /* calculate the offset of the file data section
   * in the archive file */
  for (i = 0; i < count; i++) 
  {
    /* read L S Z */
    unsigned char L;
    char  *S;
    int Z;
    read(iNode, &L, 1);
    /* read the file size */
    S = (char *)calloc(L + 1, 1);
    /* read the name and size of the file */
    read(iNode, S, L);
    read(iNode, &Z, sizeof(int));
    //check to see if the name matches
    if (strncmp(S, prefix, strlen(prefix)) == 0) 
    {
      ++number;
      /* found a file of prefix in the name, print it */
      printf("%-25s\t with size of: %d\n", S, Z);
    }
    free(S);
  }
  close(iNode);
    checkedClose(iNode);
  if (number == 0) 
  {
    printf("No matching files found.\n");
  }
}
