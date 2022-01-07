#pragma once
#include "StructsAndFunctions.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <strstream>
#include <string>

class TheColor
{
public:
    string name; float colorofthis[4] = {1, 1, 1, 1};
};

Object LoadModel(string objname, string mtlname = "", Point trans = Point(0, 0, 0), Point scale = Point(1, 1, 1), int tink = 0, bool ChangeOrder = false)
{
    TheColor* Colors; Colors = new TheColor[0]; bool istherecolors = false; int qofcolors = 0;
    if (mtlname != "")
    {
        ifstream f(mtlname); if (f.is_open()) istherecolors = true;
        if (istherecolors)
        {
            while (!f.eof())
            {
                char line[128]; f.getline(line, 128); strstream s; s << line; char junk;
                if (line[0] == 'n' && line[1] == 'e' && line[2] == 'w')
                {
                    if (qofcolors != 0)
                    {
                        string name; s >> junk >> junk >> junk >> junk >> junk >> junk >> name;
                        TheColor* PastColors; 
                        PastColors = new TheColor[qofcolors];
                        for (int i = 0; i < qofcolors; i++) PastColors[i] = Colors[i]; 
                        delete[] Colors; qofcolors++;
                        Colors = new TheColor[qofcolors];
                        for (int i = 0; i < qofcolors - 1; i++) Colors[i] = PastColors[i]; 
                        Colors[qofcolors - 1].name = name;
                    }
                    else 
                    {
                        string name; s >> junk >> junk >> junk >> junk >> junk >> junk >> name; 
                        qofcolors++; Colors = new TheColor[qofcolors]; Colors[qofcolors - 1].name = name; 
                    }
                }
                if (line[0] == 'K' && line[1] == 'd')
                {
                    float col[3]; s >> junk >> junk >> col[0] >> col[1] >> col[2];
                    Colors[qofcolors - 1].colorofthis[0] = col[0]; Colors[qofcolors - 1].colorofthis[1] = col[1]; Colors[qofcolors - 1].colorofthis[2] = col[2];
                }
            }
        }
    }
    int CountOfPGInModel = 0, CountOfVertexesInModel = 0,
        CounterOfPG = 0, CounterOfVT = 0;
    float actcolor[4] = {1, 1, 1, 1};
    if (true)
    {
        ifstream f(objname); if (!f.is_open()) return Object();
        while (!f.eof())
        {
            char line[128]; f.getline(line, 128);
            if (line[0] == 'v' && line[1] != 't' && line[1] != 'n') CountOfVertexesInModel++;
            if (line[0] == 'f') CountOfPGInModel++;
        }
    }
    MyPolygon* Polygons; Polygons = new MyPolygon[CountOfPGInModel]; MyVertex* Vertexes; Vertexes = new MyVertex[CountOfVertexesInModel]; 
    ifstream f(objname); f.is_open();
    while (!f.eof())
    {
        char line[128]; f.getline(line, 128); strstream s; s << line; char junk;
        if (line[0] == 'u' && line[1] == 's' && line[2] == 'e' && istherecolors)
        {
            string name; s >> junk >> junk >> junk >> junk >> junk >> junk >> name;
            for (int i = 0; i < qofcolors; i++)
                if (Colors[i].name == name)
                { actcolor[0] = Colors[i].colorofthis[0]; actcolor[1] = Colors[i].colorofthis[1]; actcolor[2] = Colors[i].colorofthis[2]; actcolor[3] = Colors[i].colorofthis[3];}
        }
        if (line[0] == 'v')
        { 
            Point v; if (tink == 0) s >> junk >> v.x >> v.y >> v.z; else s >> junk >> v.x >> v.z >> v.y;
            Vertexes[CounterOfVT].Pos.Set(v.x * scale.x + trans.x, v.y * scale.y + trans.y, v.z * scale.z + trans.z); CounterOfVT++; 
        }
        if (line[0] == 'f')
        {
            int f[3]; s >> junk >> f[0] >> f[1] >> f[2]; Polygons[CounterOfPG].Set(f[0] - 1, f[1] - 1, f[2] - 1);
            Vertexes[f[0] - 1].SetColor(actcolor[0], actcolor[1], actcolor[2], actcolor[3]);
            Vertexes[f[1] - 1].SetColor(actcolor[0], actcolor[1], actcolor[2], actcolor[3]);
            Vertexes[f[2] - 1].SetColor(actcolor[0], actcolor[1], actcolor[2], actcolor[3]); CounterOfPG++;
        }
    }
    if (ChangeOrder) 
        for (int i = 0; i < CountOfPGInModel; i++)
            Polygons[i].Set(Polygons[i].GetVertexNumb(2), Polygons[i].GetVertexNumb(1), Polygons[i].GetVertexNumb(0));
    Object obj(Vertexes, Polygons, CountOfVertexesInModel, CountOfPGInModel, 0, 0, false); return obj;
}