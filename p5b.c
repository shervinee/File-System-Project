#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "P5bHeaders.h"
#include"tar.h"

int main(int argc, char *argv[])
{
  if (argc < 3) {
    fprintf(stderr, "Invalid number of arguments\n");
    exit(1);
  }
    
    /* creates the specified archive */
  if (strcmp(argv[1], "-c") == 0)
  {
    archive(argv[2], &argv[3], argc - 3);

  }
    
   else if (strcmp(argv[1], "-x") == 0) 
  {
    /* extract should have exact the archive
     * name in the argument */
    if (argc != 3)
    {
      fprintf(stderr, "Invalid number of arguments\n");
      exit(1);
    }
    extract(argv[2]);
  }
    
    /* prints the name and size of the given prefix
     * should have two more arguments */
  else if (strcmp(argv[1], "-p") == 0)
  {
    
    if (argc != 4) 
    {
      fprintf(stderr, "Invalid arguments\n");
      exit(1);
    }
    printFiles(argv[3], argv[2]);
  } 
  else 
  {
    fprintf(stderr, "Invalid arguments\n");
  }

  return 0;
}

