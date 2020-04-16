#ifndef _HEIGHTMAP_H_
#define _HEIGHTMAP_H_

#include <cstdlib>
#include <vector>
#include <cmath>

class Noise {
public:
    Noise(int gridSize):_gridSize(gridSize) {
        for(int i = 0;i < gridSize; ++i) {
            _points.push_back(drand48());
        }
    }
    double eval(double x) {
        int mn = (int)std::floor(x) % _gridSize;
        int mx = (int)std::ceil(x)  % _gridSize;

        double t = x - mn;
        //t = std::pow(t, 5) * 6 - 15 * std::pow(t, 4) + 10 * std::pow(t, 3);
        t = (1.0 - std::cos(t * M_PI)) * 0.5f;

        return lerp(_points[mn], _points[mx], t);
    }

    std::vector<double> getGLCoords(double step, double bound, int* size) {
        std::vector<double> data;

        if(_points.empty()) return data;

        double x = 0.0;
        while(x < bound) {
            data.push_back(x / 10);
            data.push_back(eval(x) - 0.5f);

            x += step;
        }
        *size = int(bound / step);

        return data;
    }

private:
    double lerp(double a, double b, double t) {
        return a*(1-t) + t*b;
    }

    int _gridSize;
    std::vector<double> _points;
};

class Noise3D {
public:
    Noise3D(int gridSize) {
        for(int y = 0; y < gridSize; y++) {
            _grid.push_back(std::vector<double>());
            for(int x = 0; x < gridSize; x++) {
                _grid.back().push_back(drand48());
            }
        }

        _gridSize = gridSize;
    }

    double eval(double x, double y) {
        int mnx = (int)std::floor(x) %  _gridSize;
        int mxx = (int)std::ceil(x)  %  _gridSize;
        int mny = (int)std::floor(y) %  _gridSize;
        int mxy = (int)std::ceil(y)  %  _gridSize;

        double tx = smooth(x - mnx);
        double ty = smooth(y - mny);

        return blerp(_grid[mny][mnx], _grid[mny][mxx],
                     _grid[mxy][mnx], _grid[mxy][mxx],
                     tx, ty);
    }

    std::vector<double> getGLCoords(double step, double bound, int* size) {
        if(_grid.empty()) return std::vector<double>();


        *size =  bound / step;

        std::vector<double> _result;
        for(double stepz = 0.0; stepz < bound; stepz += step) {
            for(double stepx = 0.0; stepx < bound; stepx += step) {
                _result.push_back(stepx - bound / 2);
                _result.push_back(eval(stepx, stepz));
                _result.push_back(-stepz - bound / 2);
            }
        }

        return _result;
    }

    std::vector<unsigned int> getIndecies(int size) {
        std::vector<unsigned int> _result;
        for(int y = 0; y < size - 1; y++) {
            for(int x = 0; x < size - 1; x++) {
                _result.push_back(x + y*size);
                _result.push_back(x + y*size + 1);
                _result.push_back(x + (y + 1)*size + 1);

                _result.push_back(x + (y + 1)*size + 1);
                _result.push_back(x + (y + 1)*size);
                _result.push_back(x + y*size);
            }
        }

        return _result;
    }

private:
    double smooth(double t) {
        return (1 - std::cos(t * M_PI)) * 0.5;
    }

    double blerp(double c00, double c01,
                 double c10, double c11,
                 double tx, double ty) {
        double a = c00*(1 - tx) + tx*c01;
        double b = c10*(1 - tx) + tx*c11;

        return (1-ty)*a + ty*b;
    }

    int _gridSize;
    std::vector<std::vector<double>> _grid;
};

#endif // _HEIGHTMAP_H_
