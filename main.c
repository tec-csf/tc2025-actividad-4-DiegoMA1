//
//  Actividad 4
//  Diego Moreno Acevedo A01022113
//
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct{
    pid_t id;
    pid_t promedio;
}promedios;

void nAsteriscos(int promedio, int greater){
    float length = (promedio * 10) /greater;
    for (int i = 0; i < length;++i){
        printf("*");
    }
    printf("\n");
}

int main(int argc, char * argv[]) {
    char *pvalue = NULL;
    int index;
    int p;
    
    while ((p = getopt (argc, argv, "n:")) != -1)
        switch (p)
        {
        case 'n':
            pvalue = optarg;
            break;
        case '?':
            if (optopt == 'n')
                fprintf (stderr, "Opción -%c requiere un argumento.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Opción desconocida '-%c'.\n", optopt);
            else
                fprintf (stderr,
                         "Opción desconocida '\\x%x'.\n",
                         optopt);
            return 1;
        default:
            abort ();
    }
    //printf ("pvalue = %s\n", pvalue);
    for (index = optind; index < argc; index++)
        printf ("El argumento no es una opción válida %s\n", argv[index]);
    //Convirtiendo a entero argumento recibido en linea de comando atoi()
    int nprocesos = atoi(pvalue);
    //printf("nprocesos %d",nprocesos);
    if(nprocesos>=1){
        pid_t pid;
        pid_t ppid = getpid();
        promedios *promprocesos = (promedios *)malloc(nprocesos * sizeof(promedios));
        int i = 0;
        promedios *current = promprocesos;

        while (i < nprocesos){
            pid = fork();

            if (pid == 0){
                sleep(1);
                printf("Soy el proceso hijo con PID = %d y mi promedio es = %d\n", getpid(), (getpid() + ppid) / 2);
                exit(0);
            }
            else if (pid == -1){
                current->id = 0;
                printf("Hubo un error al crear proces hijo. Numero de procesos hijos creados hasta ahora: %d\n", i + 1);
                break;
            }
            else{
                current->id = pid;
                current->promedio = (current->id + ppid / 2);
                //printf("CID: %d PPID:%d PROM:%d\n", current->id,ppid,current->promedio);
            }
            i++;
            current++;
        }

        promedios *last = promprocesos + nprocesos;
        
        pid_t greater = 0;

        for (current = promprocesos; current < last; ++current){
            // Poner al padre a esperar que el hijo termine
            waitpid(current->id, NULL, 0);
            if (current->promedio > greater){
                greater = current->promedio;
            }    
        }

        printf("PID Hijo\tPromedio\tHistograma\n");

        for (current = promprocesos; current < last; ++current){
            printf("%d\t\t%d\t\t", current->id, current->promedio);
            nAsteriscos(current->promedio, greater);
        }
        free(promprocesos);
    }
    else{
        printf("Numero invalido para la creación de procesos");
        return 1;
    }
    return 0; 
}
