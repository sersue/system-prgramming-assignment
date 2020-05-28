/*3주차 문제
getcwd()와 동일한 동작을 실행하는 함수를 구현하라. 
getcwd()의 성공/실패 여부와 상관 없이, 탐색을 시작한 것과 동일한 디렉토리에 존재해야 함
인자가 NULL인 경우는 고려할 필요 없음

(힌트)
부모 디렉토리(..)의 각 엔트리를 돌면서 opendir()과 readdir()을 사용하면 현재 작업 디렉토리의 이름을 알 수 있음
현재 작업 디렉토리와 동일한 i-노드와 디바이스 번호를 가진 엔트리를 
 찾으면 현재 디렉토리가 무엇인지 알 수 있음
한 번에 한 단계씩 살펴보고, 스캔을 통해 디렉토리 경로를 구축해볼 것
부모 디렉토리가 현재와 동일하다면(루트 디렉토리의 경우), 검색 종료 */


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>

void creatEnv();

/**
 
 * @param buf   경로명을 저장할 버퍼 변수.
 * @param size  버퍼 변수의 크기
 * @return 버퍼 변수의 시작 주소, 즉 buf
 */
char *mygetcwd(char *buf, size_t size) {
  DIR *dir;
  struct stat statbuf;
  struct dirent *dp;
  char tmpt[size], str[size];
  
  do {
    stat(".", &statbuf);
    if(dir=(opendir("..")) == NULL) {
      printf("open failed");
    }
    
    for(;;){
      if((dp = readdir(dir)) == NULL)
        break;
  
      if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
        continue;
    
      if(dp->d_ino == statbuf.st_ino) {
        sprintf(str,"%s%s%s","/",dp->d_name,tmpt);
        strcpy(tmpt,str);
        break;
      }
    }
    
    if (chdir("..")) {
      perror("error");
      exit(1);
    }
  } while (strcmp(dp->d_name,"home"));
  
  buf = str;
  return buf;
}

int main(void) {
  pid_t pid;
  int status;
  char buf[255];

  creatEnv();
  chdir("dir/sub"); //현재 디렉토리 dir/sub

  printf("original func: %s\n", getcwd(NULL, 0));
  printf("mygetcwd func: %s\n", mygetcwd(buf, 255));

  return 0;
}

void creatEnv(){
  mkdir("dir", 0755); //mkdir(파일 생성)는 디렉토리가 만들어져 있지 않은데에 만들면 오류 dir만들고 -> dir/sub
  mkdir("dir/sub", 0755);
  mkdir("dir/sub2", 0);
  
  creat("dir/a", 0755); //creat는 쓰기전용 파일 생성
  creat("dir/b", 0755);
  creat("dir/sub/x", 0755);
  symlink("dir/a", "dir/sl");
  symlink("dir/x", "dir/dsl"); //symlink(old path , new path) oldpath 에 대한 새로운 symlink생성
}