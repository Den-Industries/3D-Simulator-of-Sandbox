#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <Windows.h>
#include <iostream>
#include "StructsAndFunctions.h"
#include "LoadModel.h"
#include "RetroTerrain.h"
#include "Func.h"
#define Nothing 0
#define Water 1
#define Sand 2
#define Wall 3

using namespace sf; using namespace std;

int FPS[5], fpscounter = 0, CrabedModel = 7;
const int qofmodels = 2;
float cd[4] = { 0 }, timevall = 200;
bool work = true, showmode = false;;
RenderWindow window; Object Models[qofmodels];

const int mapsize = 25;

float mapvall = 200;

float blinkvall = 0.0006f;

struct Mat3d
{
    int m[mapsize][mapsize][mapsize] = { };
};

Mat3d themap;
Mat3d themawaterpbuf;
Mat3d emptymap;
Point cursorplace;

struct Player
{
public:
    Camera Cam;
    Player(Point a, float HA, float VA, float fov, float screenvall, float znear, float zfar)
    {
        Cam.Set(a, HA, VA, fov, screenvall, znear, zfar);
    }
};

Player p(Point(0, 0, 0), 0, 0, 90, ScreenSize[1] / ScreenSize[0], 0.1f, 20000);

Point lookingforwaterpath(Point place, int waterlvl)
{
    themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z)] = Wall;
    switch (rand() % 6)
    {
    case 0:
        if (int(place.x) - 1 >= 0)
            if (themawaterpbuf.m[int(place.x) - 1][int(place.y)][int(place.z)] == Nothing) return Point(place.x - 1, place.y, place.z); else
                if (themawaterpbuf.m[int(place.x) - 1][int(place.y)][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x) - 1, int(place.y), int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.x) + 1 < mapsize)
            if (themawaterpbuf.m[int(place.x) + 1][int(place.y)][int(place.z)] == Nothing) return Point(place.x + 1, place.y, place.z); else
                if (themawaterpbuf.m[int(place.x) + 1][int(place.y)][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x) + 1, int(place.y), int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.y) - 1 >= 0)
            if (themawaterpbuf.m[int(place.x)][int(place.y) - 1][int(place.z)] == Nothing) return Point(place.x, place.y - 1, place.z); else
                if (themawaterpbuf.m[int(place.x)][int(place.y) - 1][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y) - 1, int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.y) + 1 < waterlvl)
            if (themawaterpbuf.m[int(place.x)][int(place.y) + 1][int(place.z)] == Nothing) return Point(place.x, place.y + 1, place.z); else
                if (themawaterpbuf.m[int(place.x)][int(place.y) + 1][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y) + 1, int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.z) - 1 >= 0)
            if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) - 1] == Nothing) return Point(place.x, place.y, place.z - 1); else
                if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) - 1] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y), int(place.z) - 1), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.z) + 1 < mapsize)
            if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) + 1] == Nothing) return Point(place.x, place.y, place.z + 1); else
                if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) + 1] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y), int(place.z) + 1), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        break;
    case 1:
        if (int(place.x) + 1 < mapsize)
            if (themawaterpbuf.m[int(place.x) + 1][int(place.y)][int(place.z)] == Nothing) return Point(place.x + 1, place.y, place.z); else
                if (themawaterpbuf.m[int(place.x) + 1][int(place.y)][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x) + 1, int(place.y), int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.x) - 1 >= 0)
            if (themawaterpbuf.m[int(place.x) - 1][int(place.y)][int(place.z)] == Nothing) return Point(place.x - 1, place.y, place.z); else
                if (themawaterpbuf.m[int(place.x) - 1][int(place.y)][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x) - 1, int(place.y), int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.y) - 1 >= 0)
            if (themawaterpbuf.m[int(place.x)][int(place.y) - 1][int(place.z)] == Nothing) return Point(place.x, place.y - 1, place.z); else
                if (themawaterpbuf.m[int(place.x)][int(place.y) - 1][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y) - 1, int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.y) + 1 < waterlvl)
            if (themawaterpbuf.m[int(place.x)][int(place.y) + 1][int(place.z)] == Nothing) return Point(place.x, place.y + 1, place.z); else
                if (themawaterpbuf.m[int(place.x)][int(place.y) + 1][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y) + 1, int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }

        if (int(place.z) - 1 >= 0)
            if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) - 1] == Nothing) return Point(place.x, place.y, place.z - 1); else
                if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) - 1] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y), int(place.z) - 1), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.z) + 1 < mapsize)
            if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) + 1] == Nothing) return Point(place.x, place.y, place.z + 1); else
                if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) + 1] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y), int(place.z) + 1), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        break;
    case 2:
        if (int(place.y) - 1 >= 0)
            if (themawaterpbuf.m[int(place.x)][int(place.y) - 1][int(place.z)] == Nothing) return Point(place.x, place.y - 1, place.z); else
                if (themawaterpbuf.m[int(place.x)][int(place.y) - 1][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y) - 1, int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.x) - 1 >= 0)
            if (themawaterpbuf.m[int(place.x) - 1][int(place.y)][int(place.z)] == Nothing) return Point(place.x - 1, place.y, place.z); else
                if (themawaterpbuf.m[int(place.x) - 1][int(place.y)][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x) - 1, int(place.y), int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.x) + 1 < mapsize)
            if (themawaterpbuf.m[int(place.x) + 1][int(place.y)][int(place.z)] == Nothing) return Point(place.x + 1, place.y, place.z); else
                if (themawaterpbuf.m[int(place.x) + 1][int(place.y)][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x) + 1, int(place.y), int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.y) + 1 < waterlvl)
            if (themawaterpbuf.m[int(place.x)][int(place.y) + 1][int(place.z)] == Nothing) return Point(place.x, place.y + 1, place.z); else
                if (themawaterpbuf.m[int(place.x)][int(place.y) + 1][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y) + 1, int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.z) - 1 >= 0)
            if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) - 1] == Nothing) return Point(place.x, place.y, place.z - 1); else
                if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) - 1] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y), int(place.z) - 1), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.z) + 1 < mapsize)
            if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) + 1] == Nothing) return Point(place.x, place.y, place.z + 1); else
                if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) + 1] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y), int(place.z) + 1), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        break;
    case 3:
        if (int(place.y) + 1 < waterlvl)
            if (themawaterpbuf.m[int(place.x)][int(place.y) + 1][int(place.z)] == Nothing) return Point(place.x, place.y + 1, place.z); else
                if (themawaterpbuf.m[int(place.x)][int(place.y) + 1][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y) + 1, int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.x) - 1 >= 0)
            if (themawaterpbuf.m[int(place.x) - 1][int(place.y)][int(place.z)] == Nothing) return Point(place.x - 1, place.y, place.z); else
                if (themawaterpbuf.m[int(place.x) - 1][int(place.y)][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x) - 1, int(place.y), int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.x) + 1 < mapsize)
            if (themawaterpbuf.m[int(place.x) + 1][int(place.y)][int(place.z)] == Nothing) return Point(place.x + 1, place.y, place.z); else
                if (themawaterpbuf.m[int(place.x) + 1][int(place.y)][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x) + 1, int(place.y), int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.y) - 1 >= 0)
            if (themawaterpbuf.m[int(place.x)][int(place.y) - 1][int(place.z)] == Nothing) return Point(place.x, place.y - 1, place.z); else
                if (themawaterpbuf.m[int(place.x)][int(place.y) - 1][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y) - 1, int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.z) - 1 >= 0)
            if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) - 1] == Nothing) return Point(place.x, place.y, place.z - 1); else
                if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) - 1] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y), int(place.z) - 1), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.z) + 1 < mapsize)
            if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) + 1] == Nothing) return Point(place.x, place.y, place.z + 1); else
                if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) + 1] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y), int(place.z) + 1), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        break;
    case 4:
        if (int(place.z) - 1 >= 0)
            if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) - 1] == Nothing) return Point(place.x, place.y, place.z - 1); else
                if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) - 1] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y), int(place.z) - 1), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.x) - 1 >= 0)
            if (themawaterpbuf.m[int(place.x) - 1][int(place.y)][int(place.z)] == Nothing) return Point(place.x - 1, place.y, place.z); else
                if (themawaterpbuf.m[int(place.x) - 1][int(place.y)][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x) - 1, int(place.y), int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.x) + 1 < mapsize)
            if (themawaterpbuf.m[int(place.x) + 1][int(place.y)][int(place.z)] == Nothing) return Point(place.x + 1, place.y, place.z); else
                if (themawaterpbuf.m[int(place.x) + 1][int(place.y)][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x) + 1, int(place.y), int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.y) - 1 >= 0)
            if (themawaterpbuf.m[int(place.x)][int(place.y) - 1][int(place.z)] == Nothing) return Point(place.x, place.y - 1, place.z); else
                if (themawaterpbuf.m[int(place.x)][int(place.y) - 1][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y) - 1, int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.y) + 1 < waterlvl)
            if (themawaterpbuf.m[int(place.x)][int(place.y) + 1][int(place.z)] == Nothing) return Point(place.x, place.y + 1, place.z); else
                if (themawaterpbuf.m[int(place.x)][int(place.y) + 1][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y) + 1, int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
       if (int(place.z) + 1 < mapsize)
            if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) + 1] == Nothing) return Point(place.x, place.y, place.z + 1); else
                if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) + 1] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y), int(place.z) + 1), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        break;
    case 5:
        if (int(place.z) + 1 < mapsize)
            if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) + 1] == Nothing) return Point(place.x, place.y, place.z + 1); else
                if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) + 1] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y), int(place.z) + 1), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.x) - 1 >= 0)
            if (themawaterpbuf.m[int(place.x) - 1][int(place.y)][int(place.z)] == Nothing) return Point(place.x - 1, place.y, place.z); else
                if (themawaterpbuf.m[int(place.x) - 1][int(place.y)][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x) - 1, int(place.y), int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.x) + 1 < mapsize)
            if (themawaterpbuf.m[int(place.x) + 1][int(place.y)][int(place.z)] == Nothing) return Point(place.x + 1, place.y, place.z); else
                if (themawaterpbuf.m[int(place.x) + 1][int(place.y)][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x) + 1, int(place.y), int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.y) - 1 >= 0)
            if (themawaterpbuf.m[int(place.x)][int(place.y) - 1][int(place.z)] == Nothing) return Point(place.x, place.y - 1, place.z); else
                if (themawaterpbuf.m[int(place.x)][int(place.y) - 1][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y) - 1, int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.y) + 1 < waterlvl)
            if (themawaterpbuf.m[int(place.x)][int(place.y) + 1][int(place.z)] == Nothing) return Point(place.x, place.y + 1, place.z); else
                if (themawaterpbuf.m[int(place.x)][int(place.y) + 1][int(place.z)] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y) + 1, int(place.z)), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        if (int(place.z) - 1 >= 0)
            if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) - 1] == Nothing) return Point(place.x, place.y, place.z - 1); else
                if (themawaterpbuf.m[int(place.x)][int(place.y)][int(place.z) - 1] == Water)
                {
                    Point mbpath = lookingforwaterpath(Point(int(place.x), int(place.y), int(place.z) - 1), waterlvl); if (mbpath.x != -1) return mbpath;
                }
        
        break;
    }

    return Point(-1, -1, -1);
}

void maplog()
{
    for (int x = 0; x < mapsize; x++)
        for (int y = 0; y < mapsize; y++)
            for (int z = 0; z < mapsize; z++)
            {
                if (themap.m[x][y][z] == Water)
                    if (y - 1 >= 0)
                    {
                        bool already = false;
                        if (themap.m[x][y - 1][z] == Nothing && !already)
                        {
                            themap.m[x][y][z] = Nothing;
                            themap.m[x][y - 1][z] = Water; already = true;
                        }
                        if (themap.m[x][y - 1][z] == Water && !already)
                        {
                            themawaterpbuf = themap;
                            Point path = lookingforwaterpath(Point(x, y - 1, z), y);
                            if (path.x != -1)
                            {
                                themap.m[x][y][z] = Nothing;
                                themap.m[int(path.x)][int(path.y)][int(path.z)] = Water; already = true;
                            }
                        }
                        if (themap.m[x][y - 1][z] == Wall || themap.m[x][y - 1][z] == Sand && !already)
                        {
                            switch (rand() % 4)
                            {
                            case 0:
                                if ((themap.m[x - 1][y - 1][z] == Nothing) && themap.m[x - 1][y][z] == Nothing && !already && x - 1 >= 0)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x - 1][y - 1][z] = Water; already = true;
                                }
                                if ((themap.m[x - 1][y - 1][z] == Water) && themap.m[x - 1][y][z] == Nothing && !already && x - 1 >= 0)
                                {
                                    themawaterpbuf = themap; Point path = lookingforwaterpath(Point(x - 1, y - 1, z), y);
                                    if (path.x != -1)
                                    {
                                        themap.m[x][y][z] = Nothing; themap.m[int(path.x)][int(path.y)][int(path.z)] = Water; already = true;
                                    }
                                }

                                if ((themap.m[x + 1][y - 1][z] == Nothing) && themap.m[x + 1][y][z] == Nothing && !already && x + 1 < mapsize)
                                {
                                    themap.m[x][y][z] = themap.m[x + 1][y - 1][z]; themap.m[x + 1][y - 1][z] = Water; already = true;
                                }
                                if ((themap.m[x + 1][y - 1][z] == Water) && themap.m[x + 1][y][z] == Nothing && !already && x + 1 < mapsize)
                                {
                                    themawaterpbuf = themap; Point path = lookingforwaterpath(Point(x + 1, y - 1, z), y);
                                    if (path.x != -1)
                                    {
                                        themap.m[x][y][z] = Nothing; themap.m[int(path.x)][int(path.y)][int(path.z)] = Water; already = true;
                                    }
                                }

                                if ((themap.m[x][y - 1][z - 1] == Nothing) && themap.m[x][y][z - 1] == Nothing && !already && z - 1 >= 0)
                                {
                                    themap.m[x][y][z] = themap.m[x][y - 1][z - 1]; themap.m[x][y - 1][z - 1] = Water; already = true;
                                }
                                if ((themap.m[x][y - 1][z - 1] == Water) && themap.m[x][y][z - 1] == Nothing && !already && z - 1 >= 0)
                                {
                                    themawaterpbuf = themap; Point path = lookingforwaterpath(Point(x, y - 1, z - 1), y);
                                    if (path.x != -1)
                                    {
                                        themap.m[x][y][z] = Nothing; themap.m[int(path.x)][int(path.y)][int(path.z)] = Water; already = true;
                                    }
                                }

                                if ((themap.m[x][y - 1][z + 1] == Nothing) && themap.m[x][y][z + 1] == Nothing && !already && z + 1 < mapsize)
                                {
                                    themap.m[x][y][z] = themap.m[x][y - 1][z + 1];themap.m[x][y - 1][z + 1] = Water; already = true;
                                }
                                if ((themap.m[x][y - 1][z + 1] == Water) && themap.m[x][y][z + 1] == Nothing && !already && z + 1 < mapsize)
                                {
                                    themawaterpbuf = themap;Point path = lookingforwaterpath(Point(x, y - 1, z + 1), y); 
                                    if (path.x != -1)
                                    {
                                        themap.m[x][y][z] = Nothing; themap.m[int(path.x)][int(path.y)][int(path.z)] = Water; already = true;
                                    }
                                }
                                break;
                            case 1:
                                if ((themap.m[x + 1][y - 1][z] == Nothing) && themap.m[x + 1][y][z] == Nothing && !already && x + 1 < mapsize)
                                {
                                    themap.m[x][y][z] = themap.m[x + 1][y - 1][z]; themap.m[x + 1][y - 1][z] = Water; already = true;
                                }
                                if ((themap.m[x + 1][y - 1][z] == Water) && themap.m[x + 1][y][z] == Nothing && !already && x + 1 < mapsize)
                                {
                                    themawaterpbuf = themap; Point path = lookingforwaterpath(Point(x + 1, y - 1, z), y);
                                    if (path.x != -1)
                                    {
                                        themap.m[x][y][z] = Nothing; themap.m[int(path.x)][int(path.y)][int(path.z)] = Water; already = true;
                                    }
                                }

                                if ((themap.m[x - 1][y - 1][z] == Nothing) && themap.m[x - 1][y][z] == Nothing && !already && x - 1 >= 0)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x - 1][y - 1][z] = Water; already = true;
                                }
                                if ((themap.m[x - 1][y - 1][z] == Water) && themap.m[x - 1][y][z] == Nothing && !already && x - 1 >= 0)
                                {
                                    themawaterpbuf = themap; Point path = lookingforwaterpath(Point(x - 1, y - 1, z), y);
                                    if (path.x != -1)
                                    {
                                        themap.m[x][y][z] = Nothing; themap.m[int(path.x)][int(path.y)][int(path.z)] = Water; already = true;
                                    }
                                }

                                if ((themap.m[x][y - 1][z - 1] == Nothing) && themap.m[x][y][z - 1] == Nothing && !already && z - 1 >= 0)
                                {
                                    themap.m[x][y][z] = themap.m[x][y - 1][z - 1]; themap.m[x][y - 1][z - 1] = Water; already = true;
                                }
                                if ((themap.m[x][y - 1][z - 1] == Water) && themap.m[x][y][z - 1] == Nothing && !already && z - 1 >= 0)
                                {
                                    themawaterpbuf = themap; Point path = lookingforwaterpath(Point(x, y - 1, z - 1), y);
                                    if (path.x != -1)
                                    {
                                        themap.m[x][y][z] = Nothing; themap.m[int(path.x)][int(path.y)][int(path.z)] = Water; already = true;
                                    }
                                }

                                if ((themap.m[x][y - 1][z + 1] == Nothing) && themap.m[x][y][z + 1] == Nothing && !already && z + 1 < mapsize)
                                {
                                    themap.m[x][y][z] = themap.m[x][y - 1][z + 1]; themap.m[x][y - 1][z + 1] = Water; already = true;
                                }
                                if ((themap.m[x][y - 1][z + 1] == Water) && themap.m[x][y][z + 1] == Nothing && !already && z + 1 < mapsize)
                                {
                                    themawaterpbuf = themap; Point path = lookingforwaterpath(Point(x, y - 1, z + 1), y);
                                    if (path.x != -1)
                                    {
                                        themap.m[x][y][z] = Nothing; themap.m[int(path.x)][int(path.y)][int(path.z)] = Water; already = true;
                                    }
                                }
                                break;
                            case 2:
                                if ((themap.m[x][y - 1][z - 1] == Nothing) && themap.m[x][y][z - 1] == Nothing && !already && z - 1 >= 0)
                                {
                                    themap.m[x][y][z] = themap.m[x][y - 1][z - 1]; themap.m[x][y - 1][z - 1] = Water; already = true;
                                }
                                if ((themap.m[x][y - 1][z - 1] == Water) && themap.m[x][y][z - 1] == Nothing && !already && z - 1 >= 0)
                                {
                                    themawaterpbuf = themap; Point path = lookingforwaterpath(Point(x, y - 1, z - 1), y);
                                    if (path.x != -1)
                                    {
                                        themap.m[x][y][z] = Nothing; themap.m[int(path.x)][int(path.y)][int(path.z)] = Water; already = true;
                                    }
                                }

                                if ((themap.m[x - 1][y - 1][z] == Nothing) && themap.m[x - 1][y][z] == Nothing && !already && x - 1 >= 0)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x - 1][y - 1][z] = Water; already = true;
                                }
                                if ((themap.m[x - 1][y - 1][z] == Water) && themap.m[x - 1][y][z] == Nothing && !already && x - 1 >= 0)
                                {
                                    themawaterpbuf = themap; Point path = lookingforwaterpath(Point(x - 1, y - 1, z), y);
                                    if (path.x != -1)
                                    {
                                        themap.m[x][y][z] = Nothing; themap.m[int(path.x)][int(path.y)][int(path.z)] = Water; already = true;
                                    }
                                }

                                if ((themap.m[x + 1][y - 1][z] == Nothing) && themap.m[x + 1][y][z] == Nothing && !already && x + 1 < mapsize)
                                {
                                    themap.m[x][y][z] = themap.m[x + 1][y - 1][z]; themap.m[x + 1][y - 1][z] = Water; already = true;
                                }
                                if ((themap.m[x + 1][y - 1][z] == Water) && themap.m[x + 1][y][z] == Nothing && !already && x + 1 < mapsize)
                                {
                                    themawaterpbuf = themap; Point path = lookingforwaterpath(Point(x + 1, y - 1, z), y);
                                    if (path.x != -1)
                                    {
                                        themap.m[x][y][z] = Nothing; themap.m[int(path.x)][int(path.y)][int(path.z)] = Water; already = true;
                                    }
                                }

                                if ((themap.m[x][y - 1][z + 1] == Nothing) && themap.m[x][y][z + 1] == Nothing && !already && z + 1 < mapsize)
                                {
                                    themap.m[x][y][z] = themap.m[x][y - 1][z + 1]; themap.m[x][y - 1][z + 1] = Water; already = true;
                                }
                                if ((themap.m[x][y - 1][z + 1] == Water) && themap.m[x][y][z + 1] == Nothing && !already && z + 1 < mapsize)
                                {
                                    themawaterpbuf = themap; Point path = lookingforwaterpath(Point(x, y - 1, z + 1), y);
                                    if (path.x != -1)
                                    {
                                        themap.m[x][y][z] = Nothing; themap.m[int(path.x)][int(path.y)][int(path.z)] = Water; already = true;
                                    }
                                }
                                break;
                            case 3:
                                if ((themap.m[x][y - 1][z + 1] == Nothing) && themap.m[x][y][z + 1] == Nothing && !already && z + 1 < mapsize)
                                {
                                    themap.m[x][y][z] = themap.m[x][y - 1][z + 1]; themap.m[x][y - 1][z + 1] = Water; already = true;
                                }
                                if ((themap.m[x][y - 1][z + 1] == Water) && themap.m[x][y][z + 1] == Nothing && !already && z + 1 < mapsize)
                                {
                                    themawaterpbuf = themap; Point path = lookingforwaterpath(Point(x, y - 1, z + 1), y);
                                    if (path.x != -1)
                                    {
                                        themap.m[x][y][z] = Nothing; themap.m[int(path.x)][int(path.y)][int(path.z)] = Water; already = true;
                                    }
                                }

                                if ((themap.m[x - 1][y - 1][z] == Nothing) && themap.m[x - 1][y][z] == Nothing && !already && x - 1 >= 0)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x - 1][y - 1][z] = Water; already = true;
                                }
                                if ((themap.m[x - 1][y - 1][z] == Water) && themap.m[x - 1][y][z] == Nothing && !already && x - 1 >= 0)
                                {
                                    themawaterpbuf = themap; Point path = lookingforwaterpath(Point(x - 1, y - 1, z), y);
                                    if (path.x != -1)
                                    {
                                        themap.m[x][y][z] = Nothing; themap.m[int(path.x)][int(path.y)][int(path.z)] = Water; already = true;
                                    }
                                }

                                if ((themap.m[x + 1][y - 1][z] == Nothing) && themap.m[x + 1][y][z] == Nothing && !already && x + 1 < mapsize)
                                {
                                    themap.m[x][y][z] = themap.m[x + 1][y - 1][z]; themap.m[x + 1][y - 1][z] = Water; already = true;
                                }
                                if ((themap.m[x + 1][y - 1][z] == Water) && themap.m[x + 1][y][z] == Nothing && !already && x + 1 < mapsize)
                                {
                                    themawaterpbuf = themap; Point path = lookingforwaterpath(Point(x + 1, y - 1, z), y);
                                    if (path.x != -1)
                                    {
                                        themap.m[x][y][z] = Nothing; themap.m[int(path.x)][int(path.y)][int(path.z)] = Water; already = true;
                                    }
                                }

                                if ((themap.m[x][y - 1][z - 1] == Nothing) && themap.m[x][y][z - 1] == Nothing && !already && z - 1 >= 0)
                                {
                                    themap.m[x][y][z] = themap.m[x][y - 1][z - 1]; themap.m[x][y - 1][z - 1] = Water; already = true;
                                }
                                if ((themap.m[x][y - 1][z - 1] == Water) && themap.m[x][y][z - 1] == Nothing && !already && z - 1 >= 0)
                                {
                                    themawaterpbuf = themap; Point path = lookingforwaterpath(Point(x, y - 1, z - 1), y);
                                    if (path.x != -1)
                                    {
                                        themap.m[x][y][z] = Nothing; themap.m[int(path.x)][int(path.y)][int(path.z)] = Water; already = true;
                                    }
                                }
                                break;
                            }
                        }
                    }
                if (themap.m[x][y][z] == Sand)
                    if (y - 1 >= 0)
                    {
                        bool already = false;
                        if (themap.m[x][y - 1][z] == Nothing && !already)
                        {
                            themap.m[x][y][z] = Nothing;
                            themap.m[x][y - 1][z] = Sand; already = true;
                        }
                        if (themap.m[x][y - 1][z] == Water && !already)
                        {
                            themap.m[x][y][z] = Water;
                            themap.m[x][y - 1][z] = Sand; already = true;
                        }
                        if (themap.m[x][y - 1][z] == Wall || themap.m[x][y - 1][z] == Sand && !already)
                        {
                            switch (rand() % 4)
                            {
                            case 0:
                                if (themap.m[x - 1][y - 1][z] == Nothing && (themap.m[x - 1][y][z] == Nothing || themap.m[x - 1][y][z] == Water) && !already && x - 1 >= 0)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x - 1][y - 1][z] = Sand; already = true;
                                }
                                if (themap.m[x - 1][y - 1][z] == Water && (themap.m[x - 1][y][z] == Nothing || themap.m[x - 1][y][z] == Water) && !already && x - 1 >= 0)
                                {
                                    if (themap.m[x - 1][y][z] == Nothing) themap.m[x - 1][y][z] = Water;
                                    else
                                    {
                                        themawaterpbuf = themap;
                                        Point path = lookingforwaterpath(Point(x - 1, y, z), y);
                                        if (path.x != -1)
                                        {
                                            themap.m[int(path.x)][int(path.y)][int(path.z)] = Water;
                                        }
                                    }
                                    themap.m[x][y][z] = Nothing; themap.m[x - 1][y - 1][z] = Sand; already = true;
                                }

                                if (themap.m[x + 1][y - 1][z] == Nothing && (themap.m[x + 1][y][z] == Nothing || themap.m[x + 1][y][z] == Water) && !already && x + 1 < mapsize)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x + 1][y - 1][z] = Sand; already = true;
                                }
                                if (themap.m[x + 1][y - 1][z] == Water && (themap.m[x + 1][y][z] == Nothing || themap.m[x + 1][y][z] == Water) && !already && x + 1 < mapsize)
                                {
                                    if (themap.m[x + 1][y][z] == Nothing) themap.m[x + 1][y][z] = Water;
                                    else
                                    {
                                        themawaterpbuf = themap;
                                        Point path = lookingforwaterpath(Point(x + 1, y, z), y);
                                        if (path.x != -1)
                                        {
                                            themap.m[int(path.x)][int(path.y)][int(path.z)] = Water;
                                        }
                                    }
                                    themap.m[x][y][z] = Nothing; themap.m[x + 1][y - 1][z] = Sand; already = true;
                                }

                                if (themap.m[x][y - 1][z - 1] == Nothing && (themap.m[x][y][z - 1] == Nothing || themap.m[x][y][z - 1] == Water) && !already && z - 1 >= 0)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x][y - 1][z - 1] = Sand; already = true;
                                }
                                if (themap.m[x][y - 1][z - 1] == Water && (themap.m[x][y][z - 1] == Nothing || themap.m[x][y][z - 1] == Water) && !already && z - 1 >= 0)
                                {
                                    if (themap.m[x][y][z - 1] == Nothing) themap.m[x][y][z - 1] = Water;
                                    else
                                    {
                                        themawaterpbuf = themap;
                                        Point path = lookingforwaterpath(Point(x, y, z - 1), y);
                                        if (path.x != -1)
                                        {
                                            themap.m[int(path.x)][int(path.y)][int(path.z)] = Water;
                                        }
                                    }
                                    themap.m[x][y][z] = Nothing; themap.m[x][y - 1][z - 1] = Sand; already = true;
                                }

                                if (themap.m[x][y - 1][z + 1] == Nothing && (themap.m[x][y][z + 1] == Nothing || themap.m[x][y][z + 1] == Water) && !already && z + 1 < mapsize)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x][y - 1][z + 1] = Sand; already = true;
                                }
                                if (themap.m[x][y - 1][z + 1] == Water && (themap.m[x][y][z + 1] == Nothing || themap.m[x][y][z + 1] == Water) && !already && z + 1 < mapsize)
                                {
                                    if (themap.m[x][y][z + 1] == Nothing) themap.m[x][y][z + 1] = Water;
                                    else
                                    {
                                        themawaterpbuf = themap;
                                        Point path = lookingforwaterpath(Point(x, y, z + 1), y);
                                        if (path.x != -1)
                                        {
                                            themap.m[int(path.x)][int(path.y)][int(path.z)] = Water;
                                        }
                                    }
                                    themap.m[x][y][z] = Nothing; themap.m[x][y][z + 1] = Water; themap.m[x][y - 1][z + 1] = Sand; already = true;
                                }
                                break;
                            case 1:
                                if (themap.m[x + 1][y - 1][z] == Nothing && (themap.m[x + 1][y][z] == Nothing || themap.m[x + 1][y][z] == Water) && !already && x + 1 < mapsize)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x + 1][y - 1][z] = Sand; already = true;
                                }
                                if (themap.m[x + 1][y - 1][z] == Water && (themap.m[x + 1][y][z] == Nothing || themap.m[x + 1][y][z] == Water) && !already && x + 1 < mapsize)
                                {
                                    if (themap.m[x + 1][y][z] == Nothing) themap.m[x + 1][y][z] = Water;
                                    else
                                    {
                                        themawaterpbuf = themap;
                                        Point path = lookingforwaterpath(Point(x + 1, y, z), y);
                                        if (path.x != -1)
                                        {
                                            themap.m[int(path.x)][int(path.y)][int(path.z)] = Water;
                                        }
                                    }
                                    themap.m[x][y][z] = Nothing; themap.m[x + 1][y - 1][z] = Sand; already = true;
                                }

                                if (themap.m[x - 1][y - 1][z] == Nothing && (themap.m[x - 1][y][z] == Nothing || themap.m[x - 1][y][z] == Water) && !already && x - 1 >= 0)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x - 1][y - 1][z] = Sand; already = true;
                                }
                                if (themap.m[x - 1][y - 1][z] == Water && (themap.m[x - 1][y][z] == Nothing || themap.m[x - 1][y][z] == Water) && !already && x - 1 >= 0)
                                {
                                    if (themap.m[x - 1][y][z] == Nothing) themap.m[x - 1][y][z] = Water;
                                    else
                                    {
                                        themawaterpbuf = themap;
                                        Point path = lookingforwaterpath(Point(x - 1, y, z), y);
                                        if (path.x != -1)
                                        {
                                            themap.m[int(path.x)][int(path.y)][int(path.z)] = Water;
                                        }
                                    }
                                    themap.m[x][y][z] = Nothing; themap.m[x - 1][y - 1][z] = Sand; already = true;
                                }

                                if (themap.m[x][y - 1][z - 1] == Nothing && (themap.m[x][y][z - 1] == Nothing || themap.m[x][y][z - 1] == Water) && !already && z - 1 >= 0)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x][y - 1][z - 1] = Sand; already = true;
                                }
                                if (themap.m[x][y - 1][z - 1] == Water && (themap.m[x][y][z - 1] == Nothing || themap.m[x][y][z - 1] == Water) && !already && z - 1 >= 0)
                                {
                                    if (themap.m[x][y][z - 1] == Nothing) themap.m[x][y][z - 1] = Water;
                                    else
                                    {
                                        themawaterpbuf = themap;
                                        Point path = lookingforwaterpath(Point(x, y, z - 1), y);
                                        if (path.x != -1)
                                        {
                                            themap.m[int(path.x)][int(path.y)][int(path.z)] = Water;
                                        }
                                    }
                                    themap.m[x][y][z] = Nothing; themap.m[x][y - 1][z - 1] = Sand; already = true;
                                }

                                if (themap.m[x][y - 1][z + 1] == Nothing && (themap.m[x][y][z + 1] == Nothing || themap.m[x][y][z + 1] == Water) && !already && z + 1 < mapsize)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x][y - 1][z + 1] = Sand; already = true;
                                }
                                if (themap.m[x][y - 1][z + 1] == Water && (themap.m[x][y][z + 1] == Nothing || themap.m[x][y][z + 1] == Water) && !already && z + 1 < mapsize)
                                {
                                    if (themap.m[x][y][z + 1] == Nothing) themap.m[x][y][z + 1] = Water;
                                    else
                                    {
                                        themawaterpbuf = themap;
                                        Point path = lookingforwaterpath(Point(x, y, z + 1), y);
                                        if (path.x != -1)
                                        {
                                            themap.m[int(path.x)][int(path.y)][int(path.z)] = Water;
                                        }
                                    }
                                    themap.m[x][y][z] = Nothing; themap.m[x][y][z + 1] = Water; themap.m[x][y - 1][z + 1] = Sand; already = true;
                                }
                                break;
                            case 2:
                                if (themap.m[x][y - 1][z - 1] == Nothing && (themap.m[x][y][z - 1] == Nothing || themap.m[x][y][z - 1] == Water) && !already && z - 1 >= 0)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x][y - 1][z - 1] = Sand; already = true;
                                }
                                if (themap.m[x][y - 1][z - 1] == Water && (themap.m[x][y][z - 1] == Nothing || themap.m[x][y][z - 1] == Water) && !already && z - 1 >= 0)
                                {
                                    if (themap.m[x][y][z - 1] == Nothing) themap.m[x][y][z - 1] = Water;
                                    else
                                    {
                                        themawaterpbuf = themap;
                                        Point path = lookingforwaterpath(Point(x, y, z - 1), y);
                                        if (path.x != -1)
                                        {
                                            themap.m[int(path.x)][int(path.y)][int(path.z)] = Water;
                                        }
                                    }
                                    themap.m[x][y][z] = Nothing; themap.m[x][y - 1][z - 1] = Sand; already = true;
                                }

                                if (themap.m[x - 1][y - 1][z] == Nothing && (themap.m[x - 1][y][z] == Nothing || themap.m[x - 1][y][z] == Water) && !already && x - 1 >= 0)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x - 1][y - 1][z] = Sand; already = true;
                                }
                                if (themap.m[x - 1][y - 1][z] == Water && (themap.m[x - 1][y][z] == Nothing || themap.m[x - 1][y][z] == Water) && !already && x - 1 >= 0)
                                {
                                    if (themap.m[x - 1][y][z] == Nothing) themap.m[x - 1][y][z] = Water;
                                    else
                                    {
                                        themawaterpbuf = themap;
                                        Point path = lookingforwaterpath(Point(x - 1, y, z), y);
                                        if (path.x != -1)
                                        {
                                            themap.m[int(path.x)][int(path.y)][int(path.z)] = Water;
                                        }
                                    }
                                    themap.m[x][y][z] = Nothing; themap.m[x - 1][y - 1][z] = Sand; already = true;
                                }

                                if (themap.m[x + 1][y - 1][z] == Nothing && (themap.m[x + 1][y][z] == Nothing || themap.m[x + 1][y][z] == Water) && !already && x + 1 < mapsize)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x + 1][y - 1][z] = Sand; already = true;
                                }
                                if (themap.m[x + 1][y - 1][z] == Water && (themap.m[x + 1][y][z] == Nothing || themap.m[x + 1][y][z] == Water) && !already && x + 1 < mapsize)
                                {
                                    if (themap.m[x + 1][y][z] == Nothing) themap.m[x + 1][y][z] = Water;
                                    else
                                    {
                                        themawaterpbuf = themap;
                                        Point path = lookingforwaterpath(Point(x + 1, y, z), y);
                                        if (path.x != -1)
                                        {
                                            themap.m[int(path.x)][int(path.y)][int(path.z)] = Water;
                                        }
                                    }
                                    themap.m[x][y][z] = Nothing; themap.m[x + 1][y - 1][z] = Sand; already = true;
                                }

                                if (themap.m[x][y - 1][z + 1] == Nothing && (themap.m[x][y][z + 1] == Nothing || themap.m[x][y][z + 1] == Water) && !already && z + 1 < mapsize)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x][y - 1][z + 1] = Sand; already = true;
                                }
                                if (themap.m[x][y - 1][z + 1] == Water && (themap.m[x][y][z + 1] == Nothing || themap.m[x][y][z + 1] == Water) && !already && z + 1 < mapsize)
                                {
                                    if (themap.m[x][y][z + 1] == Nothing) themap.m[x][y][z + 1] = Water;
                                    else
                                    {
                                        themawaterpbuf = themap;
                                        Point path = lookingforwaterpath(Point(x, y, z + 1), y);
                                        if (path.x != -1)
                                        {
                                            themap.m[int(path.x)][int(path.y)][int(path.z)] = Water;
                                        }
                                    }
                                    themap.m[x][y][z] = Nothing; themap.m[x][y][z + 1] = Water; themap.m[x][y - 1][z + 1] = Sand; already = true;
                                }
                                break;
                            case 3:
                                if (themap.m[x][y - 1][z + 1] == Nothing && (themap.m[x][y][z + 1] == Nothing || themap.m[x][y][z + 1] == Water) && !already && z + 1 < mapsize)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x][y - 1][z + 1] = Sand; already = true;
                                }
                                if (themap.m[x][y - 1][z + 1] == Water && (themap.m[x][y][z + 1] == Nothing || themap.m[x][y][z + 1] == Water) && !already && z + 1 < mapsize)
                                {
                                    if (themap.m[x][y][z + 1] == Nothing) themap.m[x][y][z + 1] = Water;
                                    else
                                    {
                                        themawaterpbuf = themap;
                                        Point path = lookingforwaterpath(Point(x, y, z + 1), y);
                                        if (path.x != -1)
                                        {
                                            themap.m[int(path.x)][int(path.y)][int(path.z)] = Water;
                                        }
                                    }
                                    themap.m[x][y][z] = Nothing; themap.m[x][y][z + 1] = Water; themap.m[x][y - 1][z + 1] = Sand; already = true;
                                }

                                if (themap.m[x - 1][y - 1][z] == Nothing && (themap.m[x - 1][y][z] == Nothing || themap.m[x - 1][y][z] == Water) && !already && x - 1 >= 0)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x - 1][y - 1][z] = Sand; already = true;
                                }
                                if (themap.m[x - 1][y - 1][z] == Water && (themap.m[x - 1][y][z] == Nothing || themap.m[x - 1][y][z] == Water) && !already && x - 1 >= 0)
                                {
                                    if (themap.m[x - 1][y][z] == Nothing) themap.m[x - 1][y][z] = Water;
                                    else
                                    {
                                        themawaterpbuf = themap;
                                        Point path = lookingforwaterpath(Point(x - 1, y, z), y);
                                        if (path.x != -1)
                                        {
                                            themap.m[int(path.x)][int(path.y)][int(path.z)] = Water;
                                        }
                                    }
                                    themap.m[x][y][z] = Nothing; themap.m[x - 1][y - 1][z] = Sand; already = true;
                                }

                                if (themap.m[x + 1][y - 1][z] == Nothing && (themap.m[x + 1][y][z] == Nothing || themap.m[x + 1][y][z] == Water) && !already && x + 1 < mapsize)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x + 1][y - 1][z] = Sand; already = true;
                                }
                                if (themap.m[x + 1][y - 1][z] == Water && (themap.m[x + 1][y][z] == Nothing || themap.m[x + 1][y][z] == Water) && !already && x + 1 < mapsize)
                                {
                                    if (themap.m[x + 1][y][z] == Nothing) themap.m[x + 1][y][z] = Water;
                                    else
                                    {
                                        themawaterpbuf = themap;
                                        Point path = lookingforwaterpath(Point(x + 1, y, z), y);
                                        if (path.x != -1)
                                        {
                                            themap.m[int(path.x)][int(path.y)][int(path.z)] = Water;
                                        }
                                    }
                                    themap.m[x][y][z] = Nothing; themap.m[x + 1][y - 1][z] = Sand; already = true;
                                }

                                if (themap.m[x][y - 1][z - 1] == Nothing && (themap.m[x][y][z - 1] == Nothing || themap.m[x][y][z - 1] == Water) && !already && z - 1 >= 0)
                                {
                                    themap.m[x][y][z] = Nothing; themap.m[x][y - 1][z - 1] = Sand; already = true;
                                }
                                if (themap.m[x][y - 1][z - 1] == Water && (themap.m[x][y][z - 1] == Nothing || themap.m[x][y][z - 1] == Water) && !already && z - 1 >= 0)
                                {
                                    if (themap.m[x][y][z - 1] == Nothing) themap.m[x][y][z - 1] = Water;
                                    else
                                    {
                                        themawaterpbuf = themap;
                                        Point path = lookingforwaterpath(Point(x, y, z - 1), y);
                                        if (path.x != -1)
                                        {
                                            themap.m[int(path.x)][int(path.y)][int(path.z)] = Water;
                                        }
                                    }
                                    themap.m[x][y][z] = Nothing; themap.m[x][y - 1][z - 1] = Sand; already = true;
                                }
                                break;
                            }
                            
                        }
                    }
            }
}

void Control()
{
    if (window.hasFocus())
    {
        if (WindowHaveFocusCD < 200) { WindowHaveFocusCD += TimesGone; SetCursorPos(ScreenSize[0] / 2, ScreenSize[1] / 2); }
        else
        {
            Vector2i MousePos; MousePos = Mouse::getPosition(window); float modif = 0.1f * TimesGone;
            if (Keyboard::isKeyPressed(Keyboard::Escape)) { window.close(); work = false; }
            if (Keyboard::isKeyPressed(Keyboard::P) && cd[2] > timevall)
            { 
                for (int i = 0; i < 13000; i++) themap.m[rand() % mapsize][rand() % mapsize][rand() % mapsize] = 0;
                cd[2] = 0;
            }
            if (Keyboard::isKeyPressed(Keyboard::O)) themap = emptymap;
            if (Keyboard::isKeyPressed(Keyboard::M)) { showmode = !showmode; Sleep(175); }
            if (Keyboard::isKeyPressed(Keyboard::R)) cout << "Fps: " << (FPS[0] + FPS[1] + FPS[2] + FPS[3] + FPS[4]) / 5 << endl;
            p.Cam.horangle += ((ScreenSize[0] / 2) - MousePos.x) * TimesGone * 0.07f; p.Cam.verangle += ((ScreenSize[1] / 2) - MousePos.y) * TimesGone * 0.07f;
            SetCursorPos(ScreenSize[0] / 2, ScreenSize[1] / 2); if (abs(p.Cam.verangle) > 90) p.Cam.verangle = 90 * p.Cam.verangle / abs(p.Cam.verangle);
            if (p.Cam.horangle > 360) p.Cam.horangle -= 360; if (p.Cam.horangle < 0) p.Cam.horangle += 360;
            if (Keyboard::isKeyPressed(Keyboard::LShift)) modif *= 2;
            modif *= 20;
            if (Keyboard::isKeyPressed(Keyboard::Space)) p.Cam.pos.y += modif * 1.4f; if (Keyboard::isKeyPressed(Keyboard::LAlt)) p.Cam.pos.y -= modif * 1.4f;
            if (Keyboard::isKeyPressed(Keyboard::W)) { p.Cam.pos.z += cos(p.Cam.horangle * 0.0175f) * modif * 5; p.Cam.pos.x += sin(p.Cam.horangle * 0.0175f) * modif * 5; }
            if (Keyboard::isKeyPressed(Keyboard::S)) { p.Cam.pos.z -= cos(p.Cam.horangle * 0.0175f) * modif * 5; p.Cam.pos.x -= sin(p.Cam.horangle * 0.0175f) * modif * 5; }
            if (Keyboard::isKeyPressed(Keyboard::A)) { p.Cam.pos.z -= sin(p.Cam.horangle * 0.0175f) * modif * 5; p.Cam.pos.x += cos(p.Cam.horangle * 0.0175f) * modif * 5; }
            if (Keyboard::isKeyPressed(Keyboard::D)) { p.Cam.pos.z += sin(p.Cam.horangle * 0.0175f) * modif * 5; p.Cam.pos.x -= cos(p.Cam.horangle * 0.0175f) * modif * 5; }

            if (Keyboard::isKeyPressed(Keyboard::Up) && cursorplace.x + 1 < mapsize && cd[2] > timevall)
            { 
                cursorplace.x++; cd[2] = 0;
            }
            if (Keyboard::isKeyPressed(Keyboard::Down) && cursorplace.x - 1 >= 0 && cd[2] > timevall)
            {
                cursorplace.x--; cd[2] = 0;
            }
            if (Keyboard::isKeyPressed(Keyboard::RControl) && cursorplace.y + 1 < mapsize && cd[2] > timevall)
            {
                cursorplace.y++; cd[2] = 0;
            }
            if (Keyboard::isKeyPressed(Keyboard::Insert) && cursorplace.y - 1 >= 0 && cd[2] > timevall)
            {
                cursorplace.y--; cd[2] = 0;
            }
            if (Keyboard::isKeyPressed(Keyboard::Right) && cursorplace.z + 1 < mapsize && cd[2] > timevall)
            {
                cursorplace.z++; cd[2] = 0;
            }
            if (Keyboard::isKeyPressed(Keyboard::Left) && cursorplace.z - 1 >= 0 && cd[2] > timevall)
            {
                cursorplace.z--; cd[2] = 0;
            }

            if (Keyboard::isKeyPressed(Keyboard::Num1) && cd[3] > timevall) { themap.m[int(cursorplace.x)][int(cursorplace.y)][int(cursorplace.z)] = Nothing; cd[3] = 0;}
            if (Keyboard::isKeyPressed(Keyboard::Num2) && cd[3] > timevall) { themap.m[int(cursorplace.x)][int(cursorplace.y)][int(cursorplace.z)] = Water; cd[3] = 0;}
            if (Keyboard::isKeyPressed(Keyboard::Num3) && cd[3] > timevall) { themap.m[int(cursorplace.x)][int(cursorplace.y)][int(cursorplace.z)] = Sand; cd[3] = 0;}
            if (Keyboard::isKeyPressed(Keyboard::Num4) && cd[3] > timevall) { themap.m[int(cursorplace.x)][int(cursorplace.y)][int(cursorplace.z)] = Wall; cd[3] = 0;}
        }
    }
    else WindowHaveFocusCD = 0;
}
void DrawAll()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); glClear(GL_DEPTH_BUFFER_BIT);
    glBindBuffer(GL_ARRAY_BUFFER, Models[0].vertexVBO);
    glVertexPointer(3, GL_FLOAT, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Models[0].indexEBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    if (showmode)
    {
        for (int x = 0; x < mapsize; x++)
            for (int y = 0; y < mapsize; y++)
                for (int z = 0; z < mapsize; z++)
                {
                    if (themap.m[x][y][z] != 0 || Point(x, y, z).IsSame(cursorplace))
                        if (!((themap.m[x + 1][y][z] != 0 && x < mapsize - 1) && (themap.m[x - 1][y][z] != 0 && x > 0) &&
                            (themap.m[x][y + 1][z] != 0 && y < mapsize - 1) && (themap.m[x][y - 1][z] != 0 && y > 0) &&
                            (themap.m[x][y][z + 1] != 0 && z < mapsize - 1) && (themap.m[x][y][z - 1] != 0 && z > 0)))
                        {
                            glMatrixMode(GL_MODELVIEW); glLoadIdentity();
                            gluLookAt(p.Cam.pos.x, p.Cam.pos.y, p.Cam.pos.z, p.Cam.pos.x + sin(p.Cam.horangle * 0.0175),
                                p.Cam.pos.y + sin(p.Cam.verangle * 0.0175), p.Cam.pos.z + cos(p.Cam.horangle * 0.0175), 0, 1, 0);
                            glTranslatef(mapvall * x, mapvall * y * 1.5f, mapvall * z);
                            if (themap.m[x][y][z] == Water) glColor3f(0.4f, 0.4f, 1.0f);
                            if (themap.m[x][y][z] == Sand) glColor3f(0.7f, 0.7f, 0.1f);
                            if (themap.m[x][y][z] == Wall) glColor3f(0.5f, 0.5f, 0.5f);
                            if (Point(x, y, z).IsSame(cursorplace))
                            {
                                glColor3f(0.1 + abs(cd[1] - 500) * blinkvall, 0.1 + abs(cd[1] - 500) * blinkvall, 0.1 + abs(cd[1] - 500) * blinkvall);
                                if (themap.m[x][y][z] == Water) glColor3f(0.4f + abs(cd[1] - 500) * blinkvall, 0.4f + abs(cd[1] - 500) * blinkvall, 0.7f + abs(cd[1] - 500) * blinkvall);
                                if (themap.m[x][y][z] == Sand) glColor3f(0.7f + abs(cd[1] - 500) * blinkvall, 0.7f + abs(cd[1] - 500) * blinkvall, 0.1f + abs(cd[1] - 500) * blinkvall);
                                if (themap.m[x][y][z] == Wall) glColor3f(0.5f + abs(cd[1] - 500) * blinkvall, 0.5f + abs(cd[1] - 500) * blinkvall, 0.5f + abs(cd[1] - 500) * blinkvall);
                            }
                            glDrawElements(GL_TRIANGLES + 3 * Models[0].PGmode, Models[0].GetQOfPG() * (3 + Models[0].PGmode), GL_UNSIGNED_INT, NULL);
                        }
                }
    }
    else
    {
        for (int x = 0; x < mapsize; x++)
            for (int y = 0; y < mapsize; y++)
                for (int z = 0; z < mapsize; z++)
                    if (themap.m[x][y][z] != 0 || Point(x, y, z).IsSame(cursorplace))
                        if (!((themap.m[x + 1][y][z] != 0 && x < mapsize - 1) && (themap.m[x - 1][y][z] != 0 && x > 0) &&
                            (themap.m[x][y + 1][z] != 0 && y < mapsize - 1) && (themap.m[x][y - 1][z] != 0 && y > 0) &&
                            (themap.m[x][y][z + 1] != 0 && z < mapsize - 1) && (themap.m[x][y][z - 1] != 0 && z > 0)))
                        {
                            glMatrixMode(GL_MODELVIEW); glLoadIdentity();
                            gluLookAt(p.Cam.pos.x, p.Cam.pos.y, p.Cam.pos.z, p.Cam.pos.x + sin(p.Cam.horangle * 0.0175),
                                p.Cam.pos.y + sin(p.Cam.verangle * 0.0175), p.Cam.pos.z + cos(p.Cam.horangle * 0.0175), 0, 1, 0);
                            glTranslatef(mapvall * x, mapvall * y * 1.5f, mapvall * z);
                            glColor3f(0.1f, 0.1f, 0.1f);
                            if (themap.m[x][y][z] == Water) glColor3f(0.2f, 0.2f, 0.5f);
                            if (themap.m[x][y][z] == Sand) glColor3f(0.35f, 0.35f, 0.05f);
                            if (themap.m[x][y][z] == Wall) glColor3f(0.25f, 0.25f, 0.25f);
                            glDrawElements(GL_TRIANGLES + 3 * Models[0].PGmode, Models[0].GetQOfPG() * (3 + Models[0].PGmode), GL_UNSIGNED_INT, NULL);
                        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  glLineWidth(3);
        for (int x = 0; x < mapsize; x++)
            for (int y = 0; y < mapsize; y++)
                for (int z = 0; z < mapsize; z++)
                    if (themap.m[x][y][z] != 0 || Point(x, y, z).IsSame(cursorplace))
                        if (!((themap.m[x + 1][y][z] != 0 && x < mapsize - 1) && (themap.m[x - 1][y][z] != 0 && x > 0) &&
                            (themap.m[x][y + 1][z] != 0 && y < mapsize - 1) && (themap.m[x][y - 1][z] != 0 && y > 0) &&
                            (themap.m[x][y][z + 1] != 0 && z < mapsize - 1) && (themap.m[x][y][z - 1] != 0 && z > 0)))
                        {
                            glMatrixMode(GL_MODELVIEW); glLoadIdentity();
                            gluLookAt(p.Cam.pos.x, p.Cam.pos.y, p.Cam.pos.z, p.Cam.pos.x + sin(p.Cam.horangle * 0.0175),
                                p.Cam.pos.y + sin(p.Cam.verangle * 0.0175), p.Cam.pos.z + cos(p.Cam.horangle * 0.0175), 0, 1, 0);
                            glTranslatef(mapvall * x, mapvall * y * 1.5f, mapvall * z);
                            glColor3f(0.1f, 0.1f, 0.1f);
                            if (themap.m[x][y][z] == Water) glColor3f(0.4f, 0.4f, 1.0f);
                            if (themap.m[x][y][z] == Sand) glColor3f(0.7f, 0.7f, 0.1f);
                            if (themap.m[x][y][z] == Wall) glColor3f(0.5f, 0.5f, 0.5f);
                            if (Point(x, y, z).IsSame(cursorplace))
                            {
                                glColor3f(0.1 + abs(cd[1] - 500) * blinkvall, 0.1 + abs(cd[1] - 500) * blinkvall, 0.1 + abs(cd[1] - 500) * blinkvall);
                                if (themap.m[x][y][z] == Water) glColor3f(0.4f + abs(cd[1] - 500) * blinkvall, 0.4f + abs(cd[1] - 500) * blinkvall, 0.7f + abs(cd[1] - 500) * blinkvall);
                                if (themap.m[x][y][z] == Sand) glColor3f(0.7f + abs(cd[1] - 500) * blinkvall, 0.7f + abs(cd[1] - 500) * blinkvall, 0.1f + abs(cd[1] - 500) * blinkvall);
                                if (themap.m[x][y][z] == Wall) glColor3f(0.5f + abs(cd[1] - 500) * blinkvall, 0.5f + abs(cd[1] - 500) * blinkvall, 0.5f + abs(cd[1] - 500) * blinkvall);
                            }
                            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Models[0].indexVBO);
                            glDrawElements(GL_TRIANGLES + 3 * Models[0].PGmode, Models[0].GetQOfPG() * (3 + Models[0].PGmode), GL_UNSIGNED_INT, NULL);
                            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//glDisableClientState(GL_COLOR_ARRAY);
                        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
    for (int i = 1; i < qofmodels; i++)
        if (Models[i].DrawMode != -1)
        {
            glMatrixMode(GL_MODELVIEW); glLoadIdentity();
            if (Models[i].DrawMode == 4)
                gluLookAt(0, 0, 0, 0, 0, 1, 0, 1, 0);
            else
                gluLookAt(p.Cam.pos.x, p.Cam.pos.y, p.Cam.pos.z, p.Cam.pos.x + sin(p.Cam.horangle * 0.0175),
                    p.Cam.pos.y + sin(p.Cam.verangle * 0.0175), p.Cam.pos.z + cos(p.Cam.horangle * 0.0175), 0, 1, 0);
            glTranslatef(Models[i].ObjPos.x, Models[i].ObjPos.y, Models[i].ObjPos.z);
            glRotatef(Models[i].yzangle, 1, 0, 0); glRotatef(Models[i].xzangle, 0, 1, 0); glRotatef(Models[i].yxangle, 0, 0, 1);
            glBindBuffer(GL_ARRAY_BUFFER, Models[i].vertexVBO); glVertexPointer(3, GL_FLOAT, 0, NULL); glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ARRAY_BUFFER, Models[i].colorVBO); glColorPointer(4, GL_FLOAT, 0, NULL); glBindBuffer(GL_ARRAY_BUFFER, 0);
            if (Models[i].DrawMode == 0 || Models[i].DrawMode == 4)
            {
                glEnableClientState(GL_VERTEX_ARRAY); glEnableClientState(GL_COLOR_ARRAY);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Models[i].indexEBO);
                glDrawElements(GL_TRIANGLES + 3 * Models[i].PGmode, Models[i].GetQOfPG() * (3 + Models[i].PGmode), GL_UNSIGNED_INT, NULL);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                glDisableClientState(GL_VERTEX_ARRAY); glDisableClientState(GL_COLOR_ARRAY);
            }
            if (Models[i].DrawMode == 1)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glLineWidth(3);
                glEnableClientState(GL_VERTEX_ARRAY); glEnableClientState(GL_COLOR_ARRAY);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Models[i].indexEBO);
                glDrawElements(GL_TRIANGLES + 3 * Models[i].PGmode, Models[i].GetQOfPG() * (3 + Models[i].PGmode), GL_UNSIGNED_INT, NULL);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                glDisableClientState(GL_VERTEX_ARRAY); glDisableClientState(GL_COLOR_ARRAY); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            if (Models[i].DrawMode == 2 || Models[i].DrawMode == 3)
            {
                glEnableClientState(GL_VERTEX_ARRAY); glColor3f(0.01, 0.01, 0.01);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Models[i].indexEBO);
                glDrawElements(GL_TRIANGLES + 3 * Models[i].PGmode, Models[i].GetQOfPG() * (3 + Models[i].PGmode), GL_UNSIGNED_INT, NULL);
                if (Models[i].DrawMode == 3) glTranslatef(0, 15, 0);
                glEnableClientState(GL_COLOR_ARRAY);  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  glLineWidth(3);
                glDrawElements(GL_TRIANGLES + 3 * Models[i].PGmode, Models[i].GetQOfPG() * (3 + Models[i].PGmode), GL_UNSIGNED_INT, NULL);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                glDisableClientState(GL_COLOR_ARRAY); glDisableClientState(GL_VERTEX_ARRAY); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
}
void LoadingScreen()
{
    Texture LS; LS.loadFromFile("data/Other/LoadingScreen.png"); Sprite LSsprite; LSsprite.setTexture(LS); LSsprite.setScale(ScreenSize[0] / 1366.0f, ScreenSize[1] / 768.0f);
    cout << ScreenSize[0] / 1366.0f << "  " << ScreenSize[1] / 768.0f << endl;
    window.clear();  window.pushGLStates(); window.draw(LSsprite); window.popGLStates(); window.display();
}
void UnInit()
{
    for (int i = 0; i < qofmodels; i++) Models[i].UnInit();
}
void Init()
{
    glewExperimental = GL_TRUE; glewInit(); glEnable(GL_DEPTH_TEST); glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glEnable(GL_CULL_FACE);
    srand(time(NULL)); glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glViewport(0, 0, ScreenSize[0], ScreenSize[1]); gluPerspective(90.0f, 1.0f, p.Cam.Znear, p.Cam.Zfar);
}
void ModelsInit()
{
    Models[0] = GenerateCube(mapvall);
    Models[1] = GenerateRetroTerrain(Point(mapsize, 0, mapsize), 0.5f, mapvall, 1);
}
void WindowInit()
{
    sf::ContextSettings settings; settings.depthBits = 16; settings.stencilBits = 8; settings.antialiasingLevel = 8;
    window.create(VideoMode(ScreenSize[0], ScreenSize[1]), "Prog", Style::Fullscreen, settings);
    window.setVerticalSyncEnabled(true); window.setMouseCursorVisible(false);
    //Image icon; icon.loadFromFile("data/Other/icon.png"); window.setIcon(256, 256, icon.getPixelsPtr()); LoadingScreen();
}
void EventUpdate()
{
    Vector2i MousePos; MousePos = Mouse::getPosition(window); Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed) { window.close(); work = false; }
        if (event.type == Event::MouseButtonPressed)
            if (event.key.code == Mouse::Left)
            {

            }
    }
}
int main()
{
    WindowInit(); Init(); ModelsInit();
    Clock clock; int AllVCount = 0, AllPGCount = 0;
    for (int i = 0; i < qofmodels; i++)
    {
        cout << "Model " << i << " Count of Vertexes " << Models[i].GetQOfVT() << " Count of Polygons " << Models[i].GetQOfPG() << endl;
        AllVCount += Models[i].GetQOfVT(); AllPGCount += Models[i].GetQOfPG();
    }
    cout << "All vertexes count " << AllVCount << " Polygons " << AllPGCount << endl;
    themap.m[mapsize / 2][mapsize / 2 + 1][mapsize / 2] = Wall;
    while (work)
    {
        TimesGone = float(clock.getElapsedTime().asMicroseconds()) / 1000.0f; clock.restart();
        FPS[fpscounter] = int(1000 / TimesGone); fpscounter++; if (fpscounter == 5) fpscounter = 0;
        for (int i = 0; i < sizeof(cd) / sizeof(float); i++) cd[i] += TimesGone;
        EventUpdate(); 
        if (cd[0] >= 25)
        {
            cd[0] = 0;
            maplog();
            if (themap.m[mapsize / 2][mapsize / 2][mapsize / 2] != Sand)
                themap.m[mapsize / 2][mapsize / 2 + 2][mapsize / 2] = Water;
            if (themap.m[mapsize / 2][mapsize / 2][mapsize / 2] != Sand)
                themap.m[mapsize / 2][mapsize / 2][mapsize / 2] = Sand;

            if (themap.m[0][mapsize / 2][mapsize / 2] != Sand)
                themap.m[0][mapsize / 2][mapsize / 2] = Sand;

            if (themap.m[mapsize / 2][mapsize / 2][0] != Sand)
                themap.m[mapsize / 2][mapsize / 2][0] = Sand;

            if (themap.m[mapsize - 1][mapsize / 2][mapsize / 2] != Sand)
                themap.m[mapsize - 1][mapsize / 2][mapsize / 2] = Sand;

            if (themap.m[mapsize / 2][mapsize / 2][mapsize - 1] != Sand)
                themap.m[mapsize / 2][mapsize / 2][mapsize - 1] = Sand;
        }
        if (cd[1] > 999) cd[1] = 0;
        if (window.hasFocus())
        {
            window.clear();
            DrawAll();
            window.display();
            Control();
        }
    }
    UnInit();
    return 0;
}
/*
glClearColor(0.0f, 0.0f, 0.0f, 1.0f); glClear(GL_DEPTH_BUFFER_BIT);
    glBindBuffer(GL_ARRAY_BUFFER, Models[0].vertexVBO);
    glVertexPointer(3, GL_FLOAT, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Models[0].indexVBO);
    glEnableClientState(GL_VERTEX_ARRAY);
    for (int x = 0; x < mapsize; x++)
        for (int y = 0; y < mapsize; y++)
            for (int z = 0; z < mapsize; z++)
            {
                if (themap.m[x][y][z] != 0 || Point(x, y, z).IsSame(cursorplace))
                    if(!((themap.m[x + 1][y][z] != 0 && x < mapsize - 1) && (themap.m[x - 1][y][z] != 0 && x > 0) &&
                    (themap.m[x][y + 1][z] != 0 && y < mapsize - 1) && (themap.m[x][y - 1][z] != 0 && y > 0) &&
                    (themap.m[x][y][z + 1] != 0 && z < mapsize - 1) && (themap.m[x][y][z - 1] != 0 && z > 0)))
                    {
                        glMatrixMode(GL_MODELVIEW); glLoadIdentity();
                        gluLookAt(p.Cam.pos.x, p.Cam.pos.y, p.Cam.pos.z, p.Cam.pos.x + sin(p.Cam.horangle * 0.0175),
                                p.Cam.pos.y + sin(p.Cam.verangle * 0.0175), p.Cam.pos.z + cos(p.Cam.horangle * 0.0175), 0, 1, 0);
                        glTranslatef(mapvall * x, mapvall * y * 1.5f, mapvall * z);
                        //glBindBuffer(GL_ARRAY_BUFFER, Models[0].colorVBO); glColorPointer(4, GL_FLOAT, 0, NULL); glBindBuffer(GL_ARRAY_BUFFER, 0);
                        if (showmode)
                        {
                            if (themap.m[x][y][z] == Water) glColor3f(0.4f, 0.4f, 1.0f);
                            if (themap.m[x][y][z] == Sand) glColor3f(0.7f, 0.7f, 0.1f);
                            if (themap.m[x][y][z] == Wall) glColor3f(0.5f, 0.5f, 0.5f);
                            if (Point(x, y, z).IsSame(cursorplace))
                            {
                                glColor3f(0.1 + abs(cd[1] - 500) * blinkvall, 0.1 + abs(cd[1] - 500) * blinkvall, 0.1 + abs(cd[1] - 500) * blinkvall);
                                if (themap.m[x][y][z] == Water) glColor3f(0.4f + abs(cd[1] - 500) * blinkvall, 0.4f + abs(cd[1] - 500) * blinkvall, 0.7f + abs(cd[1] - 500) * blinkvall);
                                if (themap.m[x][y][z] == Sand) glColor3f(0.7f + abs(cd[1] - 500) * blinkvall, 0.7f + abs(cd[1] - 500) * blinkvall, 0.1f + abs(cd[1] - 500) * blinkvall);
                                if (themap.m[x][y][z] == Wall) glColor3f(0.5f + abs(cd[1] - 500) * blinkvall, 0.5f + abs(cd[1] - 500) * blinkvall, 0.5f + abs(cd[1] - 500) * blinkvall);
                            }
                            //glEnableClientState(GL_COLOR_ARRAY);
                            glDrawElements(GL_TRIANGLES + 3 * Models[0].PGmode, Models[0].GetQOfPG() * (3 + Models[0].PGmode), GL_UNSIGNED_INT, NULL);
                            //glDisableClientState(GL_COLOR_ARRAY);
                        }
                        else
                        {
                            glColor3f(0.1f, 0.1f, 0.1f);
                            if (themap.m[x][y][z] == Water) glColor3f(0.2f, 0.2f, 0.5f);
                            if (themap.m[x][y][z] == Sand) glColor3f(0.35f, 0.35f, 0.05f);
                            if (themap.m[x][y][z] == Wall) glColor3f(0.25f, 0.25f, 0.25f);
                            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Models[0].indexVBO);
                            glDrawElements(GL_TRIANGLES + 3 * Models[0].PGmode, Models[0].GetQOfPG() * (3 + Models[0].PGmode), GL_UNSIGNED_INT, NULL);
                            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //glEnableClientState(GL_COLOR_ARRAY);
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  glLineWidth(3);
                            if (themap.m[x][y][z] == Water) glColor3f(0.4f, 0.4f, 1.0f);
                            if (themap.m[x][y][z] == Sand) glColor3f(0.7f, 0.7f, 0.1f);
                            if (themap.m[x][y][z] == Wall) glColor3f(0.5f, 0.5f, 0.5f);
                            if (Point(x, y, z).IsSame(cursorplace))
                            {
                                glColor3f(0.1 + abs(cd[1] - 500) * blinkvall, 0.1 + abs(cd[1] - 500) * blinkvall, 0.1 + abs(cd[1] - 500) * blinkvall);
                                if (themap.m[x][y][z] == Water) glColor3f(0.4f + abs(cd[1] - 500) * blinkvall, 0.4f + abs(cd[1] - 500) * blinkvall, 0.7f + abs(cd[1] - 500) * blinkvall);
                                if (themap.m[x][y][z] == Sand) glColor3f(0.7f + abs(cd[1] - 500) * blinkvall, 0.7f + abs(cd[1] - 500) * blinkvall, 0.1f + abs(cd[1] - 500) * blinkvall);
                                if (themap.m[x][y][z] == Wall) glColor3f(0.5f + abs(cd[1] - 500) * blinkvall, 0.5f + abs(cd[1] - 500) * blinkvall, 0.5f + abs(cd[1] - 500) * blinkvall);
                            }
                            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Models[0].indexVBO);
                            glDrawElements(GL_TRIANGLES + 3 * Models[0].PGmode, Models[0].GetQOfPG() * (3 + Models[0].PGmode), GL_UNSIGNED_INT, NULL);
                            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//glDisableClientState(GL_COLOR_ARRAY);
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        }
                }
            }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
    for (int i = 1; i < qofmodels; i++)
        if (Models[i].DrawMode != -1)
        {
            glMatrixMode(GL_MODELVIEW); glLoadIdentity();
            if (Models[i].DrawMode == 4)
                gluLookAt(0, 0, 0, 0, 0, 1, 0, 1, 0);
            else
                gluLookAt(p.Cam.pos.x, p.Cam.pos.y, p.Cam.pos.z, p.Cam.pos.x + sin(p.Cam.horangle * 0.0175),
                    p.Cam.pos.y + sin(p.Cam.verangle * 0.0175), p.Cam.pos.z + cos(p.Cam.horangle * 0.0175), 0, 1, 0);
            glTranslatef(Models[i].ObjPos.x, Models[i].ObjPos.y, Models[i].ObjPos.z);
            glRotatef(Models[i].yzangle, 1, 0, 0); glRotatef(Models[i].xzangle, 0, 1, 0); glRotatef(Models[i].yxangle, 0, 0, 1);
            glBindBuffer(GL_ARRAY_BUFFER, Models[i].vertexVBO); glVertexPointer(3, GL_FLOAT, 0, NULL); glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ARRAY_BUFFER, Models[i].colorVBO); glColorPointer(4, GL_FLOAT, 0, NULL); glBindBuffer(GL_ARRAY_BUFFER, 0);
            if (Models[i].DrawMode == 0 || Models[i].DrawMode == 4)
            {
                glEnableClientState(GL_VERTEX_ARRAY); glEnableClientState(GL_COLOR_ARRAY);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Models[i].indexVBO);
                glDrawElements(GL_TRIANGLES + 3 * Models[i].PGmode, Models[i].GetQOfPG() * (3 + Models[i].PGmode), GL_UNSIGNED_INT, NULL);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                glDisableClientState(GL_VERTEX_ARRAY); glDisableClientState(GL_COLOR_ARRAY);
            }
            if (Models[i].DrawMode == 1)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); glLineWidth(3);
                glEnableClientState(GL_VERTEX_ARRAY); glEnableClientState(GL_COLOR_ARRAY);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Models[i].indexVBO);
                glDrawElements(GL_TRIANGLES + 3 * Models[i].PGmode, Models[i].GetQOfPG() * (3 + Models[i].PGmode), GL_UNSIGNED_INT, NULL);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                glDisableClientState(GL_VERTEX_ARRAY); glDisableClientState(GL_COLOR_ARRAY); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            if (Models[i].DrawMode == 2 || Models[i].DrawMode == 3)
            {
                glEnableClientState(GL_VERTEX_ARRAY); glColor3f(0.01, 0.01, 0.01);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Models[i].indexVBO);
                glDrawElements(GL_TRIANGLES + 3 * Models[i].PGmode, Models[i].GetQOfPG() * (3 + Models[i].PGmode), GL_UNSIGNED_INT, NULL);
                if (Models[i].DrawMode == 3) glTranslatef(0, 15, 0);
                glEnableClientState(GL_COLOR_ARRAY);  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  glLineWidth(3);
                glDrawElements(GL_TRIANGLES + 3 * Models[i].PGmode, Models[i].GetQOfPG() * (3 + Models[i].PGmode), GL_UNSIGNED_INT, NULL);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                glDisableClientState(GL_COLOR_ARRAY); glDisableClientState(GL_VERTEX_ARRAY); glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
        */