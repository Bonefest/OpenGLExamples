#include <iostream>
#include <cmath>

#include "Dependencies/tga.h"
#include "Dependencies/model.h"

const int WIDTH = 4096;
const int HEIGHT = 4096;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

void line(TGAImage& image, int x0, int y0, int x1, int y1, TGAColor color) {
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
    TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
    Model model("data/head.obj");
    std::cout << model.nfaces() << " " << model.nverts() << std::endl;
    for(int i = 0; i < model.nfaces(); ++i) {
        auto face = model.face(i);
        for(int j = 0;j < 3; ++j) {
            auto v1 = model.vert(face[j]);
            auto v2 = model.vert(face[(j + 1) % 3]);

            int x0 = (v1.x + 1.0f) * 0.5f * WIDTH;
            int y0 = (v1.y + 1.0f) * 0.5f * HEIGHT;

            int x1 = (v2.x + 1.0f) * 0.5f * WIDTH;
            int y1 = (v2.y + 1.0f) * 0.5f * HEIGHT;

            line(image, x0, y0, x1, y1, white);
        }

    }

    image.flip_vertically();
    image.write_tga_file("output.tga");
    return 0;
}
