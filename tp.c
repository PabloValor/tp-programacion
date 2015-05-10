#include <stdio.h>
#include <conio.h>

#define CANT_NOTAS 3
#define CANT_MAX_ALUMNOS 50
#define MIN_EDAD 18
#define MAX_EDAD 99
#define MIN_NOTA 0.0
#define MAX_NOTA 10.0
#define ERROR 0
#define OK 1


typedef struct {
    int     id;  // valor autoincrementado
    char    nombre[20],
            apellido[20];
    long    dni;
    int     edad;
    float   notas[CANT_NOTAS];
    int    eliminado;
} t_alumno;

void mostrarMenu();

/** Funciones Principales**/
int cargarAlumnoEnArray(t_alumno*, const int, const int);
int eliminarAlumno(t_alumno *);
void mostrarAlumnos(const t_alumno*, int);
int modificarAlumno(t_alumno*);
int generarArchivoBinario(const t_alumno*);
int generarArchivoTextoDesdeArray(const t_alumno*, const int);
int generarArchivoTextoDesdeBinario();

/** Funciones auxiliares **/
t_alumno cargarRegistro();
int esRegistroValido(t_alumno);
int esNotaValida(const float*);
int esEdadValida(const int);
int buscarEnArrayPorId(const t_alumno*, int);
void sobreescribirAlumnos(const t_alumno, t_alumno*);
float promedioNotas(const float*);

int main() {
    t_alumno alumnos[CANT_MAX_ALUMNOS];
    int cantAlumnosCargados = 0;    // flag necesario para saber cuantos alumnos hay cargados
    int hayAlumnoCargado = 0;      // flag "booleano" para saber si al menos hay un alumno en el array, lo uso en mostrarAlumnos (mejorable)
    int realizarAccion;             //Accion a realizar elegida por el usuario
    int id = 0;               //Id autoincrementado para cada alumno cargado con exito

    do {
        system("cls");
        fflush(stdin);

        mostrarMenu();
        printf("\n Su opcion es:\t");
        scanf("%d", &realizarAccion);

        switch(realizarAccion) {
            case 1:
                    if(cargarAlumnoEnArray(alumnos, cantAlumnosCargados, id)) {
                        cantAlumnosCargados ++;
                        hayAlumnoCargado = 1;
                        id ++;
                        printf("\n Alumno cargado con exito a la memoria.");
                    } else {
                        printf("\n ERROR: no se pudo cargar alumno en memoria.");
                    }
                    break;
            case 2:
                    if(hayAlumnoCargado && modificarAlumno(alumnos)) {
                        printf("\nAlumno modificado de la memoria con exito");
                    } else {
                        printf("\nERROR: no se pudo modificar alumno.");
                    }
                    break;
            case 3:
                    if(hayAlumnoCargado && eliminarAlumno(alumnos)) {
                        printf("\nAlumno eliminado con exito.");
                    } else {
                        printf("\nERROR: no se pudo eliminar alumno.");
                    }
                    break;
            case 4:
                    if(hayAlumnoCargado) {
                        mostrarAlumnos(alumnos, cantAlumnosCargados);
                    } else {
                        printf("\nNo hay alumnos cargados para mostrar.");
                    }
                    break;
            case 5:
                    if(hayAlumnoCargado && generarArchivoBinario(alumnos)) {
                        printf("\nArchivo generado");
                    } else {
                        printf("\nERROR: no se pudo generar el archivo binario.");
                    }
                    break;
            case 6:
                    if(hayAlumnoCargado && generarArchivoTextoDesdeArray(alumnos, cantAlumnosCargados)) {
                        printf("\nArchivo 'reporte.txt' genererado con exito.");
                    } else {
                        printf("\nERROR: no se pudo generar el reporte.");
                    }
                    break;
            case 7:
                    if(generarArchivoTextoDesdeBinario()) {
                        printf("\nArchivo 'reporte.txt' genererado con exito.");
                    } else {
                        printf("\nERROR: no se pudo generar el reporte.");
                    }
                    break;
            case 0:
                    printf("\n Programa finalizado correctamente.\n");
                    return OK;
            default:
                    printf("\n ATENCION: Por favor, ingrese el numero correspondiente a la accion.");
                    break;
        }
        printf("\nPresione una tecla para continuar..");
        _getch();
    } while(realizarAccion);

    return OK;
}

/**
* Funcion: mostrarMenu
* Resumen: imprime por pantalla el menu principal del programa
* Parametro: -
* retorna: -
**/
void mostrarMenu() {
    printf("\t\t*** MENU - ALTA BAJA MODIFICACION DE ALUMNOS - ***");
    printf("\n\n Opciones:");
    printf("\n\n 1 - Cargar Alumno");
    printf("\n 2 - Modificar Alumno");
    printf("\n 3 - Eliminar Alumno");
    printf("\n 4 - Mostrar Alumnos");
    printf("\n 5 - Guardar Alumnos en disco");
    printf("\n 6 - Generar reporte de alumnos aprobados (desde memoria)");
    printf("\n 7 - Generar reporte de alumnos aprobados (desde archivo binario)");
    printf("\n 0 - Salir del programa\n");
}

/**
* Funcion: cargarAlumnoEnArray
* Resumen: carga un t_alumno en un array
* Parametro 1: puntero de tipo t_alumno (apuntaria a un array)
* Parametro 2: puntero a entero indicando la cantidad de registros guardados
* retorna: 0 si hay error. 1 si no hay error
**/
int cargarAlumnoEnArray(t_alumno *alumnos, const int cantAlumnosCargados, const int id) {
    t_alumno alumno = cargarRegistro();
    int i;

    if(cantAlumnosCargados < CANT_MAX_ALUMNOS && esRegistroValido(alumno)) {

        //realizo una asignacion de estructuras t_alumno
        alumnos[cantAlumnosCargados] = alumno;

        // asigno el id y lo incremento en la funcion invocadora
        alumnos[cantAlumnosCargados].id = id;

        alumnos[cantAlumnosCargados].eliminado = 1;  // flag de NO eliminado

        return OK;
    } else {
        return ERROR;
    }
}

/**
* Funcion: cargarRegistro
* Resumen: Funcion auxiliar para cargar un registro no sanitizado
* Parametros: -
* Retorna: un t_alumno simple
**/
t_alumno cargarRegistro() {
    t_alumno registro;
    int i;

    printf("\n Ingrese nombre de alumno:\t");
    fflush(stdin);
    scanf("%s", &registro.nombre);

    printf("\n Ingrese apellido de alumno:\t");
    fflush(stdin);
    scanf("%s", &registro.apellido);

    printf("\n Ingrese dni de alumno:\t");
    fflush(stdin);
    scanf("%ld", &registro.dni);

    printf("\n Ingrese edad de alumno:\t");
    fflush(stdin);
    scanf("%d", &registro.edad);

    for(i=0 ; i < CANT_NOTAS ; i++) {
        printf("\n Ingrese la nota %d:\t", i+1);
        fflush(stdin);
        scanf("%f", &registro.notas[i]);
    }
    return registro;
}

/**
* Funcion: modificarAlumno
* Resumen: en base a un id valido en un array de t_alumno, modifica un t_alumno
* Parametro 1: puntero a t_alumno (un array)
* Retorna: 1 si hay exito. 0 si hay error
**/
int modificarAlumno(t_alumno *alumnos) {
    int auxId, posicion;
    t_alumno registroNuevo;
    int j;

    system("cls");
    printf("\nIngrese el ID del alumno que quiere modificar:\t");
    scanf("%d", &auxId);

    posicion = buscarEnArrayPorId(alumnos, auxId);

    if( posicion == -1) {

        printf("\nEl id que ingreso no es valido");
        return ERROR;
    } else {
            printf("\n EL REGISTRO QUE VA A MODIFICAR ES EL SIGUIENTE: ");
            printf("\nID:\t\t%d", alumnos[posicion].id);
            printf("\nNOMBRE:\t\t%s", alumnos[posicion].nombre);
            printf("\nAPELLIDO:\t\t%s", alumnos[posicion].apellido);
            printf("\nDNI:\t\t%ld", alumnos[posicion].dni);
            printf("\nEDAD:\t\t%d", alumnos[posicion].edad);

            for( j = 0 ; j < CANT_NOTAS ; j++ ) {

                printf("\nNOTA %d:\t\t%.2f", j+1, alumnos->notas[j]);
            }
            printf("\n");

            printf("\n\nINGRESE LOS NUEVOS DATOS:");
            registroNuevo = cargarRegistro();

            if(esRegistroValido(registroNuevo)) {
                sobreescribirAlumnos(registroNuevo,&alumnos[posicion]);
            }
            else {
                return ERROR;
            }
    }
    return OK;
}

/**
* Funcion: mostrarAlumnos
* Resumen: muestra (con formato apropiado) un array de t_alumno
* Parametro 1: puntero constante a t_alumno (un array)
* Parametro 2: entero con la cantidad de  guardados en el array de t_alumno
* Retorna: -
**/
void mostrarAlumnos (const t_alumno *alumnos, int alumnosGuardados) {
    int i, j;
    system("cls");

    printf("\n**ALUMNOS CARGADOS**\n");

    for(i = 0 ; i < alumnosGuardados ; i++) {

        if(alumnos->eliminado) {    // si el alumno esta eliminado, no se muestra

            printf("\n [DATOS DE ALUMNO %d]", i+1);
            printf("\n\n ID: %d", alumnos->id);
            printf("\n NOMBRE: %s", alumnos->nombre);
            printf("\n APELLIDO: %s", alumnos->apellido);
            printf("\n DNI: %ld", alumnos->dni);
            printf("\n EDAD: %d", alumnos->edad);

            for( j = 0 ; j < CANT_NOTAS ; j++ ) {

                printf("\n NOTA %d: %.1f", j+1, alumnos->notas[j]);
            }
            printf("\n");
        }
        alumnos++;
    }
}

/**
* Funcion: eliminarAlumno
* Resumen: Elimina un alumno de un array de t_alumno (cambia el estado del flag 'eliminado')
* Parametro : puntero a t_alumno (un array)
* Retorna: 1 si la operacion de realizo con exito. 0 si hay error
**/
int eliminarAlumno(t_alumno *alumnos) {
    int id;

    printf("\nIngrese el id asignado al alumno que desea eliminar:\t");
    fflush(stdin);
    scanf("%d", &id);

    if(buscarEnArrayPorId(alumnos, id) != -1) {
        alumnos[id].eliminado = 0; // marco el alumno como eliminado, esto implica que no se volvera a mostrar cuando se imprima el array
        return OK;
    } else {
        printf("\nERROR: no se encontro el id.");
        return ERROR;
    }
}

/**
* Funcion: generarArchivoBinario
* Resumen: Genera un archivo binario en el root del proyecto con el nombre alumnos.bin con los datos de un array de t_alumno
* Parametro: puntero constante a array de t_alumno
* Retorna: 1 si el archivo se genera con exito. 0 si hay error
**/
int generarArchivoBinario(const t_alumno *alumnos) {
    FILE* pf;
    int cantRegistrosGuardados;

    pf = fopen("alumnos.bin", "wb");

    if(!pf) return ERROR;

    cantRegistrosGuardados = fwrite(alumnos, sizeof(t_alumno), CANT_MAX_ALUMNOS, pf);
    fclose(pf);

    if(cantRegistrosGuardados > 0) {
        printf("\nRegistros guardados.");
        return OK;
    } else {
        printf("\nERROR: No se pudo guardar los registros en el archivo.");
        return ERROR;
    }
}

/**
* Funcion: generarArchivoTextoDesdeArray
* Resumen: Genera un archivo de texto en el root del proyecto con el nombre reporte.txt con los datos de un array de t_alumno
* Parametro: puntero constante a array de t_alumno
* Retorna: 1 si el archivo se genera con exito. 0 si hay error
**/
int generarArchivoTextoDesdeArray(const t_alumno *alumnos, const int cantAlumnosCargados) {
    FILE* pf;
    int i = 0;
    float promedio;

    pf = fopen("reporte.txt", "wt");

    if(!pf) return ERROR;

    fprintf(pf, "\t\t** REPORTE DE ALUMNOS APROBADOS **\n");
    while(i < cantAlumnosCargados) {

        promedio = promedioNotas((alumnos)->notas);

        if(alumnos->eliminado && promedio >= 7) {

            fprintf(pf, "\n[Id: %d]  Nombre: %s  Apellido: %s  DNI: %ld  Edad: %d  Promedio: %.1f",
                alumnos->id, alumnos->nombre, alumnos->apellido, alumnos->dni, alumnos->edad, promedio);
        }
        i++;
        alumnos++;
    }
    fclose(pf);
    return OK;
}

/**
* Funcion: generarArchivoTextoDesdeBinario
* Resumen: Genera un archivo de texto de alumnos aprobados leyendo un archivo binario en el root del proyecto.
* Parametro: -
* Retorna: 1 si el archivo se genera con exito. 0 si hay error
**/
int generarArchivoTextoDesdeBinario() {

    FILE *pBinario, *pTexto;
    t_alumno alumno;
    float promedio;

    pBinario = fopen("alumnos.bin", "rb");
    pTexto = fopen("reporte.txt", "wt");

    if(!pTexto) {
        printf("\nError: no se pudo crear el archivo de texto.");
        return ERROR;
    }

    if(!pBinario) {
        printf("\nError: no se pudo abrir el archivo binario.");
        return ERROR;
    }

    fprintf(pTexto, "\t\t** REPORTE DE ALUMNOS APROBADOS **\n");

    while(!feof(pBinario)) {

        fread(&alumno, sizeof(t_alumno), 1, pBinario);

        promedio = promedioNotas(alumno.notas);

        if(alumno.eliminado && promedio >= 7) {

            fprintf(pTexto, "\n[Id: %d]  Nombre: %s  Apellido: %s  DNI: %ld  Edad: %d  Promedio: %.1f",
                alumno.id, alumno.nombre, alumno.apellido, alumno.dni, alumno.edad, promedio);
        }
    }

    fclose(pBinario);
    fclose(pTexto);
    return OK;
}

/**
* Funcion: promedioNotas
* Resumen: funcion auxiliar que calcula un promedio
* Parametro: puntero constante a array de float
* Retorna: promedio flotante del array recibido por parametro
**/
float promedioNotas (const float *notas) {
    int i,
        sumador = 0;

    for (i = 0 ; i < CANT_NOTAS ; i++) {
        sumador += *notas;
        notas++;
    }
    return sumador / CANT_NOTAS;
}

/**
* Funcion: buscarEnArrayPorId
* Resumen: funcion auxiliar para buscar un id en un array de t_alumno (la posicion coincide con el id )
* Parametro: puntero a array de t_alumno, int con el id a buscar
* Retorna: 0 si no existe. 1 si existe
**/
int buscarEnArrayPorId(const t_alumno *alumnos, int idABuscar) {
    int i;

    for( i = 0 ; i < CANT_MAX_ALUMNOS ; i++) {

        if(alumnos->id == idABuscar) {
            return idABuscar;
        }
        else {
            alumnos++;
        }
    }
    return -1; // el Id no existe
}

/**
* Funcion: sobreescribirAlumnos
* Resumen: funcion auxiliar para sobreescribir un elemento de un array de t_alumno con un dato de tipo t_alumno
* Parametro 1: t_alumno constante (registro nuevo)
* Parametro 2: t_alumno (registro a modificar)
* Retorna: -
**/
void sobreescribirAlumnos(const t_alumno nuevo, t_alumno* viejo) {
    *viejo = nuevo;
}

/**
* Funcion: esRegistroValido
* Resumen: funcion auxiliar para validar un t_alumno
* Parametro: t_alumno cargado por el usuario
* Retorna: 0 si hay error. 1 si los datos son validos
**/
int esRegistroValido(t_alumno alumno) {

    return esArrayDeCaracteres(&alumno.nombre)
            && esArrayDeCaracteres(&alumno.apellido)
            && esEdadValida(alumno.edad)
            && esArrayDeFlotantes(&alumno.notas);
}

/**
* Funcion: esArrayDeCaracteres
* Resumen: funcion auxiliar para validar si un array es de solo caracteres a-z o A-Z
* Parametro: puntero constante a array de char
* Retorna: 0 si hay error. 1 si los datos son validos
**/
int esArrayDeCaracteres(const char* array) {
    char *aux = array;

    while(*array) {
        if((*array) >= 'a' && (*array) <= 'z' || (*array) >= 'A' && (*array) <= 'Z') {
            array++;
        }
        else {
            printf("\nERROR: Se ingreso un dato no valido: %s.", aux);
            return ERROR;
        }
    }
    return OK;
}

/**
* Funcion: esArrayDeFlotantes
* Resumen: funcion auxiliar para validar si un array es de solo flotantes
* Parametro: puntero constante a array de float
* Retorna: 0 si hay error. 1 si los datos son validos
**/
int esArrayDeFlotantes(float *notas) {
    int i = 0;

    while(i < CANT_NOTAS) {
        if(esNotaValida(notas)) {
            notas++;
            i++;
        } else {
            printf("\n NO es array de flotantes.");
            return ERROR;
        }
    }
    return OK;
}

/**
* Funcion: esEdadValida
* Resumen: funcion auxiliar para validar si una edad esta entre un determinado rango
* Parametro: entero constante que representa la edad a evaluar
* Retorna: 0 si hay error. 1 si los datos son validos
**/
int esEdadValida(const int n) {
    if(n >= MIN_EDAD && n <= MAX_EDAD) {
        return OK;
    } else {
        printf("\nERROR: La edad no es valida. Debe ingresar un valor entre %d y %d.", MIN_EDAD, MAX_EDAD);
        return ERROR;
    }
}

/**
* Funcion: esNotaValida
* Resumen: funcion auxiliar para validar si una nota esta entre 0.0 y 10.0
* Parametro: float constante que representa la nota a evaluar
* Retorna: 0 si hay error. 1 si los datos son validos
**/
int esNotaValida(const float *n) {
    if((*n) >= MIN_NOTA && (*n) <= MAX_NOTA) {
        return OK;
    } else {
        printf("\nERROR: la nota no es valida. Debe ingresar un valor entre %.1f y %.1f.", MIN_NOTA, MAX_NOTA);
        return ERROR;
    }
}
