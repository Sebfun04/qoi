#include <stdio.h>
#include "Fonction.h"

int main(void) {

  // Création d'un tableau de pixels pour les tests
  pixel_t pixels[] = {
    {253, 0, 0, 255},    // Pixel rouge
    {0, 76, 0, 255},    // Pixel vert
    {0, 76, 0, 255},    // Pixel bleu
    {122, 2, 33, 255},// Pixel blanc
    {0, 76, 0, 255},      // Pixel noir
    {45, 0, 0, 255},    // Pixel rouge
    {0, 143, 0, 255},    // Pixel vert
    {122, 2, 33, 255}     // Pixel bleu
  };

  // Ouverture d'un fichier en écriture pour stocker le résultat de l'encodage
  FILE *out = fopen("output.qoi", "wb");
  if (out == NULL) {
    printf("Erreur à l'ouverture du fichier output.qoi\n");
    return 1;
  }

  // Appel de la fonction d'encodage QOI avec notre tableau de pixels
  if (qoi_encode(pixels, 8, 1, 4, 0, out) != 0) {
    printf("Erreur pendant l'encodage QOI\n");
    return 1;
  }

  // Fermeture du fichier
  fclose(out);

  printf("Encodage QOI terminé avec succès\n");
  print_binary_file("output.qoi");
  return 0;
}




// Voici les fonctions possibles !
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

void print_pixel_info(const char* op, pixel_t p, uint8_t channels) {
    printf("%s | %02X %02X %02X", op, p.r, p.g, p.b);
    if (channels == 4) {
        printf(" %02X", p.a);
    }
    printf(" | ");
    print_binary_byte(p.r);
    printf(" ");
    print_binary_byte(p.g);
    printf(" ");
    print_binary_byte(p.b);
    if (channels == 4) {
        print_binary_byte(p.a);
    }
    printf(" | %03d %03d %03d", p.r, p.g, p.b);
    if (channels == 4) {
        printf(" %03d", p.a);
    }
    printf("\n");
}

// Fonction pour encoder une image en QOI
int qoi_encode(pixel_t *pixels, uint32_t width, uint32_t height, uint8_t channels, uint8_t colorspace, FILE *out) {
  qoi_header header = {QOI_MAGIC, width, height, channels, colorspace};

  if (fwrite(&header, sizeof(header), 1, out) != 1) {
    return -1; // Erreur d'écriture de l'en-tête
  }

  // Initialisation des pixels précédents et du tableau d'index
  pixel_t prev = {0, 0, 0, 255};
  pixel_t index_array[64] = {0};

  for (int i = 0; i < width * height; i++) {
    pixel_t p = pixels[i];
    uint8_t index = (p.r * 3 + p.g * 5 + p.b * 7 + p.a * 11) % 64;
    printf("Pixel %d: ", i+1);

    if (memcmp(&p, &index_array[index], sizeof(pixel_t)) == 0) {
      // Ecrire un QOI_OP_INDEX
      fputc(QOI_OP_INDEX | index, out);
      print_pixel_info("INDEX", p, channels);
    }
    else if (p.r == prev.r && p.g == prev.g && p.b == prev.b && p.a == prev.a) {
      // Ecrire un QOI_OP_RUN
      fputc(QOI_OP_RUN | 1, out); // On suppose qu'il n'y a qu'un seul pixel à répéter
      print_pixel_info("RUN", p, channels);
    } 
    else if (channels == 3) {
      // Ecrire un QOI_OP_RGB
      fputc(QOI_OP_RGB, out);
      fputc(p.r, out);
      fputc(p.g, out);
      fputc(p.b, out);
      print_pixel_info("RGB", p, channels);
    } 
    else {
      // Ecrire un QOI_OP_RGBA
      fputc(QOI_OP_RGBA, out);
      fputc(p.r, out);
      fputc(p.g, out);
      fputc(p.b, out);
      fputc(p.a, out);
      print_pixel_info("RGBA", p, channels);
    }

    prev = p;
    index_array[index] = p;
  }

  // Ecrire le marqueur de fin
  for (int i = 0; i < 7; i++) {
    fputc(0, out);
  }
  fputc(1, out);

  return 0;
}




