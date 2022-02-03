#ifndef MERGEALUMNOS_H_INCLUDED
#define MERGEALUMNOS_H_INCLUDED
#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    unsigned dia,
             mes,
            anio;
}t_fecha;

typedef struct
{
    int legajo;
    unsigned dni;
    char apellido[31];
    char nombre[21];
    t_fecha fNac;
    t_fecha fIngr;
    char direccion[21];
    int telefono;
    char email[31];
    bool baja_logica; ///V= baja o F= alta //DEBE SER UN CHAR ocupa un byte

}t_alumnos;

typedef struct
{
    int legajo;
    unsigned dni;
    char apellido[31];
    char nombre[21];
    t_fecha fNac;
    t_fecha fIngr;
    char direccion[21];
    int telefono;
    char email[31];
    //bool baja_logica; ///V= baja o F= alta
    char novedad; ///A B M (alta baja o modificacion)

}t_novedades;

typedef struct
{
    int legajo;
    unsigned dni;
    char apellido[31];
    char nombre[21];
    t_fecha fNac;
    t_fecha fIngr;
    char direccion[21];
    int telefono;
    char email[31];
    bool baja_logica; ///V= baja o F= alta
    char novedad; ///A B M (alta baja o modificacion)

}t_errores;


int crearLotePruebaAlu();
int crearLotePruebaNovedades();

int actualizarMaestro(char *nombreMaestro, char* nombreNov, char*nomMaesAct, char* nomErrores);
int cmpAlumnoClave(const void* maestro, const void* nov);

void copiarDatos(t_errores *error, t_novedades* nov, t_alumnos* alu);
void modificarDatos(t_novedades *nov, t_alumnos* alu);

void mostrarArchivoAlu(char *nombreArch);
void mostrarArchivoNov(char *nombreArch);
void mostrarArchivoErrores(char *nombreArch);

#endif // MERGEALUMNOS_H_INCLUDED
