#include <stdio.h>

int main()
{
    int N;
    printf("Ingresa la longitud del mensaje: ");
    scanf("%d", &N);

    char mensajeBinario[N * 8];
    printf("Ingresa el mensaje: ");
    scanf("%s", mensajeBinario);

    for (int i = 0; i < N; i++)
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