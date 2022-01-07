#pragma once
#ifndef FUNC_H
#define FUNC_H
#endif
#include "StructsAndFunctions.h"

Object GenerateCube(float vall = 50)
{
    int CountOfPG = 6;
    int CountOfVT = 8;
    MyPolygon* Polygons; Polygons = new MyPolygon[CountOfPG];
    MyVertex* Vertexes; Vertexes = new MyVertex[CountOfVT];

    for (int i = 0; i < CountOfVT; i++)
    {
        float ba = float(50 + rand() % 50) / 100.0f;
        Vertexes[i].SetColor(ba, ba, ba);
    }

    Vertexes[0].Pos.Set(0, 0, 0);
    Vertexes[1].Pos.Set(vall, 0, 0);
    Vertexes[2].Pos.Set(vall, 0, vall);
    Vertexes[3].Pos.Set(0, 0, vall);

    Vertexes[4].Pos.Set(0, vall * 1.5f, 0);
    Vertexes[5].Pos.Set(vall, vall * 1.5f, 0);
    Vertexes[6].Pos.Set(vall, vall * 1.5f, vall);
    Vertexes[7].Pos.Set(0, vall * 1.5f, vall);

    Polygons[0].Set(0, 1, 2, 3);
    Polygons[1].Set(7, 6, 5, 4);
    Polygons[2].Set(4, 5, 1, 0);
    Polygons[3].Set(5, 6, 2, 1);
    Polygons[4].Set(6, 7, 3, 2);
    Polygons[5].Set(7, 4, 0, 3);

    Object obj(Vertexes, Polygons, CountOfVT, CountOfPG, 1, 0, true); return obj;
}

Object GenerateFuncGraph(float fristvall = 0, float lastvall = 5000, float Accuracy = 50)
{
    int qofelem = int(abs(lastvall - fristvall) / Accuracy); cout << qofelem << endl;
    int CountOfPG = qofelem * qofelem;
    int CountOfVT = CountOfPG * 4;
    MyPolygon* Polygons; Polygons = new MyPolygon[CountOfPG];
    MyVertex* Vertexes; Vertexes = new MyVertex[CountOfVT];
    cout << CountOfPG << endl;
    for (int i = 0; i < CountOfVT; i++) Vertexes[i].Pos.Set(0, 0, 0);
    cout << CountOfVT << endl;
    for (int x = 0; x < qofelem; x++)
    {
        cout << int((float(x) / float(qofelem)) * 100) << "% complete" << endl;
        for (int z = 0; z < qofelem; z++)
        {
            Vertexes[(x * qofelem + z) * 4].Pos.Set(x * Accuracy + fristvall, 0, z * Accuracy + fristvall);
            Vertexes[(x * qofelem + z) * 4 + 1].Pos.Set((x + 1) * Accuracy + fristvall, 0, z * Accuracy + fristvall);
            Vertexes[(x * qofelem + z) * 4 + 2].Pos.Set((x + 1) * Accuracy + fristvall, 0, (z + 1) * Accuracy + fristvall);
            Vertexes[(x * qofelem + z) * 4 + 3].Pos.Set(x * Accuracy + fristvall, 0, (z + 1) * Accuracy + fristvall);
            for (int i = 0; i < 4; i++)
            {
                /* Формула Димы
                float xp, yp, zp;
                xp = Vertexes[(x * qofelem + z) * 4 + i].Pos.x;
                yp = (xp * zp) / (xp + zp + 1);
                zp = Vertexes[(x * qofelem + z) * 4 + i].Pos.z;
                float r, g, b;
                r = abs(float(int(abs(xp - zp) * sin(yp * 0.02f)) % 256) / 255.0f);
                g = abs(float(int(abs(zp - yp) * sin(yp * 0.02f)) % 256) / 255.0f);
                b = abs(float(int(abs(yp - xp) * sin(yp * 0.02f)) % 256) / 255.0f);
                Vertexes[(x * qofelem + z) * 4 + i].Pos.y = yp;
                Vertexes[(x * qofelem + z) * 4 + i].SetColor(r, g, b);
                */
                /* Салфетка Дена
                float xp, yp, zp;
                xp = Vertexes[(x * qofelem + z) * 4 + i].Pos.x;
                zp = Vertexes[(x * qofelem + z) * 4 + i].Pos.z;
                yp = cosf(float((lastvall - float(abs(xp - zp))) / lastvall) * 6.28f) * 600;
                float r, g, b;
                r = abs(float(int(abs(xp)) % 256) / 255.0f);
                g = abs(float(int(abs(xp + zp)) % 256) / 255.0f);
                b = abs(float(int(abs(zp)) % 256) / 255.0f);
                Vertexes[(x * qofelem + z) * 4 + i].Pos.y = yp;
                Vertexes[(x * qofelem + z) * 4 + i].SetColor(r, g, b);*/
                /* Штука Маши 1
                float xp, yp, zp, r, g, b;
                xp = Vertexes[(x * qofelem + z) * 4 + i].Pos.x;
                zp = Vertexes[(x * qofelem + z) * 4 + i].Pos.z;
                yp = xp * zp * 0.0002f;
                r = abs(float(int(8 * xp / 3 * zp) % 256) / 255.0f);
                g = abs(float(int(2 * xp - 9 * zp) % 256) / 255.0f);
                b = abs(float(int(20 * xp + zp) % 256) / 255.0f);
                Vertexes[(x * qofelem + z) * 4 + i].Pos.y = yp;
                Vertexes[(x * qofelem + z) * 4 + i].SetColor(r, g, b);*/
                /* Штука Маши  0
                float xp, yp, zp;
                xp = Vertexes[(x * qofelem + z) * 4 + i].Pos.x;
                zp = Vertexes[(x * qofelem + z) * 4 + i].Pos.z;
                yp = xp + zp;
                float r, g, b;
                r = abs(float(int(abs(xp - zp)) % 256) / 255.0f);
                g = abs(float(int(abs(xp * zp) * sin(yp * 0.02f)) % 256) / 255.0f);
                b = abs(float(int(abs(xp / (zp + 1)) * sin(yp * 0.02f)) % 256) / 255.0f);
                Vertexes[(x * qofelem + z) * 4 + i].Pos.y = yp;
                Vertexes[(x * qofelem + z) * 4 + i].SetColor(r, g, b);*/
                float xp, yp, zp, r, g, b;
                xp = Vertexes[(x * qofelem + z) * 4 + i].Pos.x;
                zp = Vertexes[(x * qofelem + z) * 4 + i].Pos.z;
                yp = sinf(xp * 0.01f) * 300 + sinf(zp * 0.01f) * 300;
                r = abs(float(int(sinf(xp * 0.01f) * 300) % 512 - 255) / 255.0f);
                g = abs(float(int(sinf(zp * 0.01f) * 300) % 512 - 255) / 255.0f);
                b = abs(float(int(tanf(xp * 0.01f) * 300 + tanf(zp * 0.01f) * 300) % 512 - 255) / 255.0f);
                Vertexes[(x * qofelem + z) * 4 + i].Pos.y = yp;
                Vertexes[(x * qofelem + z) * 4 + i].SetColor(r, g, b);
                /*
                float xp, yp, zp, r, g, b;
                zp = Vertexes[(x * qofelem + z) * 4 + i].Pos.z;
                xp = cos(zp * (3.14 / lastvall)) * 1000;
                yp = sin(zp * (3.14 / lastvall)) * 1000;
                r = abs(float(int(xp) % 256) / 255.0f);
                g = abs(float(int(yp) % 256) / 255.0f);
                b = abs(float(int(zp) % 256) / 255.0f);
                Vertexes[(x * qofelem + z) * 4 + i].Pos.y = yp;
                Vertexes[(x * qofelem + z) * 4 + i].SetColor(r, g, b);*/
            }
            Polygons[x * qofelem + z].Set((x * qofelem + z) * 4, (x * qofelem + z) * 4 + 1, (x * qofelem + z) * 4 + 2, (x * qofelem + z) * 4 + 3);
        }
    }
    
    Object obj(Vertexes, Polygons, CountOfVT, CountOfPG, 1, 0, true); return obj;
}

