#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)

extern void scaledownhor(void* src, void* dst, uint32_t width, 
                           uint32_t height, uint32_t input_stride, 
                           uint32_t output_stride, uint32_t scale);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <input.bmp> <output.bmp> <scale>\n", argv[0]);
        return 1;
    }

    int scale = atoi(argv[3]);
    if (scale <= 0) {
        printf("Scale must be a positive integer\n");
        return 1;
    }

    // Open input file
    FILE *in = fopen(argv[1], "rb");
    if (!in) {
        perror("Cannot open input file");
        return 1;
    }

    // Read headers
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    
    fread(&fileHeader, sizeof(fileHeader), 1, in);
    fread(&infoHeader, sizeof(infoHeader), 1, in);

    // Validate format
    if (fileHeader.bfType != 0x4D42 || infoHeader.biBitCount != 24) {
        printf("Only 24-bit BMP files are supported\n");
        fclose(in);
        return 1;
    }

    // Calculate image parameters
    int width = infoHeader.biWidth;
    int height = abs(infoHeader.biHeight);
    int input_stride = (width * 3 + 3) & ~3;

    // New dimensions after scaling
    int new_width = width / scale;  // Używamy zmiennej scale
    int output_stride = (new_width * 3 + 3) & ~3;

    // Allocate memory
    uint8_t *src_pixels = malloc(input_stride * height);
    uint8_t *dst_pixels = calloc(output_stride * height, 1); // Wypełnij zerami
    
    fseek(in, fileHeader.bfOffBits, SEEK_SET);
    fread(src_pixels, 1, input_stride * height, in);
    fclose(in);

    // Call scale funtion
    printf("DBG (C): w=%u h=%u in_str=%u out_str=%u scale=%u new_w=%u\n",
       width, height, input_stride, output_stride, scale, width/scale);
    printf("Calling ASM scaling function with scale=%d...\n", scale);
    scaledownhor(src_pixels, dst_pixels, width, height, 
                    input_stride, output_stride, scale);
    printf("Scaling completed.\n");

    // Update headers
    infoHeader.biWidth = new_width;
    infoHeader.biSizeImage = output_stride * height;
    fileHeader.bfSize = sizeof(fileHeader) + sizeof(infoHeader) + infoHeader.biSizeImage;

    // Save output file
    FILE *out = fopen(argv[2], "wb");
    fwrite(&fileHeader, sizeof(fileHeader), 1, out);
    fwrite(&infoHeader, sizeof(infoHeader), 1, out);
    fwrite(dst_pixels, 1, output_stride * height, out);
    fclose(out);

    free(src_pixels);
    free(dst_pixels);
    printf("Successfully created scaled image %s\n", argv[2]);
    return 0;
}