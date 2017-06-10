#include "renderableobject.h"

RenderableObject::RenderableObject(QMatrix4x4 ctm,
                                   Model* model,
                                   QOpenGLShaderProgram* shader,
                                   QString const &mainTextureFileName,
                                   QString const &secondTextureFileName) {
    //Parameter übertragen/eintragen
    this->_model = model;
    this->_shader = shader;
    _hasTexture = false;
    _hasSecondTexture = false;

    if(mainTextureFileName.length() != 0) {
        _setMainTexture(mainTextureFileName);
    }

    if(secondTextureFileName.length() != 0) {
        _setSecondTexture(secondTextureFileName);
    }

    _myCTM = ctm;
}

void RenderableObject::_setMainTexture(QString filename) {
    //(ausgelagerte) Hilfsfunktion zum Setzen der Haupt-Textur
    _mainTexture = new QOpenGLTexture(QImage(":/" + filename).mirrored());
    _mainTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    _mainTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    Q_ASSERT(_mainTexture->textureId() != 0); //Würde Fehler bedeuten
    _hasTexture = true;
    qDebug() << "Textur: " << filename << " geladen";
}

void RenderableObject::_setSecondTexture(QString filename) {
    //(ausgelagerte) Hilfsfunktion zum Setzen der Zweit-Textur
    _secondTexture = new QOpenGLTexture(QImage(":/" + filename).mirrored());
    _secondTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    _secondTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    Q_ASSERT(_secondTexture->textureId() != 0); //Würde Fehler bedeuten
    _hasSecondTexture = true;
    qDebug() << "Textur: " << filename << " geladen";
}

void RenderableObject::render(QMatrix4x4 parentCTM,
                              QMatrix4x4 const &viewMatrix,
                              QMatrix4x4 const &projectionMatrix) {

    //TODO CTM Matrix Transformationen
    QMatrix4x4 ctm;
    ctm = parentCTM * _myCTM;

    for(RenderableObject* x : _children) {
        x->render(ctm,
                  viewMatrix,
                  projectionMatrix);
    }

    //VBO und IBO an den Kontext binden
    _model->getVBOBufferPtr()->bind();
    _model->getIBOBufferPtr()->bind();

    //Matrix Locations für den Shader
    int unifProjMatrix = 0;
    int unifViewMatrix = 1;
    int unifModelMatrix = 2;
    //int unifNormMatrix = 3;

    //Matrix Berechnungen fertig nun shader konfigurieren
    _shader->bind();

    // Lokalisiere bzw. definiere die Schnittstelle für die Eckpunkte, Normalen und Textur
    int attrVertices = 0;
    int attrNorms = 1;
    int attrTexCoords = 2;

    // Aktiviere die Verwendung der Attribute-Arrays
    _shader->enableAttributeArray(attrVertices);
    _shader->enableAttributeArray(attrNorms);
    if(_model->hasTextureCoords()) {
        _shader->enableAttributeArray(attrTexCoords);
    }

    // Fülle die Attribute-Buffer mit den korrekten Daten
    size_t model_stride = _model->stride(); //reduziert von 3 auf 1 Aufruf
    _shader->setAttributeBuffer(attrVertices, GL_FLOAT, _model->vertOffset(), 4, model_stride); //VertexPositionen
    _shader->setAttributeBuffer(attrNorms, GL_FLOAT, _model->normOffset(), 4, model_stride); //VertexNormalen
    if(_model->hasTextureCoords()) {
        _shader->setAttributeBuffer(attrTexCoords, GL_FLOAT, _model->texCoordOffset(), 4, model_stride); //TexturCoordinaten
    }

    //Uniforms an den Shader übergeben
    _shader->setUniformValue(unifProjMatrix, projectionMatrix); //projektionsMatrix (const)
    _shader->setUniformValue(unifViewMatrix, viewMatrix); //viewMatrix ("const")
    _shader->setUniformValue(unifModelMatrix, ctm); //modelMatrix (immer abhängig vom gerade zu rendernden RenderableObject)
    //_shader->setUniformValue(unifNormMatrix,  (viewMatrix * ctm).normalMatrix()); //berechnete Normalenmatrix

    //Haupt-Textur (und Zweit-Textur) binden und an shader übergeben
    if(_hasTexture) {
        _mainTexture->bind(0);
        _shader->setUniformValue("diffuseMap", 0);
    }
    if(_hasSecondTexture) {
        _secondTexture->bind(1);
        _shader->setUniformValue("bumpMap?!?", 1);
    }

    //Element zeichnen lassen
    glDrawElements(GL_TRIANGLES, _model->iboLength(), GL_UNSIGNED_INT, 0);

    // Deaktiviere die Verwendung der Attribute-Arrays
    _shader->disableAttributeArray(attrVertices);
    _shader->disableAttributeArray(attrNorms);
    if(_model->hasTextureCoords()) {
        _shader->disableAttributeArray(attrTexCoords);
    }

    //Shader lösen
    _shader->release();

    // Löse die Textur aus dem OpenGL-Kontext
    if(_hasTexture) {
        _mainTexture->release();
    }
    if(_hasSecondTexture) {
        _secondTexture->release();
    }

    //VBO und IBO vom Kontext lösen
    _model->getVBOBufferPtr()->release();
    _model->getIBOBufferPtr()->release();
}

void RenderableObject::addChild(RenderableObject* child) {
    _children.push_back(child);
}

bool RenderableObject::hasTexture() const {
    return _hasTexture;
}

bool RenderableObject::hasSecondTexture() const {
    return _hasSecondTexture;
}
