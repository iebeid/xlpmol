
#include "gl_globals.h"
#include <string>

class ShaderManager
{
public:
  ShaderManager();
  void load(const char * vertexshader,
            const char * pixelshader,
            const char * geoshader = NULL);
  int link();
  void bind();

  int  getUniformVarID(const char * name);

  void setUniformVar(const char* name, int varValue);
  void setUniformVar(const char* name, int var[4]);
  void setUniformVar(const char* name, float varValue);
  void setUniformVar(const char* name, float var[4]);

  void setUniformMat3(const char* name, float* array);
  void setUniformMat4(const char* name, float* array);

  void unbind();
  void unload();
  bool isLoaded();

  GLuint programID() const{ return _programID; }

private:
  GLuint loadShader(const char * filename, int type);
  void readEntireFile(std::string* content, const char * filename);
private:
  GLuint _programID;
  GLuint _vertexShaderID;
  GLuint _pixelShaderID;
  GLuint _geoShaderID;
  bool  _isLoaded;

};
