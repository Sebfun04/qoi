/*
#include <stdio.h>
#include <stdint.h>

typedef struct {
  uint8_t r, g, b, a;
} pixel_t;

void print_index_array(pixel_t index_array[]) {
  for (int i = 0; i < 64; i++) {
    printf("Index %d: r=%d, g=%d, b=%d, a=%d\n", i, index_array[i].r, index_array[i].g, index_array[i].b, index_array[i].a);
  }
}

int main() {
  pixel_t pixels[] = {
    {10, 20, 30, 40},
    {50, 60, 70, 80},
    {90, 100, 110, 120},
    {130, 140, 150, 160},
    {170, 180, 190, 200}
  };
  pixel_t index_array[64] = {0};

  for (int i = 0; i < sizeof(pixels) / sizeof(pixels[0]); i++) {
    uint8_t index = (pixels[i].r * 3 + pixels[i].g * 5 + pixels[i].b * 7 + pixels[i].a * 11) % 64;
    index_array[index] = pixels[i];
  }

  print_index_array(index_array);

  return 0;
}
*/