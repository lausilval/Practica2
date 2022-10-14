/*
    Autoras : Laura Silva Loureiro y Paula Vazquez Tella
    Fecha :  16/11/2022
    Ejercicio : Practica 2 -> Haz un programa para calcular la media en doble precisión de las tangentes de las raíces
                            cuadradas de los primeros 70.000.000 números naturales (considerándo que dichos números son
                            ángulos en radianes) de forma que el cálculo se haga con dos procesos hijo trabajando simultaneamente. El primero debe hacer el cálculo sobre los números pares y el segundo sobre los
                            impares. Cada uno de ellos debe mostrar su resultado en la consola y posteriormente copiarlo en
                            un fichero auxiliar compartido. Debe haber una forma de identificar en el propio fichero quien
                            escribe cada resultado. Paralelamente, un tercer hijo debe realizar el cálculo completo y almacenar el resultado en el fichero auxiliar. Cuando acaben los dos primeros hijos, un cuarto
                            hijo debe calcular el resultado en base a ambas contribuciones, leyéndolas del fichero auxiliar,
                            y añadir el resultado en ese mismo fichero. Todos los procesos hijo deben además escribir los
                            tiempos de ejecución de sus cálculos en el mismo fichero compartido. Finalmente, cuando todos
                            acaben, el padre debe calcular y mostrar la diferencia entre los resultados calculados de las dos
                            formas (es decir, los obtenidos por los hijos tercero y cuarto) y mostrar los tiempos de ejecución
                            de cada uno de los cuatro hijos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

int main() {

    // DECLARACION DE VARIABLES RELACIONADAS CON LOS PROCESOS
    int pid1, pid2, pid3, pid4;
    pid_t *p_process_id;    // variable que almacena la id de los procesos padre
    pid_t *process_id1, *process_id2, *process_id3, *process_id4;  // variables que van a almacenar la id de la llamada al sistema
    int status; // almacena la informacion de estado al realizar el waipid, sirve para poder ser evaluado despues

    // reservamos memoria para las variables que almacenaran los id
    process_id1 = (pid_t *) malloc(sizeof(pid_t));
    process_id2 = (pid_t *) malloc(sizeof(pid_t));
    process_id3 = (pid_t *) malloc(sizeof(pid_t));
    process_id4 = (pid_t *) malloc(sizeof(pid_t));
    p_process_id =  (pid_t *) malloc(sizeof(pid_t));

    // DECLARACION DE VARIABLES RELACIONADAS CON LAS OPERACIONES A REALIZAR
    double media=0, tangentes=0;


    // fichero donde guardaremos todos los resultados obtenidos
    FILE * file;
    file = fopen("Practica2.txt", "wt");   /**NO SE SI UNA O LA OTRA WT O RT DIRIA QUE WT **/
    file = fopen("Practica2.txt", "rt");
    // comprobamos que el fichero se abre correctamente
    if( file == NULL)
    {
        printf("ERROR al abrir el fichero\n");
    }
    *p_process_id = getppid();
    fprintf(file, "Aqui el padre con pid %d, voy a crear 3 hijos.\n", *p_process_id);

    //Primer fork
    pid1 = fork();

    if(pid1 == 0)   // El hijo 1 , calculo sobre los nums pares
    {
        *process_id1 = getpid();
        fprintf(file, "Aqui el hijo 1 con pid %d, calculando la media parcial de pares\n", *process_id1);

        for(int i=0; i<70000000;i++){
            if(i%2 == 0){// calcular tangente de raiz de pares
                tangentes += tan(sqrt(i));
            }
        }
        media = (tangentes/35000000);  // dividimos entre la mitad ya que hay la mitad pares y la mitad impares
        printf("La media de las tangentes de las raices cuadradas de los numero pares es: %lf\n", media);
        //fprintf(file, )
    }
    else if ( pid1 > 0)  // PADRE
    {
        pid2 = fork();
        if(pid2 == 0)  // El hijo 2 , calculo sobre los nums impares
        {
            *process_id2 = getpid();
            fprintf(file, "Aqui el hijo 2 con pid %d, calculando la media parcial de impares\n", *process_id2);

            for(int i=0; i<70000000;i++){
                if(i%2 == 1){// calcular tangente de raiz de impares
                    tangentes += tan(sqrt(i));
                }
            }
            media = (tangentes/35000000);  // dividimos entre la mitad ya que hay la mitad pares y la mitad impares
            printf("La media de las tangentes de las raices cuadradas de los numero impares es: %lf\n", media);

        }
        else if( pid2 > 0)  //PADRE
        {
            pid3 = fork();
            if(pid3 == 0)  // El hijo 3, calculo completo y almacenar en un fichero auxiliar
            {
                *process_id3 = getpid();
                fprintf(file, "Aqui el hijo 3 con pid %d, calculando la media parcial de todos los numeros\n", *process_id3);

                for(int i=0; i<70000000;i++)
                {
                    tangentes += tan(sqrt(i));

                }
                media = (tangentes/70000000);  // dividimos entre la mitad ya que hay la mitad pares y la mitad impares
                printf("La media de las tangentes de las raices cuadradas de todos los numeros: %lf\n", media);

            }
            else if ( pid3 > 0) //PADRE
            {
                fprintf(file, "Aqui el padre con pid %d, esperando que mis dos hijos terminen\n", *p_process_id);

                // tenemos que esperar por los otros hijos para poder convocar al 4
                waitpid(pid1, &status, 0);   // Esperamos hijo 1
                waitpid(pid2, &status, 0);   // Esperamos hijo 2
                //waitpid(pid3, &status, 0);   // Esperamos hijo 3

                fprintf(file, "El hijo 1 con pid %d ha terminado\n", *process_id1);
                fprintf(file, "El hijo 2 con pid %d ha terminado\n", *process_id2);

                pid4 = fork();
                if(pid4 == 0)  // El hijo 4, debe calcular el resultado juntando el de los hijos 1 y 2
                {
                    *process_id4 = getpid();

                    //   debe cogerlos del fichero
                    //fscanf();


                }
                else if (pid4 > 0)   // PADRE
                {
                    // debe esperar por os hijos 3 y 4
                    waitpid(pid3, &status, 0);
                    waitpid(pid4, &status, 0);

                }
                else
                {
                    printf("Fallo al realizar el fork() [pid4]\n");
                }
            }
            else
            {
                printf("Fallo al realizar el fork() [pid3]\n");
            }
        }
        else
        {
            printf("Fallo al realizar el fork() [pid2]\n");
        }
    }
    else
    {
        printf("Fallo al realizar el fork() [pid1] \n");
    }


    // cerramos el fichero y liberamos memoria
    fclose(file);
    free(process_id1);
    free(process_id2);
    free(process_id3);
    free(process_id4);
    free(p_process_id);

    return EXIT_SUCCESS;
}
