int main(int argc, char *argv[]){
    int pipefd[2], status, done=0;
    pid_t cpid;

    pipe(pipefd);

    cpid = fork();
    if(cpid == 0){
        close(pipefd[0]);
        dup2(pipefd[1],STDOUT_FILENO);
        execlp(argv[1],argv[1], (char *)NULL);
    
    }
}