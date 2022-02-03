#include <stdio.h>
#include <stdlib.h>
#include "mergeAlumnos.h"

int main()
{

    crearLotePruebaAlu();
    crearLotePruebaNovedades();

    puts("\nArchivo Maestro:\n");
    mostrarArchivoAlu("maestroAlumnos.dat");
    puts("\nArchivo Novedades:\n");
    mostrarArchivoNov("novedades.dat");

    if(actualizarMaestro("maestroAlumnos.dat","novedades.dat","maestroActualizado.dat","errores.dat")!=1)
    {
        printf("No se pudo actualizar el archivo maestro\n");
        return 1;
    }

    puts("\nArchivo Actualizado Maestro:\n");
    mostrarArchivoAlu("maestroActualizado.dat");

    puts("\nArchivo de Errores:\n");
    mostrarArchivoErrores("errores.dat");

    return 0;
}
