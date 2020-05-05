#include <iostream>
#include <cmath>

#include "Dependencies/tga.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

void line_one(TGAImage& image, int x0, int y0, int x1, int y1, TGAColor color) {
    for(float t = 0.0f; t < 1.0f; t += .01f) {
        int x = x0 + t * (x1 - x0);
        int y = y0 + t * (y1 - y0);
        image.set(x, y, color);
    }
}

void line_two(TGAImage& image, int x0, int y0, int x1, int y1, TGAColor color) {
    bool steer = false;
    if(std::abs(x1-x0) < std::abs(y1 - y0)) {
        std::swap(x0, y0);
        std::swap(x1, y1);

        steer = true;
    }

    if(x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    float k = float(y1 - y0)/float(x1 - x0);

    for(int x = x0; x <= x1; x++) {
        int y = int(std::round(y0 + k * (x - x0)));
        if(steer) {
            image.set(y, x, color);
        }
        else {
            image.set(x, y, color);
        }
    }
}

int main(int argc, char** argv) {
    TGAImage image(100, 100, TGAImage::RGB);

    int center_x = 50;
    int center_y = 50;

    int n = 4096;
    float angle_step = (2 * M_PI) / n;

    for(int i = 0;i < n;i++) {
        int x = std::cos(i * angle_step) * 20;
        int y = std::sin(i * angle_step) * 20;

        line_two(image, center_x, center_y, center_x + x, center_y + y, white);
    }

    image.flip_vertically();
    image.write_tga_file("output.tga");
    return 0;
}
