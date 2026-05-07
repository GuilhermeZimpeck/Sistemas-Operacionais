#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LINE 1024
#define MAX_ARGS 100
#define MAX_PATHS 100
#define ERROR_MSG "An error has occurred\n"

char* paths[MAX_PATHS];
int path_count = 0;

void print_error() {  
    write(STDERR_FILENO, ERROR_MSG, strlen(ERROR_MSG));      //printa a mensagem de erro no stderr definida por parametro
}

char* trim(char* s) {
    if (!s)
        return NULL;
    while(*s==' '||*s=='\t'||*s=='\n'||*s=='\r')        //remove espaços
        s++;
    if(*s==0)       //verifica se esta vazia
        return s;
    char* end = s + strlen(s)-1;        //vai até a ultima casa
    while(end > s && (*end==' '||*end=='\t'||*end=='\n'||*end=='\r'))       //remove espaços no final
    { 
        *end=0;     //coloca \0 no fim da string
        end--; 
    }
    return s;
}

char** parse_tokens(char* cmd, int* argc) {
    if (!cmd) 
    { 
        *argc=0;              //se a sting é vazia argc = 0
        return NULL; 
    }
    char** argv = malloc(MAX_ARGS * sizeof(char*));     //aloca memoria, e cria um vetor de ponteiro de char 
    if (!argv)           //verifica erro na string
    { 
        print_error(); 
        exit(1); 
    }
    int i = 0;
    char* token = strtok(cmd, " \t\r\n");       //divide a string pelos delimitadores " \t\r\n"
    while (token && i < MAX_ARGS-1)         //loop enquanto houver tokens      
    { 
        argv[i++] = token;          //guarda cada palavra no vetor
        token = strtok(NULL," \t\r\n"); 
    }
    argv[i] = NULL; 
    *argc = i;          //define quantidade de elementos
    return argv;
}

void builtin_path(int argc, char** argv) {        //procura o path de executaveis para rodalos
    for(int i=0;i<path_count;i++)
    { 
        free(paths[i]);         //limpa os paths antigos
        paths[i]=NULL; 
    }
    path_count=0;       //inicia novos paths
    for(int i=1;i<argc;i++)
    { 
        if(path_count<MAX_PATHS) 
            paths[path_count++]=strdup(argv[i]); 
    }
}

void builtin_cd(int argc, char** argv) {
    if(argc != 2 || chdir(argv[1]) != 0)        //verifica se a quantidade certa de argumentos doi passada, e muda o diretorio com chdir()
        print_error();          //se diretorio não existe ou mais de 2 argumentos foram passados printa o erro
}
// COMANDOS PASSADOS PARA execute_command( lista de argumentos , número de argumentos , arquivo para redirecionamento , se deve rodar em background , controle dos processos em background)
int execute_command(char** argv, int argc, char* out_file, int background, pid_t* bg_pids, int* bg_count) {
    
    //PRIMEIRO os comandos internos (builtin), aqueles que não usam fork()

    if(argc==0 || argv==NULL)       //se nada for passado por linha do comando retorna zero
        return 0;

    if(strcmp(argv[0],"exit")==0)       //verifica se o comando "exit" foi feito
    {
        if(argc != 1)       //se o exit foi passado junto a outro argumento = ERRO
            print_error(); 
        else 
            exit(0);        //executa o exit
        return 0;
    }
    if(strcmp(argv[0],"cd")==0)         //verifica se o comando "cd" foi passado
    { 
        builtin_cd(argc,argv);      //chama a função builtin_cd, que muda o diretorio do shell
        return 0; 
    }
    if(strcmp(argv[0],"path")==0)       //verifica se o comando "path" foi passado
    { 
        builtin_path(argc,argv);     //chama a função builtin_path, para definir a lista de pastas que o executavel esta
        return 0; 
    }

    //SEGUNDO caso não seja passado um comando interno, será executado um comando que criara um processo filho com fork()

    for(int i=0;i<path_count;i++)       //percorre os diretórios definidos em path
    {
        if(!paths[i]) 
            continue;
        char full[1024];        //full = caminho
        snprintf(full,sizeof(full),"%s/%s",paths[i],argv[0]);         //monta o caminho completo do path, exemplo: "/bin/ls"
        if(access(full,X_OK)==0)        //a syscall "access verifica" a permissão de do arquivo, X_OK é necessario para verificar a permissão
        {
            pid_t pid=fork();       //cria um processo filho
            if(pid<0)       //se o filho não foi criado = erro
            { 
                print_error();      //printa o erro
                return 1; 
            }
            if(pid==0)          //PROCESSO FILHO !!! ( pid = 0 )
            {
                if(out_file)        //verifica se existe arquivo de saida "ls > out.txt"
                {
                    int fd = open(out_file, O_CREAT|O_WRONLY|O_TRUNC, 0644);        //abre um arquivo e retorna um file descriptor
                    if(fd<0)        //verifica se foi criado
                    { 
                        print_error(); 
                        exit(1);
                    }
                    if(dup2(fd, STDOUT_FILENO)<0 || dup2(fd, STDERR_FILENO)<0)          //redireciona o stdout e o stderr para o arquivo
                    { 
                        print_error();          //se falhar printa o erro
                        exit(1);
                    }
                    close(fd);          //fecha o descritor original, ou seja não printa a informação na tela
                }
                execv(full, argv);      //a syscall call execv substitui o processo filho pelo programa a ser executado
                print_error();        //se o execv não funcionar, então o processo filho continua e printa o erro
                exit(1);
            } else {        //PROCESSO PAI !!!
                if(background)      //se o comando possui um "&", então o shell não espera
                {
                    bg_pids[(*bg_count)++] = pid;       //guarda o pid
                } else {        //caso não possua "&"
                    waitpid(pid,NULL,0);        //processo pai espera o processo filho
                }
                return 0;       //execução bem sucedida
            }
        }
    }
    print_error();
    return 1;          //execução falha
}

void reap_background_children() {
    int status;
    while(waitpid(-1,&status,WNOHANG)>0);       //limpa processos filhos "desativados"
}

int main(int argc_main, char* argv_main[]) {
    FILE* input = stdin;        //entrada no teclado
    char line[MAX_LINE];
    path_count=0;
    paths[path_count++] = strdup("/bin");
    paths[path_count++] = strdup(".");      // diretório atual para testes OSTEP
    
    if(!paths[0] || !paths[1]) {
        print_error();
        exit(1); 
    }

    if(argc_main>2){        //codigo aceita no maximo 2 comandos
         print_error(); 
         exit(1);
    }

    else if(argc_main==2){      //se um arquivo for passado, lê seus comandos
        input = fopen(argv_main[1], "r");       //input recebe arquivo passado
        if(!input){
            print_error();
            exit(1);
        }
    }

    while(1){       //loop principal do shell
        reap_background_children();     //chama a função trim para remover os espaços
        if(input==stdin){
             printf("wish> ");      //printa o prompt
             fflush(stdout);}
        if(fgets(line,sizeof(line),input)==NULL)        //le a linha
            break;

        size_t len = strlen(line);
        if(len>0 && line[len-1]=='\n')      //verifica se a varivel esta vazia ou se possui "\n" no final
            line[len-1]=0;

        pid_t bg_pids[MAX_ARGS];
        int bg_count = 0;

        char* saveptr;
        char* segment = strtok_r(line,"&",&saveptr);        //usa & para dividir parametros
        while(segment){
            char* cmd = trim(segment);
            int background = 1;         //define o valor do background
            if(!cmd || strlen(cmd)==0)
            {
                segment = strtok_r(NULL,"&",&saveptr);
                continue;
            }

            char* out_file = NULL;
            char* redir = strchr(cmd,'>');      //define redir apontando para > dentro da string
            if(redir){      //verifica se existe ">"
                *redir = 0;         //substitui > por \0
                redir++;        //redir aponta para a string
                char* fname = trim(redir);      //chama a função trim para remover os espaços
                if(!fname || strlen(fname)==0 || strchr(fname,' '))     //verifica se existe algum erro
                {
                    print_error();      //printa o erro
                    segment = strtok_r(NULL,"&",&saveptr);
                    continue; 
                }
                out_file = strdup(fname);       //salva o nome do arquivo
                cmd = trim(cmd);            //limpa os espaços
                if(!cmd || strlen(cmd)==0)      //verifica se tem erro
                { 
                    print_error();          //printa o erro
                    free(out_file); 
                    segment = strtok_r(NULL,"&",&saveptr); 
                    continue; 
                }           // se a entrada é ls > out.txt, então cmd = "ls" out_file = "out.txt"
            }

            int argc_cmd = 0;       //cria um contador de argumentos "argc"
            char* cmd_copy = strdup(cmd);       //aloca memoria e copia a string
            if(!cmd_copy)       //verifica erro na memoria
            { 
                print_error();      //printa o erro
                segment = strtok_r(NULL,"&",&saveptr); 
                continue; 
            }
            char** argv_cmd = parse_tokens(cmd_copy, &argc_cmd);        //chama a função parse_tonkens para quebrar os argumentos em partes

            if(argc_cmd>0)              //se algum comando for passado executa
                execute_command(argv_cmd, argc_cmd, out_file, background, bg_pids, &bg_count);      //chama a função para executar

            if(argv_cmd) 
                free(argv_cmd);     //desaloca memoria
            if(cmd_copy) 
                free(cmd_copy);     //desaloca memoria
            if(out_file) 
                free(out_file);     //desaloca memoria

            segment = strtok_r(NULL,"&",&saveptr);
        }

        for(int i=0;i<bg_count;i++) 
            waitpid(bg_pids[i],NULL,0);     //espera todos os processos em background da linha
    }

    for(int i=0;i<path_count;i++)
        free(paths[i]);     //desaloca a memoria dos paths
    if(input!=stdin) 
        fclose(input);      // caso o shell estiver lendo um arquivo passado junto a ele em sua execução, então ele o fecha e encerra
    return 0;
}
