#include <stdio.h>
#include <stdlib.h>
#include "Fonction.h"
#include <math.h>

int main(void) {

  // Création d'un tableau de pixels pour les tests
  pixel_t pixels[] = {
  {253, 0, 0, 255}, // Pixel rouge pour tester le bloc QOI_OP_RGBA
  {0, 76, 0, 255}, // Pixel vert différent pour tester le bloc QOI_OP_RGB
  {0, 76, 0, 255}, // Pixel vert identique pour tester le bloc QOI_OP_INDEX
  {1, 76, 1, 255}, // Pixel avec une petite différence pour tester le bloc QOI_OP_DIFF
  {2, 77, 2, 255}, // Pixel avec des différences appropriées pour tester le bloc QOI_OP_LUMA

  // Ajout d'un certain nombre de pixels identiques pour tester le bloc QOI_OP_RUN
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {2, 77, 2, 255},
  {3, 78, 3, 255} // Un pixel différent pour terminer la séquence de pixels identiques
};

  // Ouverture d'un fichier en écriture pour stocker le résultat de l'encodage
  FILE *out = fopen("output.qoi", "wb");
  if (out == NULL) {
    printf("Erreur à l'ouverture du fichier output.qoi\n");
    return 1;
  }

  // Appel de la fonction d'encodage QOI avec notre tableau de pixels
  if (qoi_encode(pixels, 50, 1, 4, 0, out) != 0) {
    printf("Erreur pendant l'encodage QOI\n");
    return 1;
  }

  // Fermeture du fichier
  fclose(out);

  //print_binary_file("output.qoi");
  printf("Encodage QOI terminé avec succès\n");
  
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
    printf(" ");
    if (channels == 4) {
        print_binary_byte(p.a);
    }
    printf(" | %03d %03d %03d", p.r, p.g, p.b);
    if (channels == 4) {
        printf(" %03d", p.a);
    }
    printf("\n");
}

int qoi_encode(pixel_t *pixels, uint32_t width, uint32_t height, uint8_t channels, uint8_t colorspace, FILE *out) {
  qoi_header header = {QOI_MAGIC, width, height, channels, colorspace};

  if (fwrite(&header, sizeof(header), 1, out) != 1) {
    return -1; // Erreur d'écriture de l'en-tête
  }

  // Initialisation des pixels précédents et du tableau d'index
  pixel_t prev = {0, 0, 0, 255};
  pixel_t index_array[64] = {0};
  int run_length = 0;
  pixel_t prev_run = {0, 0, 0, 0};


  for (int i = 0; i < width * height; i++) {
    pixel_t p = pixels[i];
    uint8_t index = (p.r * 3 + p.g * 5 + p.b * 7 + p.a * 11) % 64;
    printf("Pixel %d: ", i+1);

     // If current pixel matches previous and run_length is less than 62
    if (memcmp(&p, &prev_run, sizeof(pixel_t)) == 0 && run_length < 62) {
      run_length++;
      printf("\n");
      continue;  // Skip to next pixel
      } 
      else {
        if (run_length > 0) {
          // Write a QOI_OP_RUN
          uint8_t val = QOI_OP_RUN | run_length;
          fputc(val, out);
          printf("QOI_OP_RUN: Hex: %02X | Binary: ", val);
          print_binary_byte(val);
          printf(" | Length: %d\n", run_length);
          run_length = 0;  // Reset run length
        }

        // Set current pixel as the start of a new run
        prev_run = p;

      if (memcmp(&p, &index_array[index], sizeof(pixel_t)) == 0) {
        // Write a QOI_OP_INDEX
        uint8_t val = QOI_OP_INDEX | index;
        fputc(val, out);
        printf("QOI_OP_INDEX: Hex: %02X | Binary: ", val);
        print_binary_byte(val);
        printf("\n");

        //print_pixel_info("INDEX", p, channels);
      }

      else if (abs(p.r - prev.r) <= 1 && abs(p.g - prev.g) <= 1 && abs(p.b - prev.b) <= 1) {
        // Write a QOI_OP_DIFF
        uint8_t dr = (p.r - prev.r) & 3;
        uint8_t dg = (p.g - prev.g) & 3;
        uint8_t db = (p.b - prev.b) & 3;

        uint8_t val = QOI_OP_DIFF | (dr << 4) | (dg << 2) | db;
        fputc(val, out);
        printf("QOI_OP_DIFF: Hex: %02X | Binary: ", val);
        print_binary_byte(val);
        printf("\n");

        //print_pixel_info("DIFF", p, channels);
      }

      else if (abs(p.g - prev.g) <= 31 && abs(p.r - p.g - (prev.r - prev.g)) <= 7 && abs(p.b - p.g - (prev.b - prev.g)) <= 7) {
        // Write a QOI_OP_LUMA
        uint8_t dg = (p.g - prev.g) & 63;
        uint8_t dr_dg = (p.r - p.g - (prev.r - prev.g)) & 15;
        uint8_t db_dg = (p.b - p.g - (prev.b - prev.g)) & 15;

        uint8_t val = QOI_OP_LUMA | (dg << 2);
        fputc(val, out);
        printf("QOI_OP_LUMA 1st byte: Hex: %02X | Binary: ", val);
        print_binary_byte(val);
        printf("\n");

        val = (dr_dg << 4) | db_dg;
        fputc(val, out);
        printf("QOI_OP_LUMA 2nd byte: Hex: %02X | Binary: ", val);
        print_binary_byte(val);
        printf("\n");

        print_pixel_info("LUMA", p, channels);
      }
      else if (channels == 3) {
        // Write a QOI_OP_RGB
        uint8_t val = QOI_OP_RGB;
        fputc(val, out);
        printf("QOI_OP_RGB: Hex: %02X | Binary: ", val);
        print_binary_byte(val);
        printf(" | ");

        // Now output RGB
        val = p.r;
        fputc(val, out);
        printf("Red: Hex: %02X | Binary: ", val);
        print_binary_byte(val);
        printf(" | ");

        val = p.g;
        fputc(val, out);
        printf("Green: Hex: %02X | Binary: ", val);
        print_binary_byte(val);
        printf(" | ");

        val = p.b;
        fputc(val, out);
        printf("Blue: Hex: %02X | Binary: ", val);
        print_binary_byte(val);
        printf("\n");

        //print_pixel_info("RGB", p, channels);
      }
      else {
    // Write a QOI_OP_RGBA
        uint8_t val = QOI_OP_RGBA;
        fputc(val, out);
        printf("QOI_OP_RGBA: Hex: %02X | Binary: ", val);
        print_binary_byte(val);
        printf(" | ");

        // Now output RGBA
        val = p.r;
        fputc(val, out);
        printf("Red: Hex: %02X | Binary: ", val);
        print_binary_byte(val);
        printf(" | ");

        val = p.g;
        fputc(val, out);
        printf("Green: Hex: %02X | Binary: ", val);
        print_binary_byte(val);
        printf(" | ");

        val = p.b;
        fputc(val, out);
        printf("Blue: Hex: %02X | Binary: ", val);
        print_binary_byte(val);
        printf(" | ");

        val = p.a;
        fputc(val, out);
        printf("Alpha: Hex: %02X | Binary: ", val);
        print_binary_byte(val);
        printf("\n");

        //print_pixel_info("RGBA", p, channels);
      }

      prev = p;
      index_array[index] = p;
    }
  } 
  // After the loop, check and write remaining run length if any.
  if (run_length > 0) {
    uint8_t val = QOI_OP_RUN | run_length;
    fputc(val, out);
    printf("QOI_OP_RUN: Hex: %02X | Binary: ", val);
    print_binary_byte(val);
    printf(" | Length: %d\n", run_length);
  }

  // Write the end marker
  for (int i = 0; i < 7; i++) {
    uint8_t val = 0;
    fputc(val, out);
    printf("End marker (zeroes): Hex: %02X | Binary: ", val);
    print_binary_byte(val);
    printf("\n");
  }
  uint8_t val = 1;
  fputc(val, out);
  printf("End marker (one): Hex: %02X | Binary: ", val);
  print_binary_byte(val);
  printf("\n");
  return 0;
}





