#pragma once
#ifndef RETROTERRAIN_H
#define RETROTERRAIN_H
#endif
#include "StructsAndFunctions.h"

Object GenerateRetroTerrain(Point Mapsize = Point(10, 0, 50), float colorvall = 0.0f, int quadssize = 800, int maxheight = 1600)
{
    /*
    int CountOfPG = Mapsize.x * Mapsize.z;
    int CountOfVT = (Mapsize.x + 1) * (Mapsize.z + 1);
    MyPolygon* Polygons; Polygons = new MyPolygon[CountOfPG];
    MyVertex* Vertexes; Vertexes = new MyVertex[CountOfVT];
    int* heightmap; heightmap = new int[CountOfVT];
    for (int i = 0; i < CountOfVT; i++) heightmap[i] = 0;
    for (int i = 0; i < (Mapsize.x + Mapsize.z) * 0.7; i++)
    { 
        int height = 50 + rand() % maxheight; int x = rand() % (int(Mapsize.x) - 3) + 2; int z = rand() % (int(Mapsize.z) - 3) + 2;
        for (int zz = 0; zz <= 4; zz++)
            for (int xx = -abs(abs(zz - 2) - 2); xx <= abs(abs(zz - 2) - 2); xx++)
            {
                float vall = 1 - (abs(zz - 2) + abs(xx)) * 0.35; if (vall <= 0) vall = 0; 
                heightmap[(x + xx) * (int(Mapsize.z) + 1) + z + zz - 2] = int(height * vall);
            }    
    }
    for (int i = 0; i < (Mapsize.x + 1); i++)
        for (int j = 0; j < (Mapsize.z + 1); j++)
        {
            Vertexes[i * (int(Mapsize.z) + 1) + j].Pos.Set(i * quadssize, heightmap[i * (int(Mapsize.z) + 1) + j], j * quadssize);
            if (heightmap[i * (int(Mapsize.z) + 1) + j] == 0) Vertexes[i * (int(Mapsize.z) + 1) + j].SetColor(0, 1, 1, 1);
            else Vertexes[i * (int(Mapsize.z) + 1) + j].SetColor(1, 0, 1, 1);
        }
    for (int i = 0; i < Mapsize.x; i++)
        for (int j = 0; j < Mapsize.z; j++)
        {
            Polygons[i * int(Mapsize.z) + j].Set(i * int(Mapsize.z + 1) + j, i * int(Mapsize.z + 1) + (j + 1),
                (i + 1) * int(Mapsize.z + 1) + (j + 1), (i + 1) * int(Mapsize.z + 1) + j);
        }
        */
    int CountOfPG = Mapsize.x * Mapsize.z * 5;
    int CountOfVT = Mapsize.x * Mapsize.z * 8;
    float vall = 60;
    MyPolygon* Polygons; Polygons = new MyPolygon[CountOfPG];
    MyVertex* Vertexes; Vertexes = new MyVertex[CountOfVT];
    int* heightmap; heightmap = new int[(Mapsize.x + 1) * (Mapsize.z + 1)]; 
    for (int i = 0; i < (Mapsize.x + 1) * (Mapsize.z + 1); i++) heightmap[i] = 0;
    for (int i = 0; i < (Mapsize.x + Mapsize.z) * 0.65f; i++)
    {
        int height = 50 + rand() % maxheight; int x = rand() % (int(Mapsize.x) - 3) + 2; int z = rand() % (int(Mapsize.z) - 3) + 2;
        for (int zz = 0; zz <= 4; zz++)
            for (int xx = -abs(abs(zz - 2) - 2); xx <= abs(abs(zz - 2) - 2); xx++)
            {
                float vall = 1 - (abs(zz - 2) + abs(xx)) * 0.35; if (vall <= 0) vall = 0;
                heightmap[(x + xx) * (int(Mapsize.z) + 1) + z + zz - 2] = int(height * vall);
            }
    }
    for (int i = 0; i < Mapsize.x * Mapsize.z; i++)
    {
        Vertexes[i * 8].Pos.Set((i / int(Mapsize.z)) * quadssize, heightmap[i + i / int(Mapsize.z)], (i % int(Mapsize.z)) * quadssize);
        Vertexes[i * 8 + 1].Pos.Set((i / int(Mapsize.z) + 1) * quadssize, heightmap[i + int(Mapsize.z + 1) + i / int(Mapsize.z)], (i % int(Mapsize.z)) * quadssize);
        Vertexes[i * 8 + 2].Pos.Set((i / int(Mapsize.z) + 1) * quadssize, heightmap[i + int(Mapsize.z + 1) + 1 + i / int(Mapsize.z)], (i % int(Mapsize.z) + 1) * quadssize);
        Vertexes[i * 8 + 3].Pos.Set((i / int(Mapsize.z)) * quadssize, heightmap[i + 1 + i / int(Mapsize.z)], (i % int(Mapsize.z) + 1) * quadssize);
        for (int u = 0; u < 4; u++)
            if (Vertexes[i * 8 + u].Pos.y == 0) Vertexes[i * 8 + u].SetColor(0.0f, 1.0f - colorvall, 1.0f - colorvall, 1.0f);
            else Vertexes[i * 8 + u].SetColor(1.0f - colorvall, 0.0f, 1.0f - colorvall, 1.0f);

        Point Centre = PointMid(Vertexes[i * 8].Pos, Vertexes[i * 8 + 2].Pos); Point vec;
        if (PointMid(Vertexes[i * 8 + 1].Pos, Vertexes[i * 8 + 3].Pos).y > Centre.y)
            Centre = PointMid(Vertexes[i * 8 + 1].Pos, Vertexes[i * 8 + 3].Pos);

        Vertexes[i * 8 + 4].Pos.Set((i / int(Mapsize.z)) * quadssize, heightmap[i + i / int(Mapsize.z)], (i % int(Mapsize.z)) * quadssize);
        vec = MakeVec(Centre, Vertexes[i * 8 + 4].Pos); vec = Normalise(vec); vec.Scale(vall, vall, vall);
        Vertexes[i * 8 + 4].Pos.Move(vec.x, vec.y, vec.z);
        Vertexes[i * 8 + 5].Pos.Set((i / int(Mapsize.z) + 1) * quadssize, heightmap[i + int(Mapsize.z + 1) + i / int(Mapsize.z)], (i % int(Mapsize.z)) * quadssize);
        vec = MakeVec(Centre, Vertexes[i * 8 + 5].Pos); vec = Normalise(vec); vec.Scale(vall, vall, vall);
        Vertexes[i * 8 + 5].Pos.Move(vec.x, vec.y, vec.z);
        Vertexes[i * 8 + 6].Pos.Set((i / int(Mapsize.z) + 1) * quadssize, heightmap[i + int(Mapsize.z + 1) + 1 + i / int(Mapsize.z)], (i % int(Mapsize.z) + 1) * quadssize);
        vec = MakeVec(Centre, Vertexes[i * 8 + 6].Pos); vec = Normalise(vec); vec.Scale(vall, vall, vall);
        Vertexes[i * 8 + 6].Pos.Move(vec.x, vec.y, vec.z);
        Vertexes[i * 8 + 7].Pos.Set((i / int(Mapsize.z)) * quadssize, heightmap[i + 1 + i / int(Mapsize.z)], (i % int(Mapsize.z) + 1) * quadssize);
        vec = MakeVec(Centre, Vertexes[i * 8 + 7].Pos); vec = Normalise(vec); vec.Scale(vall, vall, vall);
        Vertexes[i * 8 + 7].Pos.Move(vec.x, vec.y, vec.z);
        
        for (int u = 0; u < 4; u++)
            Vertexes[i * 8 + 4 + u].SetColor(0.0f, 0.0f, 0.0f, 1.0f);

        Polygons[i * 5].Set(i * 8 + 4, i * 8 + 5, i * 8 + 1, i * 8);
        Polygons[i * 5 + 1].Set(i * 8 + 5, i * 8 + 6, i * 8 + 2, i * 8 + 1);
        Polygons[i * 5 + 2].Set(i * 8 + 6, i * 8 + 7, i * 8 + 3, i * 8 + 2);
        Polygons[i * 5 + 3].Set(i * 8 + 7, i * 8 + 4, i * 8, i * 8 + 3);
        Polygons[i * 5 + 4].Set(i * 8 + 7, i * 8 + 6, i * 8 + 5, i * 8 + 4);
        /*
        Polygons[i * 5].Set(i * 8, i * 8 + 1, i * 8 + 5, i * 8 + 4);
        Polygons[i * 5 + 1].Set(i * 8 + 1, i * 8 + 2, i * 8 + 6, i * 8 + 5);
        Polygons[i * 5 + 2].Set(i * 8 + 2, i * 8 + 3, i * 8 + 7, i * 8 + 6);
        Polygons[i * 5 + 3].Set(i * 8 + 3, i * 8, i * 8 + 4, i * 8 + 7);
        Polygons[i * 5 + 4].Set(i * 8 + 4, i * 8 + 5, i * 8 + 6, i * 8 + 7);*/
    }
    Object obj(Vertexes, Polygons, CountOfVT, CountOfPG, 1, 0, true); return obj;
}