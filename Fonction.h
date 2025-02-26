#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Définition des constantes pour les opérations QOI
#define QOI_MAGIC "qoif"
#define QOI_OP_RGB 0xFE
#define QOI_OP_RGBA 0xFF
#define QOI_OP_INDEX 0x00
#define QOI_OP_DIFF 0x40
#define QOI_OP_LUMA 0x80
#define QOI_OP_RUN 0xC0

// Structure pour le header QOI
typedef struct {
    char magic[4]; // Les 4 premiers octets sont toujours "qoif"
    uint32_t width; // Largeur de l'image en pixels
    uint32_t height; // Hauteur de l'image en pixels
    uint8_t channels; // Nombre de canaux de couleur (3 pour RGB, 4 pour RGBA)
    uint8_t colorspace; // Espace colorimétrique (0 pour sRGB avec alpha linéaire, 1 pour tous les canaux linéaires)
} qoi_header;

// Structure pour un pixel
typedef struct {
    uint8_t r; // Valeur du canal rouge
    uint8_t g; // Valeur du canal vert
    uint8_t b; // Valeur du canal bleu
    uint8_t a; // Valeur du canal alpha
} pixel_t;

typedef struct {
    uint8_t tag;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} qoi_op_rgb_t;

// Structure pour un tableau de pixels
typedef struct {
    pixel_t pixels[64]; // Tableau de pixels
} pixel_array_t;


int qoi_encode(pixel_t *pixels, uint32_t width, uint32_t height, uint8_t channels, uint8_t colorspace, FILE *out);
void decode_qoi(FILE *input, FILE *output);
void print_binary_file(const char* filename);
