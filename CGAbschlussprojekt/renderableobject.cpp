#include "renderableobject.h"

RenderableObject::RenderableObject(QMatrix4x4 ctm,
                                   Model* model,
                                   int shaderTypeFlag,
                                   QOpenGLShaderProgram* shader,
                                   QOpenGLShaderProgram* secondShader,
                                   QVector4D const &baseColor,
                                   QString const &mainTextureFileName,
                                   QString const &secondTextureFileName) {
    //Parameter übertragen/eintragen
    this->_model = model;
    _modelHasTextureCoords = (model->hasTextureCoords()) ? true : false;

    this->_shader = shader;
    _hasTexture = false;
    _hasSecondTexture = false;

    if(secondShader != nullptr) {
        _secondShader = secondShader;
    }

    if(mainTextureFileName.length() != 0) {
        if(!_modelHasTextureCoords) {
            qDebug() << endl << "#WARNING# Function: RenderableObject::RenderableObject - Model hat keine TexturCoords aber Textur wird gesetzt!" << endl;
        }
        _setMainTexture(mainTextureFileName);
    }

    if(secondTextureFileName.length() != 0) {
        _setSecondTexture(secondTextureFileName);
    }

    _myCTM = ctm;
    _shaderFlag = shaderTypeFlag;
    _baseColor = baseColor;

    _renderingCounter1 = 0.f;
    _renderingCounter2 = 0.f;
}

void RenderableObject::_setMainTexture(QString filename) {
    //(ausgelagerte) Hilfsfunktion zum Setzen der Haupt-Textur
    _mainTexture = new QOpenGLTexture(QImage(":/" + filename).mirrored());
    _mainTexture->setMinificationFilter(QOpenGLTexture::NearestMipMapLinear);
    _mainTexture->setMagnificationFilter(QOpenGLTexture::Nearest);
    Q_ASSERT(_mainTexture->textureId() != 0); //Würde Fehler bedeuten
    _hasTexture = true;
    qDebug() << endl << "Textur: " << filename << " geladen" << endl << endl;
}

void RenderableObject::_setSecondTexture(QString filename) {
    //(ausgelagerte) Hilfsfunktion zum Setzen der Zweit-Textur
    _secondTexture = new QOpenGLTexture(QImage(":/" + filename).mirrored());
    _secondTexture->setMinificationFilter(QOpenGLTexture::NearestMipMapLinear);
    _secondTexture->setMagnificationFilter(QOpenGLTexture::Nearest);
    Q_ASSERT(_secondTexture->textureId() != 0); //Würde Fehler bedeuten
    _hasSecondTexture = true;
    qDebug() << endl << "Textur: " << filename << " geladen" << endl << endl;
}

void RenderableObject::_renderWithDefaultShader(QMatrix4x4 const &parentCTM,
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
    int unifColor = 3;

    //Matrix Berechnungen fertig nun shader konfigurieren
    _shader->bind();

    // Lokalisiere bzw. definiere die Schnittstelle für die Eckpunkte, Normalen und Textur
    int attrVertices = 0;
    int attrNorms = 1;

    // Aktiviere die Verwendung der Attribute-Arrays
    _shader->enableAttributeArray(attrVertices);
    _shader->enableAttributeArray(attrNorms);

    // Fülle die Attribute-Buffer mit den korrekten Daten
    size_t model_stride = _model->stride(); //reduziert von 3 auf 1 Aufruf (implizit uint in int ist aktzeptabel - kein Werteverlust)
    _shader->setAttributeBuffer(attrVertices, GL_FLOAT, _model->vertOffset(), 4, model_stride); //VertexPositionen
    _shader->setAttributeBuffer(attrNorms, GL_FLOAT, _model->normOffset(), 4, model_stride); //VertexNormalen

    //Uniforms an den Shader übergeben
    _shader->setUniformValue(unifProjMatrix, projectionMatrix); //projektionsMatrix (const)
    _shader->setUniformValue(unifViewMatrix, viewMatrix); //viewMatrix ("const")
    _shader->setUniformValue(unifModelMatrix, ctm); //modelMatrix (immer abhängig vom gerade zu rendernden RenderableObject)
    _shader->setUniformValue(unifColor, _baseColor);


    //PolygonMode einstellen
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //Element zeichnen lassen (implizit uint in int ist aktzeptabel - kein Werteverlust)
    glDrawElements(GL_TRIANGLES, _model->iboLength(), GL_UNSIGNED_INT, 0);

    //PolygonMode auf default setzen
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Deaktiviere die Verwendung der Attribute-Arrays
    _shader->disableAttributeArray(attrVertices);
    _shader->disableAttributeArray(attrNorms);

    //Shader lösen
    _shader->release();

    //VBO und IBO vom Kontext lösen
    _model->getVBOBufferPtr()->release();
    _model->getIBOBufferPtr()->release();
}

void RenderableObject::_renderWithTextureShader(QMatrix4x4 const &parentCTM,
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
    size_t model_stride = _model->stride(); //reduziert von 3 auf 1 Aufruf (implizit uint in int ist aktzeptabel - kein Werteverlust)
    _shader->setAttributeBuffer(attrVertices, GL_FLOAT, _model->vertOffset(), 4, model_stride); //VertexPositionen
    _shader->setAttributeBuffer(attrNorms, GL_FLOAT, _model->normOffset(), 4, model_stride); //VertexNormalen
    if(_modelHasTextureCoords) {
        _shader->setAttributeBuffer(attrTexCoords, GL_FLOAT, _model->texCoordOffset(), 4, model_stride); //TexturCoordinaten
    }

    //Uniforms an den Shader übergeben
    _shader->setUniformValue(unifProjMatrix, projectionMatrix); //projektionsMatrix (const)
    _shader->setUniformValue(unifViewMatrix, viewMatrix); //viewMatrix ("const")
    _shader->setUniformValue(unifModelMatrix, ctm); //modelMatrix (immer abhängig vom gerade zu rendernden RenderableObject)
    //_shader->setUniformValue(unifNormMatrix,  (viewMatrix * ctm).normalMatrix()); //berechnete Normalenmatrix

    //Haupt-Textur (und Zweit-Textur) binden und an shader übergeben
    _mainTexture->bind(0);
    _shader->setUniformValue("diffuseMap", 0);

    /* atm nicht implementiert im shader
    if(_hasSecondTexture) {
        _secondTexture->bind(1);
        _shader->setUniformValue("bumpMap?!?", 1);
    }
    */

    //PolygonMode einstellen
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //Element zeichnen lassen (implizit uint in int ist aktzeptabel - kein Werteverlust)
    glDrawElements(GL_TRIANGLES, _model->iboLength(), GL_UNSIGNED_INT, 0);

    //PolygonMode auf default setzen
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Deaktiviere die Verwendung der Attribute-Arrays
    _shader->disableAttributeArray(attrVertices);
    _shader->disableAttributeArray(attrNorms);
    if(_modelHasTextureCoords) {
        _shader->disableAttributeArray(attrTexCoords);
    }

    //Shader lösen
    _shader->release();

    // Löse die Textur aus dem OpenGL-Kontext
    if(_hasTexture) {
        _mainTexture->release();
    }
    /* atm nicht implementiert im shader
    if(_hasSecondTexture) {
        _secondTexture->release();
    }
    */

    //VBO und IBO vom Kontext lösen
    _model->getVBOBufferPtr()->release();
    _model->getIBOBufferPtr()->release();
}

void RenderableObject::_renderWithNormalsShader(QMatrix4x4 const &parentCTM,
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


    glDisable(GL_CULL_FACE);

    //VBO und IBO an den Kontext binden
    _model->getVBOBufferPtr()->bind();
    _model->getIBOBufferPtr()->bind();

    //Matrix Locations für den Shader
    int const unifProjMatrix = 0;
    int const unifViewMatrix = 1;
    int const unifModelMatrix = 2;
    int const unifBaseColor = 3;
    //int const unifNormMatrix = 4;
    //int const unifCounter = 5;

    // - - - - - Start Rendering - - - - -

    //SHADER 1 (atm _defaultShaderProgram)

    //Matrix Berechnungen fertig nun shader konfigurieren
    _shader->bind();

    // Lokalisiere bzw. definiere die Schnittstelle für die Eckpunkte, Normalen und Textur
    int attrVertices = 0;
    int attrNorms = 1;

    // Aktiviere die Verwendung der Attribute-Arrays
    _shader->enableAttributeArray(attrVertices);
    _shader->enableAttributeArray(attrNorms);

    // Fülle die Attribute-Buffer mit den korrekten Daten
    size_t model_stride = _model->stride(); //reduziert von 3 auf 1 Aufruf (implizit uint in int ist aktzeptabel - kein Werteverlust)
    _shader->setAttributeBuffer(attrVertices, GL_FLOAT, _model->vertOffset(), 4, model_stride); //VertexPositionen
    _shader->setAttributeBuffer(attrNorms, GL_FLOAT, _model->normOffset(), 4, model_stride); //VertexNormalen

    //Uniforms an den Shader übergeben
    _shader->setUniformValue(unifProjMatrix, projectionMatrix); //projektionsMatrix (const)
    _shader->setUniformValue(unifViewMatrix, viewMatrix); //viewMatrix ("const")
    _shader->setUniformValue(unifModelMatrix, ctm); //modelMatrix (immer abhängig vom gerade zu rendernden RenderableObject)
    _shader->setUniformValue(unifBaseColor, _baseColor);

    //PolygonMode auf Lines
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //Element zeichnen lassen mit Shader 1
    glDrawElements(GL_TRIANGLES, _model->iboLength(), GL_UNSIGNED_INT, 0);

    // Deaktiviere die Verwendung der Attribute-Arrays
    _shader->disableAttributeArray(attrVertices);
    _shader->disableAttributeArray(attrNorms);

    //Shader 1 lösen
    _shader->release();

    //SHADER 2 (_normalDrawShaderProgram)

    //Matrix Berechnungen fertig nun shader konfigurieren
    _secondShader->bind();

    // Aktiviere die Verwendung der Attribute-Arrays
    _secondShader->enableAttributeArray(attrVertices);
    _secondShader->enableAttributeArray(attrNorms);

    // Fülle die Attribute-Buffer mit den korrekten Daten
    _secondShader->setAttributeBuffer(attrVertices, GL_FLOAT, _model->vertOffset(), 4, model_stride); //VertexPositionen
    _secondShader->setAttributeBuffer(attrNorms, GL_FLOAT, _model->normOffset(), 4, model_stride); //VertexNormalen

    //Uniforms an den Shader übergeben
    _secondShader->setUniformValue(unifProjMatrix, projectionMatrix);
    _secondShader->setUniformValue(unifViewMatrix, viewMatrix);
    _secondShader->setUniformValue(unifModelMatrix, ctm);
    _secondShader->setUniformValue("normal_length", 0.4f);
    //_secondShader->setUniformValue(unifNormMatrix,  (viewMatrix * ctm).normalMatrix()); //berechnete Normalenmatrix

    //PolygonMode zurück auf Fill
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //Element zeichnen lassen mit Shader 1
    glDrawElements(GL_TRIANGLES, _model->iboLength(), GL_UNSIGNED_INT, 0);

    // Deaktiviere die Verwendung der Attribute-Arrays
    _secondShader->disableAttributeArray(attrVertices);
    _secondShader->disableAttributeArray(attrNorms);

    //Shader 2 lösen
    _secondShader->release();

    // - - - - - Rendern fertig - - - - -

    //VBO und IBO vom Kontext lösen
    _model->getVBOBufferPtr()->release();
    _model->getIBOBufferPtr()->release();

    glEnable(GL_CULL_FACE);
}

void RenderableObject::render(QMatrix4x4 const &parentCTM,
                              QMatrix4x4 const &viewMatrix,
                              QMatrix4x4 const &projectionMatrix) {
    switch(_shaderFlag) {
    case SHADER_DEFAULT:
        _renderWithDefaultShader(parentCTM, viewMatrix, projectionMatrix);
        break;
    case SHADER_TEXTURE:
        _renderWithTextureShader(parentCTM, viewMatrix, projectionMatrix);
        break;
    case SHADER_NORMALS:
        _renderWithNormalsShader(parentCTM, viewMatrix, projectionMatrix);
        break;
    default:
        qDebug() << endl << "#ERROR# Function: RenderableObject:render(..) - ungültige Shader-Flag!" << endl;
        throw new std::exception();
    }
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
