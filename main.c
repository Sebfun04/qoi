#include <stdio.h>
#include "Fonction.h"

void print_binary_file(const char* filename);

void write_pixel_rgb(pixel_t p, FILE *out) {
    fprintf(out, "%c%c%c%c", QOI_OP_RGB, p.r, p.g, p.b);
}

void write_pixel_rgba(pixel_t p, FILE *out) {
    fprintf(out, "%c%c%c%c%c", QOI_OP_RGBA, p.r, p.g, p.b, p.a);
}

int main(void) {

  // Création d'un tableau de pixels pour les tests
  pixel_t pixels[] = {
    {255, 0, 0, 255},    // Pixel rouge
    {0, 255, 0, 255},    // Pixel vert
    {0, 0, 255, 255},    // Pixel bleu
    {255, 255, 255, 255},// Pixel blanc
    {0, 0, 0, 255},      // Pixel noir
    {255, 0, 0, 255},    // Pixel rouge
    {0, 255, 0, 255},    // Pixel vert
    {0, 0, 255, 255}     // Pixel bleu
  };

  // Ouverture d'un fichier en écriture pour stocker le résultat de l'encodage
  FILE *out = fopen("output.qoi", "wb");
  if (out == NULL) {
    printf("Erreur à l'ouverture du fichier output.qoi\n");
    return 1;
  }

  for (int i = 0; i < 8; i++) {
    if (pixels[i].a == 255) {
      write_pixel_rgb(pixels[i], out);
    } else {
      write_pixel_rgba(pixels[i], out);
    }
  }

  // Appel de la fonction d'encodage QOI avec notre tableau de pixels
  /*if (qoi_encode(pixels, 8, 1, 4, 0, out) != 0) {
    printf("Erreur pendant l'encodage QOI\n");
    return 1;
  }*/

  // Fermeture du fichier
  fclose(out);

  printf("Encodage QOI terminé avec succès\n");
  print_binary_file("output.qoi");
  return 0;
}

void print_binary_byte(int c) {
    for (int i = 7; i >= 0; i--) {
        putchar((c & (1 << i)) ? '1' : '0');
    }
}

void print_binary_file(const char* filename) {
    FILE* in = fopen(filename, "rb");
    if (in == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
        return;
    }

    int c;
    while ((c = fgetc(in)) != EOF) {
        printf("Hex: %02X | Binary: ", c);
        print_binary_byte(c);
        printf(" | Decimal: %03d\n", c);
    }

    fclose(in);
}
