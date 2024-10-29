#include <stdio.h>
#include <string.h>

int main()
{
    char mensajeBinario[800];

    printf("Ingresa el mensaje: ");
    scanf("%s", mensajeBinario);
    int a = strlen(mensajeBinario) / 8;
    

    for (int i = 0; i < a; i++)
    {        
        int enAscii = 0;
        for (int j = 0; j < 8; j++)
        {
            enAscii = enAscii * 2 + (mensajeBinario[i * 8 + j] - '0');
        }
        printf("%c", enAscii);
    }

    printf("\n");

    return 0;
}