#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Fecha
{
    int dia, mes, anio;
} Fecha;

typedef enum Estado
{
    PENDIENTE,
    COMPLETADO
} Estado;

typedef struct Pendiente
{
    char titulo[51], encargado[21];
    Estado estado;
    Fecha fecha;

} Pendiente;

void ImprimirPendiente(Pendiente pendiente);
void ImprimirListaDePendientes(Pendiente pendientes[], int cantidadPendientes);
void ImprimirFecha(Fecha fecha);
int LeerArchivoDePendientes(char *nombreArchivo, Pendiente pendientes[]);
int ImprimirMenu();
int AccionMenu(int opcion, Pendiente pendientes[], int cantidadPendientes, int *ptrCantidadPendientes, char *nombreArchivo);
void EditarPendiente(Pendiente pendientes[], int cantidadPendientes);
void AgregarPendiente(Pendiente pendientes[], int cantidadPendientes, int *ptrCantidadPendientes);
void BorrarPendiente(Pendiente pendientes[], int cantidadPendientes, int *ptrCantidadPendientes);
void GuardarCambios(Pendiente pendientes[], int cantidadPendientes, char *nombreArchivo);

int main(int argc, char *argv[])
{
    FILE *archivo = fopen(argv[1], "r");

    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo\n");
        return 1; // terminamos el programa haciendo return en la funcion main, y mandamos un valor distinto a 0 para indicar error.
    }

    Pendiente pendientes[100];

    int cantidadPendientes = LeerArchivoDePendientes(argv[1], pendientes);
    int *ptrCantidadPendientes = &cantidadPendientes;

    int salir = 0;

    while (salir == 0)
    {
        ImprimirListaDePendientes(pendientes, cantidadPendientes);

        int opcion = ImprimirMenu();

        salir = AccionMenu(opcion, pendientes, cantidadPendientes, ptrCantidadPendientes, argv[1]);

        printf("\e[1;1H\e[2J");
    }

    fclose(archivo);

    return 0;
}

int LeerArchivoDePendientes(char *nombreArchivo, Pendiente pendientes[])
{
    FILE *archivo = fopen(nombreArchivo, "r");

    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo\n");
        return 1; // terminamos el programa haciendo return en la funcion main, y mandamos un valor distinto a 0 para indicar error.
    }

    char linea[100];
    fgets(linea, 100, archivo);
    int cantidadPendientes = atoi(linea);

    char lineas[cantidadPendientes * 4 + 1][100];

    for (int i = 0; i < cantidadPendientes * 4 + 1; i++)
    {
        fgets(lineas[i], 100, archivo);
        lineas[i][strcspn(lineas[i], "\n")] = 0;
    }

    for (int i = 0; i < cantidadPendientes; i++)
    {
        strcpy(pendientes[i].titulo, lineas[i * 4]);
        strcpy(pendientes[i].encargado, lineas[i * 4 + 1]);

        sscanf(lineas[i * 4 + 2], "%d/%d/%d", &pendientes[i].fecha.anio, &pendientes[i].fecha.mes, &pendientes[i].fecha.dia);

        if (strcmp(lineas[i * 4 + 3], "pendiente") == 0)
        {
            pendientes[i].estado = PENDIENTE;
        }
        else if (strcmp(lineas[i * 4 + 3], "completado") == 0)
        {
            pendientes[i].estado = COMPLETADO;
        }
    }

    fclose(archivo);

    return atoi(linea);
}

void ImprimirListaDePendientes(Pendiente pendientes[], int cantidadPendientes)
{
    printf("\n=== Pendientes ===\n");
    printf("Se leyeron %d pendientes del archivo\n", cantidadPendientes);
    for (int i = 0; i < cantidadPendientes; i++)
    {
        printf("%d. ", i + 1);
        ImprimirPendiente(pendientes[i]);
        ImprimirFecha(pendientes[i].fecha);
    }
}

void ImprimirPendiente(Pendiente pendiente)
{
    if (pendiente.estado == PENDIENTE)
    {
        printf("%s ", "🕙");
    }
    else
    {
        printf("%s ", "✅");
    }
    printf("%s ", pendiente.titulo);
    printf("(%s) ", pendiente.encargado);
}

void ImprimirFecha(Fecha fecha)
{
    printf("[%d/%d/%d]\n", fecha.anio, fecha.mes, fecha.dia);
}

int ImprimirMenu()
{
    printf("\n=== Acciones ===\n");

    int opcion;
    printf("1. Editar un pendiente\n");
    printf("2. Agregar un pendiente\n");
    printf("3. Borrar un pendiente\n");
    printf("4. Guardar los cambios\n");
    printf("5. Salir\n");
    scanf("%d", &opcion);

    return opcion;
}

int AccionMenu(int opcion, Pendiente pendientes[], int cantidadPendientes, int *ptrCantidadPendientes, char *nombreArchivo)
{
    switch (opcion)
    {
    case 1:
        EditarPendiente(pendientes, cantidadPendientes);
        return 0;
    case 2:
        AgregarPendiente(pendientes, cantidadPendientes, ptrCantidadPendientes);
        return 0;
    case 3:
        BorrarPendiente(pendientes, cantidadPendientes, ptrCantidadPendientes);
        return 0;
    case 4:
        GuardarCambios(pendientes, cantidadPendientes, nombreArchivo);
        return 0;
    case 5:
        return 1;
    default:
        printf("Opcion invalida\n");
        return 0;
    }
}

void EditarPendiente(Pendiente pendientes[], int cantidadPendientes)
{
    printf("Elija el pendiente a editar [1-%d]\n", cantidadPendientes);
    int opcion;
    scanf("%d", &opcion);

    int repetir = 0;

    while (repetir == 0)
    {
        printf("\e[1;1H\e[2J");
        printf("%s %s (%s) [%d/%d/%d]\n", pendientes[opcion - 1].estado == PENDIENTE ? "🕙" : "✅", pendientes[opcion - 1].titulo, pendientes[opcion - 1].encargado, pendientes[opcion - 1].fecha.anio, pendientes[opcion - 1].fecha.mes, pendientes[opcion - 1].fecha.dia);

        printf("Que desea hacer?\n");
        printf("1. Cambiar Titulo\n");
        printf("2. Cambiar Encargado\n");
        printf("3. Cambiar Fecha\n");
        printf("4. Cambiar Estado\n");
        printf("5. Dejar de Editar\n");

        int campo;
        scanf("%d", &campo);

        int c;
        size_t len;

        switch (campo)
        {
        case 1:
            printf("Ingrese el nuevo titulo\n");

            // Limpiar el buffer de entrada
            while ((c = getchar()) != '\n' && c != EOF)
                ;

            fgets(pendientes[opcion - 1].titulo, 51, stdin);

            // Eliminar el salto de linea del final
            len = strlen(pendientes[opcion - 1].titulo);
            if (len > 0 && pendientes[opcion - 1].titulo[len - 1] == '\n')
            {
                pendientes[opcion - 1].titulo[len - 1] = '\0';
            }

            break;
        case 2:
            printf("Ingrese el nuevo encargado\n");

            // Limpiar el buffer de entrada
            while ((c = getchar()) != '\n' && c != EOF)
                ;

            fgets(pendientes[opcion - 1].encargado, 21, stdin);

            // Eliminar el salto de linea del final
            len = strlen(pendientes[opcion - 1].encargado);
            if (len > 0 && pendientes[opcion - 1].encargado[len - 1] == '\n')
            {
                pendientes[opcion - 1].encargado[len - 1] = '\0';
            }

            break;
        case 3:
            printf("Ingrese la nueva fecha [YYYY/MM/DD]\n");
            int valido = 3;
            do
            {
                if (3 != valido)
                {
                    printf("Fecha invalida, ingrese la fecha [YYYY/MM/DD]\n");
                }
                valido = scanf("%d/%d/%d", &pendientes[opcion - 1].fecha.anio, &pendientes[opcion - 1].fecha.mes, &pendientes[opcion - 1].fecha.dia);

                // Limpiar el buffer de entrada
                while ((c = getchar()) != '\n' && c != EOF)
                    ;
            } while (3 != valido);
            break;
        case 4:
            printf("Ingrese el nuevo estado\n");
            char estado[11];
            scanf("%s", estado);
            if (strcmp(estado, "pendiente") == 0)
            {
                pendientes[opcion - 1].estado = PENDIENTE;
            }
            else if (strcmp(estado, "completado") == 0)
            {
                pendientes[opcion - 1].estado = COMPLETADO;
            }
            break;
        case 5:
            repetir = 1;
            break;
        default:
            printf("Opcion invalida\n");
            break;
        }
    }
}

void AgregarPendiente(Pendiente pendientes[], int cantidadPendientes, int *ptrCantidadPendientes)
{
    int c;
    // Limpiar el buffer de entrada
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    printf("Ingrese el titulo\n");
    fgets(pendientes[cantidadPendientes].titulo, 51, stdin);

    // Eliminar el salto de linea del final
    size_t len = strlen(pendientes[cantidadPendientes].titulo);
    if (len > 0 && pendientes[cantidadPendientes].titulo[len - 1] == '\n')
    {
        pendientes[cantidadPendientes].titulo[len - 1] = '\0';
    }

    printf("Ingrese el encargado\n");
    fgets(pendientes[cantidadPendientes].encargado, 21, stdin);

    // Eliminar el salto de linea del final
    len = strlen(pendientes[cantidadPendientes].encargado);
    if (len > 0 && pendientes[cantidadPendientes].encargado[len - 1] == '\n')
    {
        pendientes[cantidadPendientes].encargado[len - 1] = '\0';
    }

    printf("Ingrese la fecha [YYYY/MM/DD]\n");
    int valido = 3;
    do
    {
        if (3 != valido)
        {
            printf("Fecha invalida, ingrese la fecha [YYYY/MM/DD]\n");
        }
        valido = scanf("%d/%d/%d", &pendientes[cantidadPendientes].fecha.anio, &pendientes[cantidadPendientes].fecha.mes, &pendientes[cantidadPendientes].fecha.dia);

        // Limpiar el buffer de entrada
        while ((c = getchar()) != '\n' && c != EOF)
            ;
    } while (3 != valido);

    pendientes[cantidadPendientes].estado = PENDIENTE;

    *ptrCantidadPendientes = *ptrCantidadPendientes + 1;
}

void BorrarPendiente(Pendiente pendientes[], int cantidadPendientes, int *ptrCantidadPendientes)
{
    printf("Elija el pendiente a borrar [1-%d]\n", cantidadPendientes);
    int opcion;
    scanf("%d", &opcion);

    for (int i = opcion - 1; i < cantidadPendientes - 1; i++)
    {
        pendientes[i] = pendientes[i + 1];
    }

    *ptrCantidadPendientes = *ptrCantidadPendientes - 1;
}

void GuardarCambios(Pendiente pendientes[], int cantidadPendientes, char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "w");

    fprintf(archivo, "%d\n", cantidadPendientes);

    for (int i = 0; i < cantidadPendientes; i++)
    {
        fprintf(archivo, "%s\n", pendientes[i].titulo);
        fprintf(archivo, "%s\n", pendientes[i].encargado);
        fprintf(archivo, "%d/%d/%d\n", pendientes[i].fecha.anio, pendientes[i].fecha.mes, pendientes[i].fecha.dia);
        if (pendientes[i].estado == PENDIENTE)
        {
            fprintf(archivo, "pendiente\n");
        }
        else if (pendientes[i].estado == COMPLETADO)
        {
            fprintf(archivo, "completado\n");
        }
    }

    fclose(archivo);
}
