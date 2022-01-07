#pragma once
#ifndef STRUCTSANDFUNCTIONS_H
#define STRUCTSANDFUNCTIONS_H
#endif
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <strstream>
#include <string>
#include <stdio.h>
#include <malloc.h>

using namespace std;

float ScreenSize[2] = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) }, TimesGone, VallForDebugging = 0, WindowHaveFocusCD = 0;

struct Point
{
public:
    float x = 0, y = 0, z = 0;
    Point() {} void Get(Point a) { x = a.x; y = a.y; z = a.z; }
    Point(float X, float Y, float Z = 0) { x = X; y = Y; z = Z; }
    void Set(float X, float Y, float Z = 0) { x = X; y = Y; z = Z; }
    void Move(float X, float Y = 0, float Z = 0) { x += X; y += Y; z += Z; }
    void Scale(float X, float Y, float Z) { x *= X; y *= Y; z *= Z; }
    bool IsSame(Point a) { if (x == a.x && y == a.y && z == a.z) return true; else return false; }
};

float Dist(Point a, Point b) { return sqrtf(fabsf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2) + powf(a.z - b.z, 2))); }
float Lenght(Point a) { return sqrtf(fabsf(powf(a.x, 2) + powf(a.y, 2) + powf(a.z, 2))); }
float WhatIsAngle(Point a, Point b)
{
    float da = (a.x * b.x + a.y * b.y + a.z * b.z) / fabsf(Lenght(a) * Lenght(b)); if (da <= 0.999999) { da = acosf(da); da = da * 57.296f; return da; }
    else return 0;
}
float WhatIsAngle(Point vec)
{
    if (vec.x == 0 && vec.y > 0) return 90; if (vec.x == 0 && vec.y < 0) return 270;
    if (vec.y == 0 && vec.x > 0) return 0; if (vec.y == 0 && vec.x < 0) return 180;
    if (vec.x == 0 && vec.y == 0) return 0; else
    {
        float gippo = sqrtf(vec.x * vec.x + vec.y * vec.y), sinvall = fabsf(vec.y / gippo), angle = 0; int fourth = 0;
        if (vec.x >= 0 && vec.y >= 0) fourth = 0; if (vec.x <= 0 && vec.y >= 0) fourth = 1; if (vec.x <= 0 && vec.y <= 0) fourth = 2; if (vec.x >= 0 && vec.y <= 0) fourth = 3;
        angle = asinf(sinvall) * 57.296f; angle = angle + 90 * fourth; return angle;
    }
}
bool IsFilled(Point a) { if (a.x != 0 || a.y != 0 || a.z != 0) return true; else return false; }
Point PointMid(Point a, Point b) { return Point((a.x + b.x) / 2, (a.y + b.y) / 2, (a.z + b.z) / 2); }
Point RotateVec(Point vec, float ang) { return Point(vec.x * cosf(ang * 0.0175f) - vec.y * sinf(ang * 0.0175f), vec.x * sinf(ang * 0.0175f) + vec.y * cosf(ang * 0.0175f), 0); }
Point AdvRotateVec(Point vec, float xzang = 0, float yxang = 0, float yzang = 0)
{
    Point buffertvec; 
    if (xzang != 0)
    {
        buffertvec.x = vec.x * cosf(xzang * 0.01745329) - vec.z * sinf(xzang * 0.01745329); buffertvec.z = vec.x * sinf(xzang * 0.01745329) + vec.z * cosf(xzang * 0.01745329);
        buffertvec.y = vec.y; vec = buffertvec;
    }
    if (yxang != 0)
    {
        buffertvec.y = vec.x * cosf(yxang * 0.01745329) - vec.y * sinf(yxang * 0.01745329); buffertvec.x = vec.x * sinf(yxang * 0.01745329) + vec.y * cosf(yxang * 0.01745329);
        buffertvec.z = vec.z; vec = buffertvec;
    }
    if (yzang != 0)
    {
        buffertvec.y = vec.y * cosf(yzang * 0.01745329) - vec.z * sinf(yzang * 0.01745329); buffertvec.z = vec.y * sinf(yzang * 0.01745329) + vec.z * cosf(yzang * 0.01745329);
        buffertvec.x = vec.x; vec = buffertvec;
    }
    return vec;
}
Point Normalise(Point vec) 
{ 
    float lenght = Lenght(vec); vec.x /= lenght; vec.y /= lenght; vec.z /= lenght; 
    if (fabsf(vec.x) < 0.00001) vec.x = 0; if (fabsf(vec.y) < 0.00001) vec.y = 0; if (fabsf(vec.z) < 0.00001) vec.z = 0; return vec; }
Point MakeVec(Point a, Point b) { return Point(a.x - b.x, a.y - b.y, a.z - b.z); }
Point Sum(Point a, Point b) { return Point(a.x + b.x, a.y + b.y, a.z + b.z); }

Point LightPos(0.0f, 0.0f, 0.0f);

struct MyVertex { 
public: Point Pos, Normal; float Color[4] = {1, 1, 1, 1};
      MyVertex() {} 
      void SetColor(float r, float g, float b, float a = 1) { Color[0] = r; Color[1] = g; Color[2] = b; Color[3] = a; }
};

struct Camera
{
public:
    Point pos; float horangle, verangle, FOV, ScreenVall, Znear, Zfar, Tan; bool canmove = true;
    Camera() {}
    Camera(Point a, float HA, float VA, float fov, float screenvall, float znear, float zfar)
    { pos.Get(a); horangle = HA; verangle = VA; FOV = fov; ScreenVall = screenvall; Znear = znear, Zfar = zfar; Tan = 1 / tan(FOV * 0.5 * 0.0175); }
    void Set(Point a, float HA, float VA, float fov, float screenvall, float znear, float zfar)
    { pos.Get(a); horangle = HA; verangle = VA; FOV = fov; ScreenVall = screenvall; Znear = znear, Zfar = zfar; Tan = 1 / tan(FOV * 0.5 * 0.0175); }
};

struct MyPolygon
{
private: Point NormalOfPG;  int NumOfV[4] = { -1, -1, -1, -1 };
public:
    MyPolygon() {}
    int GetVertexNumb(int a) { return NumOfV[a]; }
    void SetNormal(Point a) { NormalOfPG = a; }
    void Set(int a, int b, int c, int d = -1) { NumOfV[0] = a; NumOfV[1] = b; NumOfV[2] = c; NumOfV[3] = d; }
    Point GetNormal() { return NormalOfPG; } 
};

struct Object
{
private:
    int QofVTinobj, QofPGinObj; MyVertex* ObjVertexes; MyPolygon* ObjPolygons;
    void UpdateVertexesPos()
    {
        float* Vertexes; Vertexes = new float[3 * QofVTinobj];
        for (int i = 0; i < QofVTinobj; i++)
        { Vertexes[i * 3] = ObjVertexes[i].Pos.x; Vertexes[i * 3 + 1] = ObjVertexes[i].Pos.y; Vertexes[i * 3 + 2] = ObjVertexes[i].Pos.z; }
        glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float) * QofVTinobj, Vertexes);
        glBindBuffer(GL_ARRAY_BUFFER, 0); delete[] Vertexes;
    }
    void UpdateVertexesColors()
    {
        float* Colors; Colors = new float[4 * QofVTinobj];
        for (int i = 0; i < QofVTinobj; i++)
        {
            if (fixedcolor)
            {
                Colors[i * 4] = ObjVertexes[i].Color[0]; Colors[i * 4 + 1] = ObjVertexes[i].Color[1];
                Colors[i * 4 + 2] = ObjVertexes[i].Color[2];  Colors[i * 4 + 3] = ObjVertexes[i].Color[3];
            }
            else
            {
                float vall = 255 - WhatIsAngle(MakeVec(LightPos, Sum(ObjVertexes[i].Pos, ObjPos)), ObjVertexes[i].Normal) * 1.3; if (vall < 0) vall = 0; vall /= 255;
                Colors[i * 4] = vall * ObjVertexes[i].Color[0]; Colors[i * 4 + 1] = vall * ObjVertexes[i].Color[1];
                Colors[i * 4 + 2] = vall * ObjVertexes[i].Color[2];  Colors[i * 4 + 3] = ObjVertexes[i].Color[3];
            }
        }
        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 4 * QofVTinobj, Colors);
        glBindBuffer(GL_ARRAY_BUFFER, 0); delete[] Colors;
    }
public:
    Point ObjPos; Point coll1; Point coll2;
    float xzangle = 0, yxangle = 0, yzangle = 0; bool fixedcolor = false; int PGmode = 0; //0 - triangles 1 - Quads
    GLuint vertexVBO = 0; GLuint colorVBO = 0; GLuint textVBO = 0; GLuint indexEBO = 0; int DrawMode = 0; //0 - faces 1 - lines 2 - simple retro 3 - terretro 4 - faces in interf
    Object() {}
    Object(MyVertex* ObjVerts, MyPolygon* ObjPGs, int qofvt, int qofpg, int mode, int drawmode, bool fix = false)
    { 
        ObjVertexes = ObjVerts;  ObjPolygons = ObjPGs;  QofVTinobj = qofvt;  QofPGinObj = qofpg; PGmode = mode; DrawMode = drawmode; fixedcolor = fix; this->Init();
    }
    void Init()
    {
        this->CalculatePGNormals();  this->CalculateVTNormals();
        glGenBuffers(1, &vertexVBO);
        float* Vertexes; Vertexes = new float[3 * QofVTinobj];
        for (int i = 0; i < QofVTinobj; i++)
        { Vertexes[i * 3] = ObjVertexes[i].Pos.x; Vertexes[i * 3 + 1] = ObjVertexes[i].Pos.y; Vertexes[i * 3 + 2] = ObjVertexes[i].Pos.z; }
        glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * QofVTinobj, Vertexes, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0); delete[] Vertexes;
        glGenBuffers(1, &colorVBO); 
        float* Colors; Colors = new float[4 * QofVTinobj];
        for (int i = 0; i < QofVTinobj; i++)
        {
            if (fixedcolor)
            {
                Colors[i * 4] = ObjVertexes[i].Color[0]; Colors[i * 4 + 1] = ObjVertexes[i].Color[1];
                Colors[i * 4 + 2] = ObjVertexes[i].Color[2];  Colors[i * 4 + 3] = ObjVertexes[i].Color[3];
            }
            else
            {
                float vall = 255 - WhatIsAngle(MakeVec(LightPos, Sum(ObjVertexes[i].Pos, ObjPos)), ObjVertexes[i].Normal) * 1.3; if (vall < 0) vall = 0; vall /= 255;
                Colors[i * 4] = vall * ObjVertexes[i].Color[0]; Colors[i * 4 + 1] = vall * ObjVertexes[i].Color[1];
                Colors[i * 4 + 2] = vall * ObjVertexes[i].Color[2];  Colors[i * 4 + 3] = ObjVertexes[i].Color[3];
            }
        }
        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) * QofVTinobj, Colors, GL_STREAM_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0); delete[] Colors;
        glGenBuffers(1, &indexEBO); 
        int* Indexes; Indexes = new int[(3 + PGmode) * QofPGinObj];
        for (int i = 0; i < QofPGinObj; i++)
        { Indexes[i * (3 + PGmode)] = ObjPolygons[i].GetVertexNumb(0); Indexes[i * (3 + PGmode) + 1] = ObjPolygons[i].GetVertexNumb(1); 
        Indexes[i * (3 + PGmode) + 2] = ObjPolygons[i].GetVertexNumb(2); if (PGmode) Indexes[i * (3 + PGmode) + 3] = ObjPolygons[i].GetVertexNumb(3);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (3 + PGmode) * sizeof(int) * QofPGinObj, Indexes, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        delete[] Indexes;
    }
    void UnInit() { glDeleteBuffers(1, &vertexVBO); glDeleteBuffers(1, &colorVBO); glDeleteBuffers(1, &indexEBO); }
    void Move(float x, float y, float z = 0, bool upcolors = true)  { ObjPos.x += x; ObjPos.y += y; ObjPos.z += z; if (upcolors) this->UpdateVertexesColors(); }
    void Move(Point a) { this->Move(a.x, a.y, a.z); }
    void Transfer(float x, float y, float z = 0, bool upcolors = true) { Move(x - ObjPos.x, y - ObjPos.y, z - ObjPos.z, upcolors);}
    void Rotate(float xzang, float yxang = 0, float yzang = 0, bool upcolors = true)
    { 
        xzangle += xzang; yxangle += yxang; yzangle += yzang;
        for (int i = 0; i < QofVTinobj; i++) ObjVertexes[i].Normal = AdvRotateVec(ObjVertexes[i].Normal, -xzang, -yxang, -yzang);
        if (upcolors) this->UpdateVertexesColors();// this->CalculateHitBox();
    }
    void FillColor(float r, float g, float b, float a = 1, int code = 0, float vall = 1) 
    {  
        if (code == 0) for (int i = 0; i < QofVTinobj; i++) ObjVertexes[i].SetColor(r, g, b, a);
        if (code == 1) for (int i = 0; i < QofVTinobj; i++) 
            if (ObjVertexes[i].Pos.z < 2) ObjVertexes[i].SetColor(0, vall, vall, 1); else ObjVertexes[i].SetColor(vall, 0, vall, 1);
        if (code == 2) for (int i = 0; i < QofVTinobj; i++)
            if (ObjVertexes[i].Pos.z < 2) ObjVertexes[i].SetColor(0, vall, vall, 1); else ObjVertexes[i].SetColor(vall, vall, 0, vall);
        if (code == 3) for (int i = 0; i < QofVTinobj; i++) ObjVertexes[i].SetColor(float(rand() % 10 + 40) / 100.0f, 0.3f, float(rand() % 15 + 85) / 100.0f, vall);
        if (code == 4) for (int i = 0; i < QofVTinobj; i++)
            if (ObjVertexes[i].Pos.y < 2) ObjVertexes[i].SetColor(0, vall, vall, 1); else ObjVertexes[i].SetColor(vall, 0, vall, 1);
        this->UpdateVertexesColors(); 
    }
    void CalculatePGNormals()
    {
        for (int i = 0; i < QofPGinObj; i++)
        {
            Point l1, l2; 
            l1 = MakeVec(ObjVertexes[ObjPolygons[i].GetVertexNumb(1)].Pos, ObjVertexes[ObjPolygons[i].GetVertexNumb(0)].Pos); 
            l2 = MakeVec(ObjVertexes[ObjPolygons[i].GetVertexNumb(2)].Pos, ObjVertexes[ObjPolygons[i].GetVertexNumb(0)].Pos);
            Point NormalOfPG; NormalOfPG.x = l1.y * l2.z - l1.z * l2.y; NormalOfPG.y = l1.z * l2.x - l1.x * l2.z; NormalOfPG.z = l1.x * l2.y - l1.y * l2.x;
            NormalOfPG = Normalise(NormalOfPG); ObjPolygons[i].SetNormal(NormalOfPG);
        }
    }
    void CalculateVTNormals()
    {
        for (int i = 0; i < QofVTinobj; i++)
        {
            const int vall = 10;
            int PGs[vall]; int howmanyPGs = 0; bool stop = false;
            for (int j = 0; j < QofPGinObj && !stop; j++) for (int u = 0; u < 3 + PGmode && !stop; u++)
                if (ObjPolygons[j].GetVertexNumb(u) == i) { if (howmanyPGs < vall) { PGs[howmanyPGs] = j; howmanyPGs++; } else stop = true; }
            ObjVertexes[i].Normal = ObjPolygons[PGs[0]].GetNormal();
            for (int j = 1; j < howmanyPGs; j++)
            {
                Point pgnormal = ObjPolygons[PGs[j]].GetNormal(); bool calc = true;
                for (int u = 0; u < j; u++)
                    if (pgnormal.x == ObjPolygons[PGs[u]].GetNormal().x
                        && pgnormal.y == ObjPolygons[PGs[u]].GetNormal().y
                        && pgnormal.z == ObjPolygons[PGs[u]].GetNormal().z) calc = false;
                if (calc) ObjVertexes[i].Normal = PointMid(ObjVertexes[i].Normal, pgnormal);
            }
            /*ObjVertexes[i].Normal = ObjPolygons[PGs[0]].GetNormal();
            for (int j = 1; j < howmanyPGs; j++)
            {
                Point pgnormal = ObjPolygons[PGs[j]].GetNormal(); bool calc = true;
                for (int u = 0; u < howmanyPGs; u++)
                    if (u != j && pgnormal.x == ObjPolygons[PGs[u]].GetNormal().x
                        && pgnormal.y == ObjPolygons[PGs[u]].GetNormal().y
                        && pgnormal.z == ObjPolygons[PGs[u]].GetNormal().z)
                        calc = false;
                if (calc) ObjVertexes[i].Normal = PointMid(ObjVertexes[i].Normal, pgnormal);
            }*/
            ObjVertexes[i].Normal = Normalise(ObjVertexes[i].Normal);
        }
    }
    void CalculateHitBox()
    {
        coll1.Set(999999, 999999, 999999); coll2.Set(-999999, -999999, -999999);
        for (int i = 0; i < QofVTinobj; i++)
        {
            if (ObjVertexes[i].Pos.x < coll1.x) coll1.x = ObjVertexes[i].Pos.x; if (ObjVertexes[i].Pos.y < coll1.y) coll1.y = ObjVertexes[i].Pos.y;
            if (ObjVertexes[i].Pos.z < coll1.z) coll1.z = ObjVertexes[i].Pos.z; if (ObjVertexes[i].Pos.x > coll2.x) coll2.x = ObjVertexes[i].Pos.x;
            if (ObjVertexes[i].Pos.y > coll2.y) coll2.y = ObjVertexes[i].Pos.y; if (ObjVertexes[i].Pos.z > coll2.z) coll2.z = ObjVertexes[i].Pos.z;
        }
    }
    int GetQOfPG() { return QofPGinObj; } int GetQOfVT() { return QofVTinobj; }
    MyVertex GetVertex(int a) { if (a < QofVTinobj) return ObjVertexes[a]; }
    MyPolygon GetPolygon(int a) { if (a < QofPGinObj) return ObjPolygons[a]; }
    Object GetCopy() { return Object(ObjVertexes, ObjPolygons, QofVTinobj, QofPGinObj, PGmode, DrawMode, fixedcolor); }
};

GLuint LoadTexture(sf::String name)
{
    sf::Image image; if (!image.loadFromFile(name)) { return EXIT_FAILURE; cout << "Fail" << endl; } image.flipVertically();
    GLuint texture = 0; glGenTextures(1, &texture); glBindTexture(GL_TEXTURE_2D, texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getSize().x, image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return texture;
}