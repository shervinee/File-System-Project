#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include"P5aHeaders.h"



/* this function find the hidden file in the
 * specified directory (sorted by non-decreasing
 * order of logical file size) */
FileN *findHiddenFiles(const char filePath[])
{
  FileN* root;
   root = NULL;
  char fullPath[256];
    int count = 0; /* to find the root of the tree */
  DIR* dir;/* to open the directory to read */
  struct dirent* dp;
  struct stat st;  
  if ((dir = opendir(filePath)) == NULL)
    {
    fprintf(stderr, "Invalid directory\n");
    exit(-1);
    }
  /* check each file in the directory
   * if its a hidden file, add into the list */
  while ((dp = readdir(dir)) != NULL)
    {
        
        const char* name = dp->d_name;
        
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)/* we don't want the '.' and '..' files in our list */
        {
            continue;
        }
    /* check if this is a hidden file (or hidden directory) */
        if (name[0] != '.')
        {
            continue;
        }
    /* get the logic file size and last
     * modification date*/
        sprintf(fullPath, "%s/%s", filePath, name);
        if (stat(fullPath, &st) < 0)
        {
            printf("Could not call stat to file '%s'\n", fullPath);
            continue;
        }

        //if it's the first hidden file in the directory i.e root of the tree
        if(count == 0)
        {
            root = insert(root,st.st_size,name,st.st_mtime);
            ++count;
        }
        //if it's the following elements
        if(count != 0)
        {
            insert(root,st.st_size,name,st.st_mtime);
        }

        
  }

  /* we close the directory as we may not be able to have mutiple directories being openned, at last we return the root of BST */
  closedir(dir);
  return root;
}

int main(int argc, char *argv[])
{
  FileN* rootOfHidden;

  /* check the parameters */
  if (argc != 2 && argc != 3) {
    fprintf(stderr, "Invalid number of arguments\n");
    exit(1);
  }
  if (argc == 3 && strcmp(argv[2], "-s") != 0) {
    fprintf(stderr, "Invalid option flag\n");
    exit(1);
  }

  rootOfHidden = findHiddenFiles(argv[1]);
  /* display the result in the list */
  if (rootOfHidden == NULL) {
    printf("The directory had no hidden files\n");
  } 
  else
  {
    
      PrintIn(rootOfHidden);
  }
  return 0;
}
