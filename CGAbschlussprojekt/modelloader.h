/*
 * ModelLoader class
 * A simple interface to the Assimp library to generate arrays
 * to be used as Buffer Objects within OpenGL
 *
 * v0.4 beta, 2015
 *
 * kai.krueckel@alumni.fh-aachen.de
 *
 * Released under THE BEER-WARE LICENSE (Rev. 42)
 *
*/

#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <GL/gl.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <qdebug.h>
#include <QVector>
#include <QVector2D>
#include <QVector3D>

class ModelLoader
{
public:
    ModelLoader();
    ~ModelLoader();

    bool loadObjectFromFile(const std::string& pFile);

    //KAI'S FUNKTIONEN
    // Generates a VBO with v1.x, v1.y, v1.z, v1.w, v2.x, ...
    void genSimpleVBO(GLfloat* vbo, unsigned int meshId = 0) const;

    // Generates a VBO with
    // v1.x, v1.y, v1.z, v1.w, [n1.x, n1.y, n1.z, n1.w,] [t1.u, t1.v, t1.w, t1.a,] v2.x, ...
    void genVBO(GLfloat* vbo, unsigned int meshId = 0, bool normals = true, bool texcoords = true) const;

    // Generates am index array with v1, v2, v3, v4, v5, v6, ...
    // to be used with drawElements(GL_TRIANGLES, ...)
    void genIndexArray(GLuint* vbi, unsigned int meshId = 0) const;

    // These return the complete expected length of the buffer object
    unsigned int lengthOfSimpleVBO(unsigned int meshId = 0) const;
    unsigned int lengthOfVBO(unsigned int meshId = 0, bool normals = true, bool texcoords = true) const;
    unsigned int lengthOfIndexArray(unsigned int meshId = 0) const;

    //ZUSÄTZLICHE SOA FUNKTIONEN
    size_t* genSOA(GLfloat* vertArray,
                   GLfloat* normArray,
                   GLfloat* texCArray,
                   GLuint*  indexArray,
                   unsigned int meshID = 0);

    // Returns whether the model has texture coordinates
    bool hasTextureCoordinates(unsigned int meshId = 0);

    // Returns whether a model is present
    bool hasScene();

private:
    bool _hasScene;
    const aiScene* scene;
    Assimp::Importer importer;
};

#endif // MODELLOADER_H

