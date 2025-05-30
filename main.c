#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} BMPHeader;

typedef struct {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bpp;
    uint32_t compression;
    uint32_t image_size;
    int32_t x_ppm;
    int32_t y_ppm;
    uint32_t clr_used;
    uint32_t clr_important;
} DIBHeader;
#pragma pack(pop)

extern void scaledownhor(uint8_t* img, uint8_t* new_img, 
                         uint32_t width, uint32_t height,
                         uint32_t scale, uint32_t stride, uint32_t new_stride);

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Użycie: %s input.bmp output.bmp scale\n", argv[0]);
        return 1;
    }

    const char* input_filename = argv[1];
    const char* output_filename = argv[2];
    int scale = atoi(argv[3]);

    FILE* f = fopen(input_filename, "rb");
    if (!f) {
        perror("Błąd otwierania pliku wejściowego");
        return 1;
    }

    BMPHeader header;
    DIBHeader dib;
    fread(&header, sizeof(BMPHeader), 1, f);
    fread(&dib, sizeof(DIBHeader), 1, f);

    if (header.type != 0x4D42 || dib.bpp != 24 || dib.compression != 0) {
        fprintf(stderr, "Obsługiwane są tylko nieskompresowane BMP 24-bitowe.\n");
        fclose(f);
        return 1;
    }

    int width = dib.width;
    int height = dib.height;
    int stride = (width * 3 + 3) & ~3;
    int new_width = width / scale;
    int new_stride = (new_width * 3 + 3) & ~3;

    uint8_t* input_data = malloc(stride * height);
    uint8_t* output_data = calloc(new_stride * height, 1);

    fseek(f, header.offset, SEEK_SET);
    fread(input_data, stride, height, f);
    fclose(f);

    printf("Adres input_data: %p\n", input_data);
    printf("Adres output_data: %p\n", output_data);
    printf("width: %u, height: %u, scale: %u, stride: %u, new_stride: %u\n",
        width, height, scale, stride, new_stride);
    scaledownhor(input_data, output_data, width, height, scale, stride, new_stride);

    // Zaktualizuj nagłówki BMP
    header.size = sizeof(BMPHeader) + sizeof(DIBHeader) + new_stride * height;
    dib.width = new_width;
    dib.image_size = new_stride * height;

    FILE* out = fopen(output_filename, "wb");
    if (!out) {
        perror("Błąd otwierania pliku wyjściowego");
        return 1;
    }

    fwrite(&header, sizeof(BMPHeader), 1, out);
    fwrite(&dib, sizeof(DIBHeader), 1, out);
    fwrite(output_data, new_stride, height, out);
    fclose(out);

    free(input_data);
    free(output_data);
    return 0;
}
