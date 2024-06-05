#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>


///Prototipos de las funciones
void ord_ins(int v[], int n);
void ord_shell(int v[], int n);
void inicializar_semilla();
void aleatorio(int v[], int n);
void aleatorioAscendente(int v[], int n);
void aleatorioDescendente(int v[], int n);
void imprimirVector(int v[], int n);
int esOrdenado(int v[], int n);
double microsegundos();
void test(void (*ordenacion)(int v[], int n), void (*generador)(int v[], int n), int n);
void calculosTiempos(int n, void (*ordenacion)(int v[], int n), void (*generador)(int v[], int n), double time);
void printCabecera(void (*ordenacion)(int v[], int n), void (*generador)(int v[], int n));
void analizarTiempos(void (*ordenacion)(int v[], int n), void (*generador)(int v[], int n),int num_sizes);


void ord_ins(int v[], int n){
    int x;
    int j;
    int i;

    for ( i = 0; i < n; ++i) {
        x = v[i];
        j = i - 1;

        while(j >= 0 && v[j] > x){
            v[j + 1] = v[j];
            j = j - 1;
        }
        v[j + 1] = x;
    }
}

void ord_shell(int v[], int n){
    int incr = n;
    int i, j, temp;
    bool seguir;

    do{
    incr = incr/2;
        for (i = incr; i < n ; ++i) {
            temp = v[i];
            j = i;
            seguir = true;

            while((j - incr >= 0) && seguir){
                if(temp < v[j - incr]){
                    v[j] = v[j - incr];
                    j = j - incr;

                }else{
                    seguir = false;
                }
            }
            v[j] = temp;
        }

    }while(incr != 1);
}

void inicializar_semilla() {
    srand(time(NULL));
}

void aleatorio(int v [], int n) {
    int i, m = 2*n + 1;
    for (i = 0; i < n; i++)
        v[i] = (rand() % m) - n;
}

void aleatorioAscendente(int v[], int n) {
    int i;
    for (i = 0; i < n; i++)
        v[i] = i;
}

void aleatorioDescendente(int v[], int n) {
    int i;
    for (i = 0; i < n; i++)
        v[i] = n - i - 1;
}

void imprimirVector(int v[], int n) {
    printf("[");
    int i;
    for (i = 0; i < n; ++i) {
        printf("%d ", v[i]);
    }
    printf("]  \t");
}

int esOrdenado(int v[], int n){
    if (n <= 1) {
        return 1;
    }

    for (int i = 1; i < n; i++) {
        if (v[i] < v[i - 1]) {
            return 0;
        }
    }

    return 1;
}

double microsegundos(){
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void analizarTiempos(void (*ordenacion)(int v[], int n), void (*generador)(int v[], int n), int n) {
    inicializar_semilla();
    int k;
    double elapsed_time1, start_time, end_time, t1, t2;
    int *v;

    v = malloc(n * sizeof (int));

    generador(v, n);

    start_time = microsegundos();
    ordenacion(v, n);
    end_time = microsegundos();
    elapsed_time1 = end_time - start_time;

    if (elapsed_time1 < 500) {
        printf("(*)");
        start_time = microsegundos();

        for (k = 0; k < 100; k++) {
            generador(v, n);
            ordenacion(v, n);
        }
        end_time = microsegundos();
        t1 = end_time - start_time;
        start_time = microsegundos();

        for (k = 0; k < 100; k++) {
            generador(v, n);
        }
        end_time = microsegundos();
        t2 = end_time - start_time;

        elapsed_time1 = (t1 - t2) / 100;
        calculosTiempos(n, ordenacion, generador, elapsed_time1);

    } else {
        calculosTiempos(n, ordenacion, generador, elapsed_time1);
    }
    free(v);
}

void Shell(){
    int i;

    printf("Ordenacion Shell Aleatorio\n");
    printCabecera(ord_shell, aleatorio);
    for(i = 500; i <= 128000; i = i * 2){
        analizarTiempos(ord_shell, aleatorio, i);
    }
    printf("\n\n");

    printf("Ordenacion Shell Ascendente\n");
    printCabecera(ord_shell, aleatorioAscendente);
    for(i = 500; i <= 128000; i = i * 2){
        analizarTiempos(ord_shell, aleatorioAscendente, i);
    }
    printf("\n\n");

    printf("Ordenacion Shell Descendente\n");
    printCabecera(ord_shell, aleatorioDescendente);
    for(i = 500; i <= 128000; i = i * 2){
        analizarTiempos(ord_shell, aleatorioDescendente, i);
    }
    printf("\n\n");


}

void Ins(){
    int i;

    printf("Ordenacion Insercion Aleatorio\n");
    printCabecera(ord_ins, aleatorio);
    for(i = 500; i <= 64000; i = i * 2){
        analizarTiempos(ord_ins, aleatorio, i);
    }
    printf("\n\n");

    printf("Ordenacion Insercion Ascendente\n");
    printCabecera(ord_ins, aleatorioAscendente);
    for(i = 500; i <= 64000; i = i * 2){
        analizarTiempos(ord_ins, aleatorioAscendente, i);
    }
    printf("\n\n");

    printf("Ordenacion Insercion Descendente\n");
    printCabecera(ord_ins, aleatorioDescendente);
    for(i = 500; i <= 64000; i = i * 2){
        analizarTiempos(ord_ins, aleatorioDescendente, i);
    }
    printf("\n\n");


}

void calculosTiempos(int n, void (*ordenacion)(int v[], int n), void (*generador)(int v[], int n), double time){
    double time1, time2, time3;

    if(ordenacion == ord_ins){
        if(generador == aleatorioAscendente) {
            time1 = time / pow(n, 0.8);
            time2 = time / pow(n, 1);
            time3 = time / pow(n, 1.2);

        }else{
            time1 = time / pow(n, 1.8);
            time2 = time / pow(n, 2);
            time3 = time / pow(n, 2.2);
        }

    }else{

        if(generador == aleatorio) {
            time1 = time / (pow(n, 1));
            time2 = time / (pow(n, 1.195));
            time3 = time / (pow(n, 1.33));
        }
        else if(generador == aleatorioAscendente){
            time1 = time / (pow(n, 0.85));
            time2 = time / (pow(n, 1.02) * log2(n));
            time3 = time / (pow(n, 1.33));

        }else{
            time1 = time / (pow(n, 1));
            time2 = time / (pow(n, 1.13));
            time3 = time / (pow(n, 1.33));

        }
    }
    printf("\t% 8d\t\t% 15.3lf\t\t% 14.8lf\t\t% 14.8lf\t\t% 14.8lf\n", n, time, time1, time2, time3);

}

void printCabecera(void (*ordenacion)(int v[], int n), void (*generador)(int v[], int n)){
    if(ordenacion == ord_ins){
        if(generador == aleatorioAscendente) {
            printf("\t       n\t\t\t  t(n)\t\t   t(n) / n^0.8\t\t   t(n) / n^1\t           t(n) / n^1.2\n");
        }else{
            printf("\t       n\t\t\t  t(n)\t\t   t(n) / n^1.8\t\t   t(n) / n^2\t           t(n) / n^2.2\n");
        }

    }else{
        if(generador == aleatorio) {
            printf("\t       n\t\t\t  t(n)\t\t   t(n) / n^1\t\t   t(n) / n^1.195\t   t(n) / n^1.33\n");
        }
        else if(generador == aleatorioAscendente){
              printf("\t       n\t\t\t  t(n)\t\t t(n) / n^0.85\t     t(n) / n^1.02*log2(n)\t    t(n) / n^1.33\n");
        }else{
            printf("\t       n\t\t\t  t(n)\t\t   t(n) / n^1\t\t   t(n) / n^1.13\t    t(n) / n^1.33\n");
        }
    }
}

void test(void (*ordenacion)(int v[], int n), void (*generador)(int v[], int n), int n) {
    int v[n];

    generador(v, n);
    imprimirVector(v, n);

    printf("\n");

    if (esOrdenado(v, n)) {
        printf("ordenado? 1\n");
    } else {
        printf("ordenado? 0\n");
    }

    ordenacion(v, n);

    printf("Ordenacion\n");
    imprimirVector(v, n);
    printf("\n");

    if (esOrdenado(v, n)) {
        printf("ordenado? 1\n");
    } else {
        printf("ordenado? 0\n");
    }
}



int main() {
    inicializar_semilla();

    ///test
    printf("Inicializacion aleatoria shell\n");
    test(&ord_shell, &aleatorio, 100);
    printf("\n\n");
    printf("Inicializacion descendente shell\n");
    test(&ord_shell, &aleatorioDescendente, 100);
    printf("\n\n");
    printf("Inicializacion ascendente shell\n");
    test(&ord_shell, &aleatorioAscendente, 100);

    printf("\n\n");

    printf("Inicializacion aleatoria ins\n");
    test(&ord_ins, &aleatorio, 100);
    printf("\n\n");
    printf("Inicializacion descendente ins\n");
    test(&ord_ins, &aleatorioDescendente, 100);
    printf("\n\n");
    printf("Inicializacion ascendente ins\n");
    test(&ord_ins, &aleatorioAscendente, 100);

    printf("\n\n\n");


    ///Análisis de algoritmos ord_shell y ord_ins
    Ins();
    Shell();


    return 0;
}

