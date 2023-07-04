#include"Fonction.h"

// Fonction pour encoder une image en QOI
int qoi_encode(pixel *pixels, int width, int height, uint8_t channels, uint8_t colorspace, FILE *out) {
  qoi_header header = {QOI_MAGIC, width, height, channels, colorspace};

  if (fwrite(&header, sizeof(header), 1, out) != 1) {
    return -1; // Erreur d'écriture de l'en-tête
  }

  // Initialisation des pixels précédents et du tableau d'index
  pixel prev = {0, 0, 0, 255};
  pixel index_array[64] = {0};

  for (int i = 0; i < width * height; i++) {
    pixel p = pixels[i];
    uint8_t index = (p.r * 3 + p.g * 5 + p.b * 7 + p.a * 11) % 64;

    if (memcmp(&p, &index_array[index], sizeof(pixel)) == 0) {
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


// Fonction pour décoder une image QOI
void decode_qoi(FILE *input, FILE *output) {
    qoi_header header;
    pixel_array array;
    // TODO: Lire le header du fichier d'entrée
    // TODO: Parcourir le fichier d'entrée et lire chaque pixel en utilisant le format QOI
    // TODO: Écrire l'image décodée dans le fichier de sortie
}

// Fonction pour tester l'encodage et le décodage QOI
void test_qoi() {
    FILE *test_file = fopen("test_output.qoi", "wb");

    // Créez un pixel pour le test
    pixel test_pixel;
    test_pixel.r = 123;
    test_pixel.g = 45;
    test_pixel.b = 67;
    test_pixel.a = 255;

    // Créez un tableau de pixels et le remplir avec le pixel de test
    pixel test_pixels[4];
    for (int i = 0; i < 4; i++) {
        test_pixels[i] = test_pixel;
    }

    // Test de l'encodage
    int result = qoi_encode(test_pixels, 2, 2, 4, 0, test_file);
    fclose(test_file);

    // Vérifiez que l'encodage a réussi
    if (result == 0) {
        printf("Test d'encodage QOI réussi\n");
    } else {
        printf("Test d'encodage QOI échoué\n");
    }
}