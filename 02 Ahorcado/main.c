// Juego del ahorcado
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void recibirPalabra();
void validarPalabra();   // solo letras y de 3 a 20 caracteres
int longitudPalabra();   // de 3 a 20 caracteres
int soloLetras();        // solo letras
void minusculas();       // convertir a minusculas
int recibirLetra();      // recibir letra
int compararLetra();     // si la letra ya se ingresó
void imprimirPantalla(); // imprimir pantalla
int ganaste();           // si se adivinó la palabra

int main()
{
    char palabra[21] = "";
    char letra[2] = "";
    char letrasIngresadas[27] = "";
    char letrasIncorrectas[27] = "";
    int errores = 0;
    int fin = 0;

    printf("Juego del ahorcado\n");

    recibirPalabra(palabra);

    printf("\e[1;1H\e[2J");

    printf("Comienza el juego\n");

    while (1 != fin)
    {
        imprimirPantalla(palabra, &errores, letrasIngresadas, letrasIncorrectas);
        errores = recibirLetra(letrasIngresadas, letra, palabra, letrasIncorrectas, errores);
        if (5 <= errores)
        {
            printf("Perdiste, la palabra era: %s\n", palabra);
            fin = 1;
        }
        if (ganaste(palabra, letrasIngresadas))
        {
            printf("Ganaste (gran aplauso)!, la palabra era: %s\n", palabra);
            fin = 1;
        }
    }

    return 0;
}

void recibirPalabra(char palabra[21])
{
    int reingresar = 0;

    while (1 != reingresar)
    {
        printf("Ingresa la palabra: ");
        scanf("%s", palabra);

        validarPalabra(palabra);

        printf("\e[1;1H\e[2J");

        printf("La palabra es: %s\nSi desea continuar, ingrese 1.\nSi desea ingresar la palabra de nuevo, ingrese cualquier otro numero.\n", palabra);
        scanf("%d", &reingresar);

        fflush(stdin);
    }
}

void validarPalabra(char palabra[21])
{
    int valido = 1;
    do
    {
        if (1 != longitudPalabra(palabra))
        {
            printf("La palabra debe tener entre 3 y 20 caracteres.\n");
            printf("Ingresa la palabra: ");
            scanf("%s", palabra);
            valido = 0;
        }
        else if (1 != soloLetras(palabra))
        {
            printf("La palabra solo debe contener letras.\n");
            printf("Ingresa la palabra: ");
            scanf("%s", palabra);
            valido = 0;
        }
        else
        {
            minusculas(palabra);
            valido = 1;
        }
    } while (1 != valido);
}

int longitudPalabra(char palabra[21])
{
    if (3 > strlen(palabra) || 20 < strlen(palabra))
    {
        return 0;
    }
    return 1;
}

int soloLetras(char palabra[21])
{
    for (unsigned int i = 0; i < strlen(palabra); i++)
    {
        if ((palabra[i] < 'a' || palabra[i] > 'z') && (palabra[i] < 'A' || palabra[i] > 'Z'))
        {
            return 0;
        }
    }
    return 1;
}

void minusculas(char palabra[21])
{
    for (unsigned int i = 0; i < strlen(palabra); i++)
    {
        if (palabra[i] >= 'A' && palabra[i] <= 'Z')
        {
            palabra[i] += 32;
        }
    }
}

int recibirLetra(char letrasIngresadas[27], char letra[2], char palabra[21], char letrasIncorrectas[27], int errores)
{
    int reingresar = 1;

    do
    {
        printf("Ingresa la letra: ");
        scanf("%s", letra);

        if ((letra[0] < 'a' || letra[0] > 'z'))
        {
            if (letra[0] >= 'A' && letra[0] <= 'Z')
            {
                letra[0] += 32;
                errores = compararLetra(letrasIngresadas, letra, palabra, letrasIncorrectas, errores);
                printf("\e[1;1H\e[2J");
                reingresar = 1;
                return errores;
            }
            else
            {
                printf("Debe ingresar una letra minuscula.\n");
                reingresar = 0;
            }
        }
        else
        {
            errores = compararLetra(letrasIngresadas, letra, palabra, letrasIncorrectas, errores);
            printf("\e[1;1H\e[2J");
            reingresar = 1;
            return errores;
        }

    } while (1 != reingresar);
    return errores;
}

int compararLetra(char letrasIngresadas[27], char letra[2], char palabra[21], char letrasIncorrectas[27], int errores)
{
    for (unsigned int i = 0; i < strlen(letrasIngresadas); i++)
    {
        if (letrasIngresadas[i] == letra[0])
        {
            printf("La letra ya fue ingresada, pulse Enter y vuelva a ingresarla.");
            getchar();
            getchar();
        }
    }

    int encontrada = 0;
    for (unsigned int i = 0; i < strlen(palabra); i++)
    {
        if (palabra[i] == letra[0])
        {
            encontrada = 1;
        }
    }
    if (1 != encontrada)
    {
        if (letrasIncorrectas[strlen(letrasIncorrectas) - 1] != letra[0])
        {
            letrasIncorrectas[strlen(letrasIncorrectas)] = letra[0];
        }
        else
        {
            printf("La letra incorrecta ya habia sido ingresada, pulse Enter e ingrese otra.");
            getchar();
            getchar();
        }
        errores++;
    }
    letrasIngresadas[strlen(letrasIngresadas)] = letra[0];
    return errores;
}

void imprimirPantalla(char palabra[21], int *errores, char letrasIngresadas[27], char letrasIncorrectas[27])
{
    for (unsigned int i = 0; i < strlen(palabra); i++)
    {
        int found = 0;
        for (unsigned int j = 0; j < strlen(letrasIngresadas); j++)
        {
            if (palabra[i] == letrasIngresadas[j])
            {
                found = 1;
                break;
            }
        }
        if (found)
        {
            printf("%c ", palabra[i]);
        }
        else
        {
            printf("_ ");
        }
    }
    printf("\n");
    printf("%d de 5 errores: ", *errores);
    for (unsigned int i = 0; i < strlen(letrasIncorrectas); i++)
    {
        printf("%c ", letrasIncorrectas[i]);
    }
    printf("\n");
}

int ganaste(char palabra[21], char letrasIngresadas[27])
{
    for (unsigned int i = 0; i < strlen(palabra); i++)
    {
        int encontrada = 0;
        for (unsigned int j = 0; j < strlen(letrasIngresadas); j++)
        {
            if (palabra[i] == letrasIngresadas[j])
            {
                encontrada = 1;
                break;
            }
        }
        if (0 == encontrada)
        {
            return 0;
        }
    }
    return 1;
}