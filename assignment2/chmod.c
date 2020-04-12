#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void fatal(const char* str, int no);
void access_perm(char *perm, mode_t mode);
void chmod_arX(char *list[]){
  int i;
 
  if(chmod(list[0],S_IXUSR|S_IXGRP|S_IXOTH|S_IRUSR|S_IRGRP|S_IROTH)<0){
    perror("chmod error");
    exit(1);
  }
  for(i=1;list[i]!=NULL;i++){
    if(access(list[i],X_OK)==0){
  chmod(list[i],S_IXUSR|S_IXGRP|S_IXOTH|S_IRUSR|S_IRGRP|S_IROTH);
    }
    else if(access(list[i],F_OK)==0){
      chmod(list[i],S_IRUSR|S_IRGRP|S_IROTH);
    }
    else{
      perror("chmoderror");
      exit(1);
    }
  }
}  
int main(void) {
  struct stat statbuf;
  char perm[11];
  char pathname[80];
  char *list[] = {"dir1", "file1", "file2", NULL};
  int fd1, fd2, d, i;

  rmdir(list[0]);
  remove(list[1]);
  remove(list[2]);

  mkdir(list[0], 0400);
  fd1 = creat(list[1], 0500);
  fd2 = creat(list[2], 0500);
  
  printf("===== before =====\n");
  for(i = 0; list[i] != NULL; i++){
    if (stat(list[i], &statbuf) < 0) fatal("stat error", 1);
    access_perm(perm, statbuf.st_mode);
    printf("%s\t%s\n", list[i], perm);
  }
  
  chmod_arX(list);
  
  printf("\n===== after ======\n");
  for(i = 0; list[i] != NULL; i++){
    if (stat(list[i], &statbuf) < 0) fatal("stat error", 1);
    access_perm(perm, statbuf.st_mode);
    printf("%s\t%s\n", list[i], perm);
  }
  close(fd1);
  close(fd2);
  return 0;
}

void fatal(const char* str, int no){
  perror(str);
  exit(no);
}

void access_perm(char *perm, mode_t mode) {
  char permchar[] = "rwx";
  memset(perm, '-', 10);
  perm[10] = '\0';
  int i;
  
  if (S_ISDIR(mode)) perm[0] = 'd';
  else if (S_ISCHR(mode)) perm[0] = 'c';
  else if (S_ISBLK(mode)) perm[0] = 'b';
  else if (S_ISFIFO(mode)) perm[0] = 'p';
  else if (S_ISLNK(mode)) perm[0] = 'l';
  
  for (i = 0; i < 9; i++)
    if ((mode << i) & 0x100)
      perm[i+1] = permchar[i%3];
      
  if (mode & S_ISUID) perm[3] = 's';
  if (mode & S_ISGID) perm[6] = 's';
  if (mode & S_ISVTX) perm[9] = 't';
}