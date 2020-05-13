#include <iostream>
#include <cmath>

#include "Dependencies/tga.h"
#include "Dependencies/model.h"

using std::min;
using std::max;

const int WIDTH = 1024;
const int HEIGHT = 1024;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

Vec3f barycentric(Vec3f v1, Vec3f v2, Vec3f v3, Vec3f p) {
    float denom = (v1.y - v3.y)*(v2.x - v3.x) + (v2.y - v3.y)*(v3.x - v1.x);

    float b1 = ((p.y - v3.y)*(v2.x - v3.x) + (v2.y - v3.y)*(v3.x - p.x)) / denom;

    float b2 = ((p.y - v1.y)*(v3.x - v1.x) + (v3.y - v1.y)*(v1.x - p.x)) / denom;

    float b3 = ((p.y - v2.y)*(v1.x - v2.x) + (v1.y - v2.y)*(v2.x - p.x)) / denom;

    return Vec3(b1, b2, b3);
}

void triangle(TGAImage& image, Vec3f v1, Vec3f v2, Vec3f v3, TGAColor color) {
    int x0 = min(min(v1.x, v2.x), v3.x);
    int xn = max(max(v1.x, v2.x), v3.x);

    int sy = min(min(v1.y, v2.y), v3.y);
    int my = max(max(v1.y, v2.y), v3.y);

    for(int y = sy;y <= my;y++) {

            TGAColor resColor = TGAColor(int(rand() * 255), int(rand() * 255), int(rand() * 255), 255);
        for(int x = x0; x <= xn; x++) {
            Vec3f bar = barycentric(v1, v2, v3, Vec3f(x, y, 0));
            //std::cout << bar.x << " " << bar.y << " " << bar.z << "\n";
            if(bar.x < 0 || bar.x > 1 || bar.y < 0 || bar.y > 1 || bar.z < 0 || bar.z > 1) {
                continue;
            }

            //TGAColor resColor = TGAColor(int(color.r * bar.x), int(color.g * bar.y), int(color.b * bar.z), 255);
            float h = int((float(y) / HEIGHT) * 255);
            image.set(x, y, resColor);
        }
    }
}

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

Vec3f transformVector(Vec3f vec) {
    return Vec3f( (vec.x + 1.0f) * 0.5f * WIDTH,
                  (vec.y + 1.0f) * 0.5f * HEIGHT,
                  vec.z );
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

            //line(image, x0, y0, x1, y1, white);
        }
        triangle(image, transformVector(model.vert(face[0])),
                        transformVector(model.vert(face[1])),
                        transformVector(model.vert(face[2])), white);

    }

    //triangle(image, Vec2f(0.0f, 0.0), Vec2f(256.0f, 256.0f), Vec2f(512.0f, 300.0f), white);
    image.flip_vertically();
    image.write_tga_file("output.tga");
    return 0;
}
