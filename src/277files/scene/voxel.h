#pragma once

#include <shaderprogram.h>
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Voxel : public ShaderProgram::Drawable
{
 private:
  int count;
  QOpenGLBuffer bufIdx;
  QOpenGLBuffer bufPos;
  QOpenGLBuffer bufNor;
  QOpenGLBuffer bufCol; // Can be used to pass per-vertex color information to the shader, but is currently unused.

  // Store corresponding row/column info from the img slice

  int rowID;
  int columnID;
  double lengthID;
  int arrayID;

  static const int VOXEL_IDX_COUNT = 36;
  static const int VOXEL_VERT_COUNT = 24;

 public:
  Voxel();

  void create();
  void destroy();

  virtual GLenum drawMode();
  virtual int elemCount();
  virtual bool bindIdx();
  virtual bool bindPos();
  virtual bool bindNor();
  virtual bool bindCol();

  void set2DAddress(int w, int h, int pos);
  void setLengthAddress(double l);
  int getRowID();
  int getColumnID();
  int getArrayID();
  double getLengthID();

  glm::vec4 voxCol;

  void setVoxelColor(glm::vec4 voxelColor);

  void createVoxelColors(glm::vec4 (&voxel_vert_col)[VOXEL_VERT_COUNT]);
  void setTransform(glm::mat4 trans);
  glm::mat4 getTransform();

  ShaderProgram::Drawable getDrawable();
private:

  glm::mat4 transform = glm::mat4(1.0); //init as identity matrix
};
