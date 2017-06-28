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

#include "modelloader.h"
#include <iostream>

ModelLoader::ModelLoader()
    : _hasScene(false)
    , scene(NULL)
{

}

ModelLoader::~ModelLoader()
{
    /* Note: Assimp::Importer cleans up after itself, freeing the scene it previously returned */
}

bool ModelLoader::loadObjectFromFile(const std::string& pFile)
{
    /* TOD_O: this is some weak error handling */
    if (_hasScene) {
        return false;
    }

    // load scene with triangulation, removing identical vertices
    // and generating normals, if necessary
    scene = importer.ReadFile(pFile, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals);

    // check if import was successful
    if(!scene) {
      qDebug() << importer.GetErrorString();
      return false;
    }

    // check if there is at least one mesh
    if (scene->mNumMeshes == 0 || scene->mMeshes[0]->mNumFaces == 0) {
        importer.FreeScene();
        return false;
    }

    // everything checked out
    _hasScene = true;
    return true;
}

bool ModelLoader::hasScene()
{
    return _hasScene;
}

//KAI'S FUNKTIONEN
void ModelLoader::genSimpleVBO(GLfloat* vbo, unsigned int meshId) const
{
    if (!_hasScene) {
        return;
    }
    // copy array and add 4th component
    for (unsigned int i = 0; i < scene->mMeshes[meshId]->mNumVertices; ++i) {
        vbo[i*4+0] = scene->mMeshes[meshId]->mVertices[i].x;
        vbo[i*4+1] = scene->mMeshes[meshId]->mVertices[i].y;
        vbo[i*4+2] = scene->mMeshes[meshId]->mVertices[i].z;
        vbo[i*4+3] = 1.0f;
    }
}

void ModelLoader::genVBO(GLfloat* vbo, unsigned int meshId, bool normals, bool texcoords) const
{
    if (!_hasScene) {
        return;
    }

    // calculate stride
    unsigned int stride = 4;
    if (normals) {
        stride += 4;
    }
    if (texcoords) {
        stride += 4;
    }

    // copy array and add 4th component
    int unsigned n;
    for (unsigned int i = 0; i < scene->mMeshes[meshId]->mNumVertices; ++i) {
        //std::cout << "read vertex number " << i << std::endl;
        n = 0;
        vbo[i*stride+n++] = scene->mMeshes[meshId]->mVertices[i].x;
        vbo[i*stride+n++] = scene->mMeshes[meshId]->mVertices[i].y;
        vbo[i*stride+n++] = scene->mMeshes[meshId]->mVertices[i].z;
        vbo[i*stride+n++] = 1.0f;

        if (normals) {
            vbo[i*stride+n++] = scene->mMeshes[meshId]->mNormals[i].x;
            vbo[i*stride+n++] = scene->mMeshes[meshId]->mNormals[i].y;
            vbo[i*stride+n++] = scene->mMeshes[meshId]->mNormals[i].z;
            vbo[i*stride+n++] = 1.0f;
        }

        if (texcoords) {
            /* TOD_O: auto-selects first UV(W) channel */
            vbo[i*stride+n++] = (scene->mMeshes[meshId]->mTextureCoords[0])[i].x;
            vbo[i*stride+n++] = (scene->mMeshes[meshId]->mTextureCoords[0])[i].y;
            vbo[i*stride+n++] = (scene->mMeshes[meshId]->mTextureCoords[0])[i].z;
            //std::cout << "derp: " << scene->mMeshes[meshId]->mTextureCoords[i]->z << std::endl;
            vbo[i*stride+n++] = 1.0f;
        }
    }
}

void ModelLoader::genIndexArray(GLuint* vbi, unsigned int meshId) const
{
    if (!_hasScene) {
        return;
    }
    // copying this whole array takes time, but just referencing it and keeping the other data is worse
    for (unsigned int i = 0; i < scene->mMeshes[meshId]->mNumFaces; ++i) {
        vbi[i*3+0] = scene->mMeshes[meshId]->mFaces[i].mIndices[0];
        vbi[i*3+1] = scene->mMeshes[meshId]->mFaces[i].mIndices[1];
        vbi[i*3+2] = scene->mMeshes[meshId]->mFaces[i].mIndices[2];
    }
}

unsigned int ModelLoader::lengthOfSimpleVBO(unsigned int meshId) const
{
    if (!_hasScene) {
        return 0;
    }
    return scene->mMeshes[meshId]->mNumVertices * 4;
}

unsigned int ModelLoader::lengthOfVBO(unsigned int meshId, bool normals, bool texcoords) const
{
    unsigned int res = scene->mMeshes[meshId]->mNumVertices * 4;
    if (normals) {
        res += scene->mMeshes[meshId]->mNumVertices * 4;
    }
    if (texcoords) {
        res += scene->mMeshes[meshId]->mNumVertices * 4;
    }
    return res;
}

unsigned int ModelLoader::lengthOfIndexArray(unsigned int meshId) const
{
    if (!_hasScene || scene->mMeshes[meshId]->mNumFaces == 0) {
        return 0;
    }
    return scene->mMeshes[meshId]->mNumFaces * scene->mMeshes[meshId]->mFaces[0].mNumIndices;
}

//ZUSÄTZLICHE SOA FUNKTIONEN
size_t* ModelLoader::genSOA(GLfloat* vertArray,
                            GLfloat* normArray,
                            GLfloat* texCArray,
                            GLuint*  indexArray,
                            unsigned int meshID) {

    if (!_hasScene) return nullptr;

    if(!scene->mMeshes[meshID]) return nullptr;

    aiMesh* myMesh  = scene->mMeshes[meshID];

    size_t vertArraySize  = myMesh->mNumVertices * 3;
    size_t normArraySize  = myMesh->mNumVertices * 3;
    size_t texCArraySize  = myMesh->mNumVertices * 2;
    size_t indexArraySize = myMesh->mNumFaces * 3;

    vertArray = new GLfloat[vertArraySize];
    normArray = new GLfloat[normArraySize];
    texCArray = new GLfloat[texCArraySize];
    indexArray = new GLuint[indexArraySize];

    //vert norm und tex Vec füllen
    for (unsigned int i = 0; i < myMesh->mNumVertices; i++) {
        //std::cout << "read vertex number " << i << std::endl;

        vertArray[i+0] = myMesh->mVertices[i].x;
        vertArray[i+1] = myMesh->mVertices[i].y;
        vertArray[i+2] = myMesh->mVertices[i].z;

        normArray[i+0] = myMesh->mNormals[i].x;
        normArray[i+1] = myMesh->mNormals[i].y;
        normArray[i+2] = myMesh->mNormals[i].z;

        texCArray[i+0] = (myMesh->mTextureCoords[0])[i].x;
        texCArray[i+1] = (myMesh->mTextureCoords[0])[i].y;
    }

    //IndexVec füllen (GL_TRIANGLES)
    for (unsigned int i = 0 ; i < myMesh->mNumFaces ; i++) {
        const aiFace& Face = myMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        indexArray[i+0] = Face.mIndices[0];
        indexArray[i+1] = Face.mIndices[1];
        indexArray[i+2] = Face.mIndices[2];
    }
    size_t* result = new size_t[4];
    result[0] = vertArraySize;
    result[1] = normArraySize;
    result[2] = texCArraySize;
    result[3] = indexArraySize;

    return result;
}

bool ModelLoader::hasTextureCoordinates(unsigned int meshId)
{
    /* TOD_O: assumes first UV(W) channel */
    return scene->mMeshes[meshId]->HasTextureCoords(0);
}


