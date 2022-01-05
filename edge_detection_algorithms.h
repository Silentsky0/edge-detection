#pragma once

#include "edge_detection_auxiliary.h"
#include "edge_detection_masks.h"

uint8_t calculate_mask(uint8_t** input, size_t x, size_t y, struct Mask mask) {
    mask.mask_radius = (size_t)(mask.mask_size / 2);
    long long sum = 0;
    
    
    //printf("%d %d image\n", x, y);
    for (size_t i = y - mask.mask_radius; i <= y + mask.mask_radius; ++i) {
        for (size_t j = x - mask.mask_radius; j <= x + mask.mask_radius; ++j) {
            //printf("%d ", input[i][j]);
        }
        //printf("\n");
    }

    //printf("values\n");
    for (size_t i = y - mask.mask_radius; i <= y + mask.mask_radius; ++i) {
        for (size_t j = x - mask.mask_radius; j <= x + mask.mask_radius; ++j) {

            uint8_t image_val = input[i][j];
            
            int mask_val = mask.mask[i - y + mask.mask_radius][j - x + mask.mask_radius];

            sum += (long long)image_val * mask_val;

            //printf("%d ", (long long)image_val * mask_val);
        }
        //printf("\n");
    }
    //printf("suma %d\n", sum);
    //printf("uint8 %d\n", (uint8_t) sum);

    //printf("\n");

    //sum /= 1200.0;

    if (sum < 0) sum = 0;
    if (sum > 255) sum = 255;
    //printf("suma %d\n", sum);
    //printf("uint8 %d\n", (uint8_t)sum);
    //printf("\n");

    return (uint8_t)sum;
}

uint8_t calculate_gaussian_mask(uint8_t** input, size_t x, size_t y, struct Mask mask) {
    mask.mask_radius = (size_t)(mask.mask_size / 2);
    size_t sum = 0;

    for (size_t i = y - mask.mask_radius; i <= y + mask.mask_radius; ++i) {
        for (size_t j = x - mask.mask_radius; j <= x + mask.mask_radius; ++j) {

            uint8_t image_val = input[i][j];

            int mask_val = mask.mask[i - y + mask.mask_radius][j - x + mask.mask_radius];

            sum += (long long)image_val * mask_val;
        }
    }

    sum /= 159;

    if (sum < 0) sum = 0;
    if (sum > 255) sum = 255;
    return (uint8_t)sum;
}

void calculate_gradient_strength(struct ImageMatrix* mat, struct Mask mask) {
    uint8_t** gradient_strength = malloc(mat->height * sizeof(*gradient_strength));
    for (size_t i = 0; i < mat->height; i++) {
        gradient_strength[i] = malloc(mat->width * sizeof(gradient_strength[0]));
    }

    for (size_t i = mask.mask_radius; i < mat->height - mask.mask_radius - 1; i++) {
        for (size_t j = mask.mask_radius; j < mat->width - mask.mask_radius - 1; j++) {

            uint8_t pixel_value = calculate_mask(mat->matrix, j, i, mask);

            gradient_strength[i][j] = pixel_value;

        }
    }

    mat->matrix = gradient_strength;
}

void gaussian_blur(struct ImageMatrix* mat) {
    struct Masks mo = create_masks();
    struct Mask mask = mo.masks[0];



    if (mat->width > 0 && mat->height > 0) {
        uint8_t** gaussian_blur = malloc(mat->height * sizeof(*gaussian_blur));
        for (size_t i = 0; i < mat->height; i++) {
            gaussian_blur[i] = malloc(mat->width * sizeof(gaussian_blur[0]));
        }

        for (size_t i = mask.mask_radius; i < mat->height - mask.mask_radius; ++i) {
            for (size_t j = mask.mask_radius; j < mat->width - mask.mask_radius; ++j) {

                uint8_t pixel_value = calculate_gaussian_mask(mat->matrix, j, i, mask);

                gaussian_blur[i][j] = pixel_value;

            }
        }

        mat->matrix = gaussian_blur;
    }
}

void sobel_horizontal(struct ImageMatrix* mat) {
    struct Masks mo = create_masks();
    struct Mask mask = mo.masks[2];

    uint8_t** gradient_strength = malloc(mat->height * sizeof(*gradient_strength));
    for (size_t i = 0; i < mat->height; i++) {
        gradient_strength[i] = malloc(mat->width * sizeof(gradient_strength[0]));
    }

    for (size_t i = mask.mask_radius; i < mat->height - mask.mask_radius - 1; i++) {
        for (size_t j = mask.mask_radius; j < mat->width - mask.mask_radius - 1; j++) {

            uint8_t pixel_value = calculate_mask(mat->matrix, j, i, mask);

            gradient_strength[i][j] = pixel_value;

        }
    }

    mat->matrix = gradient_strength;
}

void sobel_vertical(struct ImageMatrix* mat) {
    struct Masks mo = create_masks();
    struct Mask mask = mo.masks[1];

    uint8_t** gradient_strength = malloc(mat->height * sizeof(*gradient_strength));
    for (size_t i = 0; i < mat->height; i++) {
        gradient_strength[i] = malloc(mat->width * sizeof(gradient_strength[0]));
    }

    for (size_t i = mask.mask_radius; i < mat->height - mask.mask_radius - 1; i++) {
        for (size_t j = mask.mask_radius; j < mat->width - mask.mask_radius - 1; j++) {

            uint8_t pixel_value = calculate_mask(mat->matrix, j, i, mask);

            gradient_strength[i][j] = pixel_value;

        }
    }

    mat->matrix = gradient_strength;
}

void detect_edges(struct ImageMatrix* mat) {

    uint8_t** output = malloc(mat->height * sizeof(*output));
    for (size_t i = 0; i < mat->height; i++) {
        output[i] = malloc(mat->width * sizeof(output[0]));
    }

    struct ImageMatrix gradient_vertical = copy_matrix(mat);
    struct ImageMatrix gradient_horizontal = copy_matrix(mat);

    sobel_vertical(&gradient_vertical);
    sobel_horizontal(&gradient_horizontal);

    

}