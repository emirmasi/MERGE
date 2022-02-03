#include "mergeAlumnos.h"
#include <string.h>

int crearLotePruebaAlu()
{
    FILE *pf=fopen("maestroAlumnos.dat","wb");
    t_alumnos vecDatos[]=
    {
        {1,11111111,"Ayala","Abril",{12,3,1999},{20,04,2010},"Peru 111", 46450985,"notreply@mail.com",true}, //bien
        {11,22222222,"Bermudez","Bastian",{11,3,1994},{2,2,2014},"Pasteur 648", 44436363,"mail@mail.com",false}, //bien Modif
        {22,33333331,"Diaz","Camila",{10,4,1999},{20,4,2017},"Almafuerte 222",46503344,"mail@mail.com",true}, //BAJA LOG y no en novedades
        {23,33333333,"Fernandez","Cintia",{01,1,1995},{21,4,2014},"Miro 1779",0000000,"",false}, ///mal alta-alta
        {44,44444444,"Galguera","Gennaro",{28,5,1995},{20,4,2015},"Las Bases 123",46450984,"",false},///pasa normal
        {55,55555555,"Jumanji","Jessica",{30,10,1993},{9,4,2019},"",00000000,"jesi@hotmail.com",true},
        {77,77777778,"Opra","Ozuna",{1,1,2000},{1,7,2019},"Pringles 4321",7654134,"",false},
        {88,88888888,"Rodriguez","Roma",{13,11,1993},{5,5,2020},"Florio 2233",43445656,"roma@gmail.com",false}
    };
    if(!pf)
        return 0;

    fwrite(vecDatos,sizeof(vecDatos),1,pf);
    fclose(pf);
    return 1;
}

int crearLotePruebaNovedades()
{
    FILE *pf=fopen("novedades.dat","wb");
    t_novedades vecDatos[]=
    {
        {1,11111111,"Ayala","Abril",{12,3,1999},{20,04,2010},"Peru 111", 46450985,"notreply@gmail.com",'B'}, ///bien, baja log y baja novedades
        {11,22222222,"Bermudez","Bastian",{11,3,1994},{2,2,2014},"Pasteur 648",44436363,"islasbermudas@mail.com",'M'}, ///bien modificacion
        {23,33333333,"Fernandez","Cintia",{01,1,1995},{21,4,2014},"Miro 1779",0000000,"",'A'}, ///error: ya figura de alta
        {41,44444411,"Hilary","Isabella",{11,2,1994},{2,4,2020},"Paris 555",44416060,"",'B'}, ///error: baja a alguien que no está
        {66,66666611,"Lillo","Liliana",{8,2,1980},{21,8,2010},"Carrasco 115",44601234,"acuarioocho@yahoo.com.ar",'A'}, ///bien, dar de alta
        {77,77777777,"Miss","Mia",{9,9,2001},{30,8,2016},"Cuba 123",44449999,"mimia@hotmail.com",'M'}, ///error: modif a quien no está
        {89,99999999,"Silva","Romina",{12,3,2000},{3,3,2013},"Arieta 01",44432234,"romi@outlook.com",'A'},
        {99,10000000,"Silver","Susana",{10,10,1992},{28,4,2016},"Rivadavia 14000",0,"",'M'},
        {999,99999999,"Tassi","Mariel",{13,8,1995},{22,4,2017},"Rosas 125",43210987,"",'A'},
        {111,99999999,"Zarate","Florencia",{20,07,1999},{10,4,2017},"Santamaria 1773",46961615,"florzarate@mail.com",'B'}
    };
    if(!pf)
        return 0;

    fwrite(vecDatos,sizeof(vecDatos),1,pf);
    fclose(pf);
    return 1;
}

int actualizarMaestro(char *nombreMaestro, char* nombreNov, char*nomMaesAct,char*nomErrores)
{
    FILE *fMae,
         *fNov,
         *fAct,
         *pfError;

    t_alumnos alu;
    t_errores aluError;
    t_novedades nov;
    int cmp;

    fMae=fopen(nombreMaestro,"rb");
    if(!fMae)
        return -1;

    fNov=fopen(nombreNov,"rb");
    if(!fMae)
    {
        fclose(fMae);
        return -1;
    }

    fAct=fopen(nomMaesAct,"wb");
    if(!fAct)
    {
        fclose(fMae);
        fclose(fNov);
        return -1;
    }

    pfError=fopen(nomErrores,"wb");
    if(!pfError)
    {
        fclose(fMae);
        fclose(fNov);
        fclose(fAct);
        return -1;
    }

    fread(&alu,sizeof(t_alumnos),1,fMae);
    fread(&nov,sizeof(t_novedades),1,fNov);
    while(!feof(fMae) && !feof(fNov))
    {
        cmp=cmpAlumnoClave(&alu,&nov);
        if(cmp==0)
        {
            if(alu.baja_logica==true && nov.novedad=='B')
            {
                fread(&alu,sizeof(t_alumnos),1,fMae);
                fread(&nov,sizeof(t_novedades),1,fNov);
            }
            if(nov.novedad=='M')//modicamos el alumno para el actualizado
            {
                modificarDatos(&nov,&alu);
                fwrite(&alu,sizeof(t_alumnos),1,fAct);
                //avanza los dos
                fread(&alu,sizeof(t_alumnos),1,fMae);
                fread(&nov,sizeof(t_novedades),1,fNov);
            }
            if(nov.novedad=='A'&& alu.baja_logica==false)
            {   ///Alta pero ya figura alta: error
                ///Copia el error como viene entonces si es de alumnos o de errores que estructura graba?
                copiarDatos(&aluError,&nov, NULL);
                fwrite(&aluError,sizeof(t_errores),1,pfError);
                fread(&nov,sizeof(t_novedades),1,fNov);
                fread(&alu,sizeof(t_alumnos),1,fMae);
            }
        }
        if(cmp < 0) ///alumnos es menor que la novedad//no está en novedad
        {
            if(alu.baja_logica==true) //era baja pero no fig en novedad
            {
                copiarDatos(&aluError,NULL, &alu);
                fwrite(&aluError,sizeof(t_errores),1,pfError);
                fread(&alu,sizeof(t_alumnos),1,fMae);
            }else //si es alta pero no modifica ni baja
            {
                fwrite(&alu,sizeof(t_alumnos),1,fAct);
                fread(&alu,sizeof(t_alumnos),1,fMae);
            }
        }
        if(cmp > 0)// alu > novedad
        {
            if(alu.baja_logica==true)
            {
                //COPIO EL ERROR en estructura valida
                copiarDatos(&aluError,NULL,&alu);
                fwrite(&aluError,sizeof(t_errores),1,pfError);
                fread(&alu,sizeof(t_alumnos),1,fMae);
            }
            if(nov.novedad=='B')//esta dado de baja pero no esta en maestro
            {
                copiarDatos(&aluError,&nov, NULL);
                fwrite(&aluError,sizeof(t_errores),1,pfError);
                fread(&nov, sizeof(t_novedades),1,fNov);
            } else if(nov.novedad=='A')//Alta
                {
                    fwrite(&nov,sizeof(t_novedades),1,fAct);
                    fread(&nov,sizeof(t_novedades),1,fNov);
                }
                else //MODIFICACION (Copia lo de novedades como está)
                    {
                        copiarDatos(&aluError,&nov, NULL);
                        fwrite(&aluError,sizeof(t_errores),1,pfError);
                        fread(&nov, sizeof(t_novedades),1,fNov);
                    }
        }
    }
    while(!feof(fMae))//se termino el de novedades
    {
        if(alu.baja_logica != true)
        {
          fwrite(&alu,sizeof(t_alumnos),1,fAct);
          fread(&alu,sizeof(t_alumnos),1,fMae);
        }else //si está de baja,pero NO novedades, copia el error y lee el sgte
        {
            copiarDatos(&aluError,NULL,&alu);
           fwrite(&aluError,sizeof(t_errores),1,pfError);
           fread(&alu,sizeof(t_alumnos),1,fMae);
        }
    }
    while(!feof(fNov))
    {
        if(nov.novedad=='A')//SOLO COPIA LAS ALTAS QUE QUEDASEN
        {
            fwrite(&nov,sizeof(t_novedades),1,fAct);
            fread(&nov,sizeof(t_novedades),1,fNov);
        }
        if(nov.novedad=='M' || nov.novedad=='B')//no existen en maestro
        {
            copiarDatos(&aluError,&nov, NULL);
            fwrite(&aluError,sizeof(t_errores),1,pfError);
            fread(&nov,sizeof(t_novedades),1,fNov);
        }
    }

    fclose(fMae);
    fclose(fNov);
    fclose(fAct);
    fclose(pfError);

    return 1;
}

int cmpAlumnoClave(const void *alu,const void *nov )
{
    t_alumnos *alumno= (t_alumnos*)alu;
    t_novedades *aluNov=(t_novedades*)nov;

    int cmp;
    if(!(cmp= alumno->legajo-aluNov->legajo))
    {
       if(!(cmp=strcmpi(alumno->apellido,aluNov->apellido)))
            cmp=strcmpi(alumno->nombre,aluNov->nombre);
    }

    return cmp;
}

void copiarDatos(t_errores *error, t_novedades* nov, t_alumnos* alu)
{
   if(nov!=NULL)
    {
        error->legajo= nov->legajo;

        error->dni= nov->dni;

        strcpy(error->apellido,nov->apellido);

        strcpy(error->nombre,nov->nombre);

        error->fNac.dia = nov->fNac.dia;
        error->fNac.mes = nov->fNac.mes;
        error->fNac.anio= nov->fNac.anio;

        error->fIngr.dia= nov->fIngr.dia;
        error->fIngr.mes= nov->fIngr.mes;
        error->fIngr.anio= nov->fIngr.anio;

        strcpy(error->direccion, nov->direccion);

        error->telefono= nov->telefono;

        strcpy(error->email,nov->email);

        error->novedad= nov->novedad; //Sigue igual
    }
    if(alu!=NULL)
    {
        error->legajo= alu->legajo;

        error->dni= alu->dni;

        strcpy(error->apellido,alu->apellido);

        strcpy(error->nombre,alu->nombre);

        error->fNac.dia = alu->fNac.dia;
        error->fNac.mes = alu->fNac.mes;
        error->fNac.anio= alu->fNac.anio;

        error->fIngr.dia = alu->fIngr.dia;
        error->fIngr.mes = alu->fIngr.mes;
        error->fIngr.anio= alu->fIngr.anio;

        strcpy(error->direccion, alu->direccion);

        error->telefono= alu->telefono;

        strcpy(error->email,alu->email);

        error->baja_logica= alu->baja_logica; //Sigue igual
    }

}
void modificarDatos(t_novedades *nov, t_alumnos* alu)
{
    int cmp;
    if((cmp=strcmpi(nov->apellido,alu->apellido))!=0)
        strcpy(alu->apellido,nov->apellido);
    if((cmp=strcmpi(nov->nombre,alu->nombre))!=0)
        strcpy(alu->nombre,nov->nombre);

    if(alu->fNac.dia!= nov->fNac.dia)
        alu->fNac.dia= nov->fNac.dia;
    if(alu->fNac.mes!= nov->fNac.mes)
        alu->fNac.mes= nov->fNac.mes;
    if(alu->fNac.anio!= nov->fNac.anio)
        alu->fNac.anio= nov->fNac.anio;

    if(alu->fIngr.dia!= nov->fIngr.dia)
        alu->fIngr.dia= nov->fIngr.dia;
    if(alu->fIngr.mes!= nov->fIngr.mes)
        alu->fIngr.mes= nov->fIngr.mes;
    if(alu->fIngr.anio!= nov->fIngr.anio)
        alu->fIngr.anio= nov->fIngr.anio;

    if((cmp=strcmpi(nov->direccion,alu->direccion))!=0)
        strcpy(alu->direccion, nov->direccion);

    if(((cmp= nov->telefono - alu->telefono))!=0)
        alu->telefono= nov->telefono;

    if((cmp=strcmpi(nov->email,alu->email))!=0)
        strcpy(alu->email,nov->email);

    alu->baja_logica=false; //Sigue igual

}

void mostrarArchivoAlu(char *nombreArch)
{
    FILE *pf;
    pf=fopen(nombreArch,"rb");
    if(!pf)
        return;

    t_alumnos alu;

    fread(&alu,sizeof(t_alumnos),1,pf);
    while(!feof(pf))
    {
        printf("%03d %08d %s %s %02d/%02d/%04d %02d/%02d/%04d %s %d %s\n",
                alu.legajo,
                alu.dni,
                alu.apellido,
                alu.nombre,
                alu.fNac.dia,
                alu.fNac.mes,
                alu.fNac.anio,
                alu.fIngr.dia,
                alu.fIngr.mes,
                alu.fIngr.anio,
                alu.direccion,
                alu.telefono,
                alu.email);

        fread(&alu,sizeof(t_alumnos),1,pf);
    }

    fclose(pf);
}
void mostrarArchivoNov(char *nombreArch)
{
    FILE *pf;
    pf=fopen(nombreArch,"rb");
    if(!pf)
        return;

    t_novedades nov;

    fread(&nov,sizeof(t_novedades),1,pf);
    while(!feof(pf))
    {
        printf("%03d %08d %s %s %02d/%02d/%04d %02d/%02d/%04d %s %d %s %c\n",
                nov.legajo,
                nov.dni,
                nov.apellido,
                nov.nombre,
                nov.fNac.dia,
                nov.fNac.mes,
                nov.fNac.anio,
                nov.fIngr.dia,
                nov.fIngr.mes,
                nov.fIngr.anio,
                nov.direccion,
                nov.telefono,
                nov.email,
                nov.novedad);

        fread(&nov,sizeof(t_novedades),1,pf);
    }

    fclose(pf);

}

void mostrarArchivoErrores(char *nombreArch)
{
    FILE *pf;
    pf=fopen(nombreArch,"rb");
    if(!pf)
        return;

    t_errores error;

    fread(&error,sizeof(t_errores),1,pf);
    while(!feof(pf))
    {
        printf("%03d %08d %s %s %02d/%02d/%04d %02d/%02d/%04d %s %d %s %c\n",
                error.legajo,
                error.dni,
                error.apellido,
                error.nombre,
                error.fNac.dia,
                error.fNac.mes,
                error.fNac.anio,
                error.fIngr.dia,
                error.fIngr.mes,
                error.fIngr.anio,
                error.direccion,
                error.telefono,
                error.email,
                error.novedad);

        fread(&error,sizeof(t_errores),1,pf);
    }

    fclose(pf);

}
