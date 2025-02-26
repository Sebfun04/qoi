#include"Fonction.h"

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

    if (memcmp(&p, &index_array[index], sizeof(pixel_t)) == 0) {
      // Ecrire un QOI_OP_INDEX
      fputc(QOI_OP_INDEX | index, out);
    } else if (p.r == prev.r && p.g == prev.g && p.b == prev.b && p.a == prev.a) {
      // Ecrire un QOI_OP_RUN
      fputc(QOI_OP_RUN | 1, out); // On suppose qu'il n'y a qu'un seul pixel à répéter
    } else if (channels == 3) {
      // Ecrire un QOI_OP_RGB
      fputc(QOI_OP_RGB, out);
      fputc(p.r, out);
      fputc(p.g, out);
      fputc(p.b, out);
    } else {
      // Ecrire un QOI_OP_RGBA
      fputc(QOI_OP_RGBA, out);
      fputc(p.r, out);
      fputc(p.g, out);
      fputc(p.b, out);
      fputc(p.a, out);
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


