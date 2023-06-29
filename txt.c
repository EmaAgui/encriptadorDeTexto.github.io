

void intercambiar(void * d1, void *d2, size_t tam)
{
    void *aux = malloc(tam);

    if(!aux)
        return;
    memcpy(aux, d1, tam);
    memcpy(d1, d2, tam);
    memcpy(d2, aux, tam);

    free(aux);
}


void *buscarMenor(const void *ini, const void *fin, size_t tam, int (*cmp)(const void *d1, const void *d2))
{
    const void *men = ini;
    ini += tam;

    while(ini <= fin)
    {
        if(cmp(ini, men) < 0)
            men = ini;

        ini += tam;
    }

    return (void*)men;
}


void *ordenarSeleccion(void *vec, size_t ce, size_t tam, int (*cmp)(const void *d1, const void *d2))
{
    void *ini = vec;
    void *fin = vec + (ce - 1)*tam;
    void *men;

    while(vec < fin)
    {
        men = buscarMenor(vec, fin, tam, cmp);
        if(vec != men)
            intercambiar(vec, men, tam);
        vec+=tam;
    }
    return ini;
}


void *busquedaBinaria(const void *clave, const void *base, size_t nMenb, size_t tam, int (*comparar)(const void *d1, const void *d2))
{
    int li = 0;
    int ls = nMenb - 1;
    int pm, comp;

    while (li <= ls)
    {
        pm = (li + ls) / 2;
        comp = comparar(base + (pm * tam), clave);

        if (comp == 0)
            return (void*)(base + (pm * tam));

        if (comp > 0)
            ls = pm - 1;
        else
            li = pm + 1;
    }

    return NULL;
}





void trozarArchVar(void *dato, char *cad)
{
    Alumno *alu = (Alumno*)dato;
    char *aux = strchr(cad, '\n');
    *aux = '\0';

    aux = strrchr(cad, '|');
    sscanf(aux + 1, "%f", &(alu->nota));
    *aux = '\0';

    aux = strrchr(cad, '|');
    strcpy(alu->nombre, aux + 1);
    *aux = '\0';


    sscanf(cad, "%d", &(alu->dni));
}


void trozarArchFijo(void *dato, char *cad)
{
    Alumno *alu = (Alumno*)dato;
    char *aux = cad;

    while(*aux)
        aux++;

    *aux = '\0';

    aux -= 6;
    sscanf(aux, "%f", &(alu->nota));
    *aux = '\0';

    aux -= 29;
    strcpy(alu->nombre, aux);
    *aux = '\0';

    aux -= 8;
    sscanf(aux, "%d", &(alu->dni));
    *aux = '\0';
}


void actualizarSueldo(const char *nombreEmp, const char *nombreEst)
{
    Estudiante est;
    Empleado emp;
    FILE *pEmp, *pEst;

    pEmp = fopen(nombreEmp, "r+b");
    pEst = fopen(nombreEst, "rb");

    if(!pEmp || ! pEst)
    {
        fclose(pEmp);
        fclose(pEst);
        return;
    }

    fread(&emp, sizeof(emp), 1, pEmp);
    fread(&est, sizeof(est), 1, pEst);

    while(!feof(pEmp) && !feof(pEst))
    {
        int cmp = compararEstEmp(&est, &emp);
        if(cmp == 0 && est.promedio > 7)
        {
            emp.sueldo *= 1.0728;
            fseek(pEmp, -(long)sizeof(Empleado), SEEK_CUR);
            fwrite(&emp, sizeof(Empleado), 1, pEmp);
            fseek(pEmp, 0L, SEEK_CUR);
            fread(&emp, sizeof(emp), 1, pEmp);
            fread(&est, sizeof(est), 1, pEst);
        }
        else
        {
            if(compararEstEmp(&est, &emp) > 0)
                fread(&emp, sizeof(emp), 1, pEmp);
            else
                fread(&est, sizeof(est), 1, pEst);
        }
    }

    fclose(pEmp);
    fclose(pEst);
}



int fBusquedaBinaria(const char *nombreArchivo, void *info, unsigned tamBytes, int comparacion(const void *dato1, const void *dato2))
{
    FILE *pArch = fopen(nombreArchivo, "rb");

    void *alu = malloc(tamBytes);

    if(!pArch || !alu)
        return 0;

    int cantRegistros, li, ls, pm, comp;

    fseek(pArch, 0, SEEK_END); ///PARARSE AL FINAL DEL ARCHIVO
    cantRegistros = ftell(pArch) / tamBytes; ///OBTENER CANTIDAD DE REGISTROS

    li = 0;
    ls = cantRegistros - 1;
    pm = li +(ls +li) / 2;

    fseek(pArch, pm * tamBytes, SEEK_CUR);

    fread(alu, tamBytes, 1, pArch);

    while ( li <= ls) ///MIENTRAR NO ENCUENTRE EL REGISTRO
    {
        comp = comparacion(alu, info);
        if(comp > 0) /// alu >
        {
            li = 0;
            ls = pm - 1;
            pm = ls;
        }

        if(comp < 0) ///alu <
        {
            li = pm + 1;
            ls = ls;
            pm = li;
        }

        if (comp == 0)
        {
            memcpy(info, alu, tamBytes);
            free(alu);
            return 1;
        }

        fseek(pArch, pm * tamBytes, SEEK_SET);
        fread(alu, tamBytes, 1, pArch);
    }

    free(alu);
    return 0;
}
