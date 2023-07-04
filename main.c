#include <stdio.h>
#include "Fonction.h"

int main(void) {
  // Création d'un tableau de pixels pour les tests
  pixel pixels[] = {
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

  // Appel de la fonction d'encodage QOI avec notre tableau de pixels
  if (qoi_encode(pixels, 8, 1, 4, 0, out) != 0) {
    printf("Erreur pendant l'encodage QOI\n");
    return 1;
  }

  // Fermeture du fichier
  fclose(out);

  printf("Encodage QOI terminé avec succès\n");
  return 0;
}