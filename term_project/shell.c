#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <dirent.h>
#include <ctype.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <grp.h>
#include <errno.h>
#define BUFFER 1024
#define MAX_HISTORY 10
#define MAX_COMMAND 10


char history[MAX_HISTORY][100];
int Index = -1;

static void makepath(const char *path);
static void error(char *s);




void cmd(char *getcommand, char **env);
void signall(int signum);

int main(int argc, char *argv[], char **env){
    char getcommand[100];   
    char *commmandLine;
    char *cwd;
    char *home= getenv("HOME");

    if (argc>1){
        perror("Argument error");
        exit(1);
    }
 char name[1024];
    uid_t user_id;
    struct passwd *user_pw;
   
    user_id  = getuid();
    user_pw  = getpwuid( user_id );
    gethostname(name, 1024);

   

    system("clear");
   
    while(1){
        cwd = getcwd(NULL, BUFFER);
        printf("%s@%s" ":", user_pw->pw_name, name);
        printf("%s $ ", (strcmp(home, cwd)==0) ? "~" : strrchr(cwd, '/')); 
        gets(getcommand);

        if(strcmp(getcommand, "\0")!=0){ 
            strcpy(history[(++Index) % MAX_HISTORY], getcommand);
            if(Index > 9){
                Index %= Index;
            }
        }

        if(strstr(getcommand, "|")!=NULL || strstr(getcommand, ">")!=NULL || strstr(getcommand, "<")!=NULL ){
            system(getcommand);
        }
        else {   
            cmd(getcommand, env);
        } 
    }
    exit(0);
}

void cmd(char *getcommand, char **env){
    char *commandarr[MAX_COMMAND] = { NULL, };


    char *commandline = strtok(getcommand, " ");
    int cmdlength = 0;     

    while (commandline != NULL){
        commandarr[cmdlength] = commandline;
        cmdlength++;
        commandline = strtok(NULL, " ");
    }
    if(commandarr[0]!=NULL){
        if (strcmp(commandarr[0], "exit")==0){ 
            puts("Bye");
            exit(0);
        }
        
        else if(strcmp(commandarr[0], "cd")==0){
            if(cmdlength==1){
                chdir(getenv("HOME"));
            }
            else if(cmdlength==2){
                if(chdir(commandarr[1])){
                    printf( "No such file or directory\n");
                }
            }
            else{
                printf("ERROR: incorrect form\n");
                puts("cd [dir]" );
            }
        }
 else if(strcmp(commandarr[0], "history")==0){  
            int historyStart = (Index%MAX_HISTORY);

            for(int i=historyStart+1; i<=historyStart+MAX_HISTORY; i++){
                if(strcmp(history[i%MAX_HISTORY], "") != 0){
                    printf("%s\n", history[i%MAX_HISTORY]);
                }
            }
        }
        else if(strcmp(commandarr[0], "ls")==0){ 
            
            pid_t pid = fork();
            int status;
            switch (pid){
                case -1:
                    perror("FORK ERROR");
                    exit(1);
                    break;
                case 0:
                    for(int i=0; commandarr[i]!=NULL; i++) ;
                    char **newcommandarr = (char **)malloc(sizeof(char *)*(cmdlength));

                    for(int idx=0; idx<cmdlength; idx++){
                        newcommandarr[idx] = commandarr[idx];
                    }
                    newcommandarr[cmdlength] = NULL;
                    if(execvp("ls", newcommandarr)==-1){
                        printf("ERROR");
                        exit(1);
                    }
                    exit(3);  
                    break;
                default:
                    wait(&status);
                    break;
            }

        }

       
        
        else if(strcmp(commandarr[0], "man")==0){ 
            if(cmdlength!=2){
                puts( "ERROR" );
            }
            else{
                pid_t pid = fork();
                int status;
                switch (pid){
                    case -1:
                        perror("FORK ERROR");
                        exit(1);
                        break;
                    case 0:
                        for(int i=0; commandarr[i]!=NULL; i++) ;
                        char **newcommandarr = (char **)malloc(sizeof(char *)*(cmdlength));

                        for(int idx=0; idx<cmdlength; idx++){
                            newcommandarr[idx] = commandarr[idx];
                        }
                        newcommandarr[cmdlength] = NULL;
                        if(execvp("man", newcommandarr)==-1){
                            printf("ERROR");
                            exit(1);
                        }
                        exit(3);
                        break;

                    default:
                        wait(&status);
                        break;
                }
            }
        }
        else if(strcmp(commandarr[0], "chmod")==0){ 
            if(cmdlength!=2){
                puts( "ERROR" );
            }else{
                int mode = strtol(commandarr[1], NULL, 8);

                for(int i=2; i<cmdlength; i++){
                    if(chmod(commandarr[i], mode) < 0){
                        printf("%s : chmod ERROR\n", commandarr[i]);   
                    }
                }
            }
        }

        else if(strcmp(commandarr[0], "pwd")==0){ 
            puts(getcwd(NULL, BUFFER));
        }

        


        /**/

        else if(strcmp(commandarr[0], "rm")==0){ 
            pid_t pid = fork();
            int status;
            switch (pid){
                case -1:
                    perror("FORK ERROR");
                    exit(1);
                    break;
                case 0:
                    for(int i=0; commandarr[i]!=NULL; i++){

                    }
                    char **newcommandarr = (char **)malloc(sizeof(char *)*(cmdlength));

                    for(int idx=0; idx<cmdlength; idx++){
                        newcommandarr[idx] = commandarr[idx];
                    }
                    newcommandarr[cmdlength] = NULL;
                    if(execvp("rm", newcommandarr)==-1){
                        printf("ERROR");
                        exit(1);
                    }
                    exit(3);  
                    break;

                default:
                    wait(&status); 
                    break;
            }
        }
else if(strcmp(commandarr[0], "ps")==0){ 
            
            pid_t pid = fork();
            int status;
            switch (pid){
                case -1:
                    perror("FORK ERROR");
                    exit(1);
                    break;
                case 0:
                    for(int i=0; commandarr[i]!=NULL; i++) ;

                    char **newcommandarr = (char **)malloc(sizeof(char *)*(cmdlength));

                    for(int idx=0; idx<cmdlength; idx++){
                        newcommandarr[idx] = commandarr[idx];
                    }
                    newcommandarr[cmdlength] = NULL;
                    if(execvp("ps", newcommandarr)==-1){
                        printf("ERROR");
                        exit(1);
                    }
                    exit(3);  
                    break;

                default:
                    wait(&status); 
                    break;
            }

        }
        else if(strcmp(commandarr[0], "echo")==0){ 
            if(cmdlength>1){
                if(commandarr[1][0]=='$'){ 
                    char *echoMsg;
                    echoMsg = (char *)malloc(strlen(commandarr[0])+strlen(commandarr[1])+2);
                    strcat(echoMsg, "echo ");
                    strcat(echoMsg, commandarr[1]);
                    system(echoMsg);
                }
                else{
                    for(int i=1; commandarr[i]!=NULL; i++){
                        printf("%s ", commandarr[i]);
                    }
                    printf("\n");
                }
            }
            else if(cmdlength==1){
                puts( "echo" );
            }
        }

        
        
        else if(strcmp(commandarr[0], "grep")==0){ 
            pid_t pid = fork();
            int status;
            switch (pid){
                case -1:
                    perror("FORK ERROR");
                    exit(1);
                    break;
                case 0:
                    for(int i=0; commandarr[i]!=NULL; i++) ;

                    char **newcommandarr = (char **)malloc(sizeof(char *)*(cmdlength));

                    for(int idx=0; idx<cmdlength; idx++){
                        newcommandarr[idx] = commandarr[idx];
                    }
                    newcommandarr[cmdlength] = NULL;
                    if(execvp("grep", newcommandarr)==-1){
                        printf("ERROR");
                        exit(1);
                    }
                    exit(3);  
                    break;

                default:
                    wait(&status); 
                    break;
            }
        }

        else if(strcmp(commandarr[0], "env")==0){ 
            if(cmdlength!=1){
                puts( "put command : env" );
            }
            else{
                char **env = env;
                while(*env){
                    printf("%s\n", *env);
                    env++;
                }
            }
        }
   else if(strcmp(commandarr[0], "mkdir")==0){ 
            for(int i=1; commandarr[i]!=NULL; i++){
                makepath(commandarr[i]);
            } 
        }

        else if(strcmp(commandarr[0], "rmdir")==0){ 
            for(int i=1; commandarr[i]!=NULL; i++){
                if(rmdir(commandarr[i]) < 0){
                    printf( "%s: rmdir ERROR\n" , commandarr[i]);
                    break;
                }
            }
        }
        else if(strcmp(commandarr[0], "cat")==0){ 
            FILE *f;
            int c;
            for(int i=1; commandarr[i]!=NULL; i++){
                f = fopen(commandarr[i], "r");
                if(!f){
                    printf( "%s: No such file or directory\n" , commandarr[i]);
                    break;
                }
                while((c=fgetc(f)) != EOF){
                    if(putchar(c) < 0 ){
                        puts("EOF ERROR!");
                        break;
                    }
                }
            }
        }

     


        else if(strcmp(commandarr[0], "cls")==0){ 
            system("clear");
        }
 else if(strcmp(commandarr[0], "mv")==0){ 
            if(cmdlength!=3){
                puts( "incorrect form \n" );
            }

            if(rename(commandarr[1], commandarr[2])<0){
                printf("ERROR[%s]\n", commandarr[2]);
            }
        }

        
        else if(strcmp(commandarr[0], "ln")==0){ 
            if(cmdlength!=3){
                puts( "incorrect form!\n" );
            }

            if(link(commandarr[1], commandarr[2])<0){
                printf("ERROR[%s] \n", commandarr[2]);
            }
        }

       

        else if(strcmp(commandarr[0], "vi")==0 || strcmp(commandarr[0], "vim")==0){ 
            if(cmdlength!=2){
                puts( "incorrect form!\n");
            }
            else{
                pid_t pid = fork();
                int status;
                switch (pid){
                case -1:
                    perror("FORK ERROR");
                    exit(1);
                    break;
                case 0:
                    for(int i=0; commandarr[i]!=NULL; i++) ;
                    
                    char **newcommandarr = (char **)malloc(sizeof(char *)*(cmdlength));

                    for(int idx=0; idx<cmdlength; idx++){
                        newcommandarr[idx] = commandarr[idx];
                    }
                    newcommandarr[cmdlength] = NULL;
                    if(execvp("vi", newcommandarr)==-1){
                        printf("ERROR");
                        exit(1);
                    }
                    exit(3);  
                    break;

                default:
                    wait(&status); 
                    break;
                }
            }
        }
else if(strcmp(commandarr[0], "date")==0){ 
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            
            printf("%d-%d-%d %d:%d:%d\n",tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,tm.tm_hour, tm.tm_min, tm.tm_sec);
        }
        else if(strcmp(commandarr[0], "make")==0){ 
            pid_t pid = fork();
                int status;
                switch (pid){
                case -1:
                    perror("FORK ERROR");
                    exit(1);
                    break;
                case 0:
                    for(int i=0; commandarr[i]!=NULL; i++) ;
                    
                    char **newcommandarr = (char **)malloc(sizeof(char *)*(cmdlength));

                    for(int idx=0; idx<cmdlength; idx++){
                        newcommandarr[idx] = commandarr[idx];
                    }
                    newcommandarr[cmdlength] = NULL;
                    if(execvp("make", newcommandarr)==-1){
                        printf("ERROR");
                        exit(1);
                    }
                    exit(3);
                    break;

                default:
                    wait(&status); 
                    break;
                }
        }

        

        else{ 
            puts("Unknown Command" );
        }
        
    } 
}

static void error(char *s){
    perror(s);
    exit(1);
}

static void makepath(const char *path){

    if(mkdir(path, 0777)==0){
	    return;
    }
    
    if (errno == EEXIST) {
        struct stat st;
        if (stat(path, &st) < 0){
            perror("stat");
            exit(1);
        }
        
        if (!S_ISDIR(st.st_mode)) {
            fprintf(stderr, "not a directory: %s\n", path);
            exit(1);
        }
        return;
    }
    else if (errno == ENOENT) {
        char *parent_path = strdup(path);
        if (!parent_path){
            perror("stat");
            exit(1);
        }

        char *last = parent_path + strlen(parent_path) - 1;
        while (*last == '/' && last != parent_path) {
            *last-- = '\0';
        }

        if (strcmp(parent_path, "/") == 0) {
            fprintf(stderr, "error: root directory is not a directory\n");
            exit(1);
        }

        char *sep = strrchr(parent_path, '/');
        if (!sep) {
            fprintf(stderr, "error: current directory is not a directory\n");
            exit(1);
        }
        else if (sep == parent_path) {
            fprintf(stderr, "error: root directory is not a directory\n");
            exit(1);
        }
        *sep = '\0';

        makepath(parent_path);
        mkdir(path, 0777);
        return;
    }

    else {
        perror(path);
        exit(1);
    }
}

void signall(int signum){
    puts("\n no break" );
}
