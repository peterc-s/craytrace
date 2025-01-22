#include <stdio.h>
#include <stdint.h>

#include "vec3.h"
#include "colour.h"

// 16 KiB IO buffer
#define IO_BUFSIZ 1024 * 16

int main(void) {
    char stdout_buf[IO_BUFSIZ];
    setvbuf(stdout, stdout_buf, _IOFBF, IO_BUFSIZ);

    uint16_t image_width = 256;
    uint16_t image_height = 256;

    printf("P3\n%d %d\n255\n", image_width, image_height);

    for (uint16_t i = 0; i < image_height; ++i) {
        fprintf(stderr, "\rLines done: %d / %d", i + 1, image_height);
        for (uint16_t j = 0; j < image_width; ++j) {
            Colour pixel_colour = vec3_with((double)j / (image_width - 1), \
                                            (double)i / (image_height - 1), \
                                            0.0);

            write_colour(stdout, &pixel_colour);
        }
    }

    fprintf(stderr, "\nDone.\n");
    fflush(stdout);
}
