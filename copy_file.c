#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <fcntl.h> 
#define BUFSIZE 512 /* 한 번에 읽을 문자열의 크기 */
#define PERM 0644 /* 새로 만든 파일의 사용 권한 */ 

/**
 * 파일을 복사하여 사본을 생성한다.
 * @param name1 원본 파일 이름
 * @param name2 사본 파일 이름
 * @return 처리 결과를 숫자 형태로 반환 (자유롭게 정의하여 사용)
 */
int copy_file(const char *name1, const char *name2); 
void create_holed_file(const char *str);
void fatal(const char *str, int errno);

int main(){
    create_holed_file("file.hole");
    return copy_file("file.hole", "file2.hole");
  
}

int copy_file(const char *name1, const char *name2) { 
  /*홀이 있어도 홀도 복사해와야함*/
  /*open, read, write, close를 이용*/
  /* 함수 내용 작성, 하고 두 함수 다 닫아야함 */
  int fd1,fd2,n;
  char buf[BUFSIZE];

  if((fd1 = open("file.hole", O_RDONLY)) == -1){
      perror("file open error");
      exit(1);
  }
  
  if((fd2 = creat(name2,PERM)) < 0){
      perror("file open error");
      exit(1);
    
  }
  while((n = read(fd1,buf,BUFSIZE)) > 0){
    if(write(fd2,buf,n) < n){
        close(fd1);
        close(fd2);
        exit(1);
    }
  }
  close(fd1);
  close(fd2);
  
  if(n == -1){
      perror("file read error");
      exit(1);
  }
  else {
      return 0;
  }
} 

void create_holed_file(const char *str){
    char buf1[] = "abcdefghij", buf2[] = "ABCDEFGHIJ";
    int fd; 
/* file.hole 생성됨 
  abcdefghij
  **********
  **********
  **********
  ABCDEFGHIJ
  이걸 file.hole2에 복사*/
  if ((fd = creat(str, 0640)) < 0)
      fatal("creat error", 1);

  if (write(fd, buf1, 10) != 10)
      fatal("buf1 write error", 1);
    
  if (lseek(fd, 40, SEEK_SET) == -1) 
      fatal("lseek error", 1);

  if (write(fd, buf2, 10) != 10) 
      fatal("buf2 write error", 1);
}

void fatal(const char *str, int errcode){
     perror(str);
     exit(errcode);
}
