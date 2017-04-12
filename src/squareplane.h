#pragma once

#include "drawable.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class SquarePlane : public Drawable
{
public:
    SquarePlane(GLWidget277* context) : Drawable(context){}
    virtual void create();

    GLenum drawMode();
};
