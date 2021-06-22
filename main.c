#include <GL4D/gl4dg.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4duw_SDL2.h>
#include <GL4D/gl4dp.h>
#include <stdio.h>

static void init(void);
static void resize(int width, int height);
static void draw(void);
static void sortie(void);

static GLuint _wW = 1280, _wH = 960;
static GLuint _sphereId = 0;
static GLuint _torusId = 0;
static GLuint _pId = 0;
static GLuint _texId = 0;
static GLuint _quadId = 0;

static GLuint _cubeId = 0;

int main(int argc, char ** argv) {
  if(!gl4duwCreateWindow(argc, argv, "bases 3D", GL4DW_POS_CENTERED, GL4DW_POS_CENTERED,
			 _wW, _wH, GL4DW_OPENGL | GL4DW_RESIZABLE | GL4DW_SHOWN)) {
    fprintf(stderr, "Erreur lors de la création de la fenêtre\n");
    return 1;
  }
  init();
  atexit(sortie);
  gl4duwResizeFunc(resize);
  gl4duwDisplayFunc(draw);
  gl4duwMainLoop();
  return 0;
}

void init(void) {
  GLuint pixels[] = {
    RGB(255, 100, 100), RGB(100, 255, 100), RGB(100, 100, 255), RGB(255, 255, 100),
    RGB(100, 100, 255), RGB(255, 255, 100), RGB(255, 255, 255), RGB(255, 100, 100), 
    RGB(255, 255, 100), RGB(255, 100, 100), RGB(100, 255, 100), RGB(100, 100, 255)
  };
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  _sphereId = gl4dgGenSpheref(10, 10);
  _torusId = gl4dgGenTorusf(10, 10, 0.3f);
  _quadId = gl4dgGenQuadf();
  _cubeId = gl4dgGenCubef();
  _pId = gl4duCreateProgram("<vs>shaders/hello.vs", "<fs>shaders/hello.fs", NULL);
  glEnable(GL_DEPTH_TEST);
  /* glEnable(GL_CULL_FACE); */
  /* glCullFace(GL_FRONT); */
  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
  gl4duGenMatrix(GL_FLOAT, "proj");
  gl4duGenMatrix(GL_FLOAT, "mod");
  gl4duGenMatrix(GL_FLOAT, "view");
  glGenTextures(1, &_texId);
  glBindTexture(GL_TEXTURE_2D, _texId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4, 3, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  glBindTexture(GL_TEXTURE_2D, 0);
  glViewport(0, 0, _wW, _wH );
  resize(_wW, _wH);
}

static void resize(int width, int height) {
  GLfloat ratio;
  _wW = width;
  _wH = height;
  glViewport(0, 0, _wW, _wH);
  ratio = _wW / ((GLfloat)_wH);
  gl4duBindMatrix("proj");
  gl4duLoadIdentityf();
  /* gl4duOrthof(-1 * ratio, 1 * ratio, -1 /\* / ratio *\/, 1 /\* / ratio *\/, -1, 1); */
  gl4duFrustumf(-1 * ratio, 1 * ratio, -1 /* / ratio */, 1 /* / ratio */, 2, 100);

}

void scene(GLfloat dt) {
  static GLfloat x = 0.0f;
  GLfloat lumpos0[] = { -5.0f, 12.0f, -30.0f, 1.0f }, lumpos[4], * mat;
  lumpos0[2] = 4.0f * sin(x) ;
  x += dt;
  gl4duBindMatrix("view");
  mat = (GLfloat *)gl4duGetMatrixData();
  MMAT4XVEC4(lumpos, mat, lumpos0);

  GLfloat rouge[] = {1.0f, 0.0f, 0.0f, 1.0f}, jaune[] = { 1.0f,1.0f, 0, 1.0f}, blanc[] = {1.0f, 1.0f, 1.0f, 1.0f};
  gl4duBindMatrix("mod");
  gl4duLoadIdentityf();
  gl4duScalef(4, 3, 4);
  gl4duRotatef(90, 0, 1, 0);
  glUseProgram(_pId);
  gl4duSendMatrices();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texId);
  glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
  glUniform4fv(glGetUniformLocation(_pId, "lumpos"), 1, lumpos);
  glUniform4fv(glGetUniformLocation(_pId, "scolor"), 1, rouge);
  glUniform4fv(glGetUniformLocation(_pId, "lcolor"), 1, blanc);
  gl4dgDraw(_quadId);

  gl4duBindMatrix("mod");
  gl4duLoadIdentityf();
  gl4duScalef(0.15, 0.2, 0.7);
  gl4duTranslatef(1, 3, 3);
  glUseProgram(_pId);
  gl4duSendMatrices();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texId);
  glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
  glUniform4fv(glGetUniformLocation(_pId, "lumpos"), 1, lumpos);
  glUniform4fv(glGetUniformLocation(_pId, "scolor"), 1, jaune);
  glUniform4fv(glGetUniformLocation(_pId, "lcolor"), 1, blanc);
  gl4dgDraw(_cubeId);
  
  gl4duBindMatrix("mod");
  gl4duLoadIdentityf();
  gl4duScalef(0.5, 0.2, 0.2);
  gl4duTranslatef(1.5,  3,12);
  glUseProgram(_pId);
  gl4duSendMatrices();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texId);
  glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
  glUniform4fv(glGetUniformLocation(_pId, "lumpos"), 1, lumpos);
  glUniform4fv(glGetUniformLocation(_pId, "scolor"), 1, jaune);
  glUniform4fv(glGetUniformLocation(_pId, "lcolor"), 1, blanc);
  gl4dgDraw(_cubeId);
  
  gl4duBindMatrix("mod");
  gl4duLoadIdentityf();
  gl4duScalef(0.15, 0.2, 0.7);
  gl4duTranslatef(8, 3, 3);
  glUseProgram(_pId);
  gl4duSendMatrices();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texId);
  glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
  glUniform4fv(glGetUniformLocation(_pId, "lumpos"), 1, lumpos);
  glUniform4fv(glGetUniformLocation(_pId, "scolor"), 1, jaune);
  glUniform4fv(glGetUniformLocation(_pId, "lcolor"), 1, blanc);
  gl4dgDraw(_cubeId);
  
  gl4duBindMatrix("mod");
  gl4duLoadIdentityf();
  gl4duScalef(0.5, 0.2, 0.2);
  gl4duTranslatef(1.4,  3,7);
  glUseProgram(_pId);
  gl4duSendMatrices();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texId);
  glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
  glUniform4fv(glGetUniformLocation(_pId, "lumpos"), 1, lumpos);
  glUniform4fv(glGetUniformLocation(_pId, "scolor"), 1, jaune);
  glUniform4fv(glGetUniformLocation(_pId, "lcolor"), 1, blanc);
  gl4dgDraw(_cubeId);
  

  
  gl4duBindMatrix("mod");
  gl4duLoadIdentityf();
  gl4duScalef(0.15, 0.2, 0.5);
  gl4duTranslatef(1, 3, -3);
  glUseProgram(_pId);
  gl4duSendMatrices();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texId);
  glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
  glUniform4fv(glGetUniformLocation(_pId, "lumpos"), 1, lumpos);
  glUniform4fv(glGetUniformLocation(_pId, "scolor"), 1, jaune);
  glUniform4fv(glGetUniformLocation(_pId, "lcolor"), 1, blanc);
  gl4dgDraw(_cubeId);

  
  gl4duBindMatrix("mod");
  gl4duLoadIdentityf();
  gl4duScalef(0.2, 0.2, 0.2);
  gl4duTranslatef(2,  3,-9);
  glUseProgram(_pId);
  gl4duSendMatrices();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texId);
  glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
  glUniform4fv(glGetUniformLocation(_pId, "lumpos"), 1, lumpos);
  glUniform4fv(glGetUniformLocation(_pId, "scolor"), 1, jaune);
  glUniform4fv(glGetUniformLocation(_pId, "lcolor"), 1, blanc);
  gl4dgDraw(_cubeId);

  gl4duBindMatrix("mod");
  gl4duLoadIdentityf();
  gl4duScalef(0.15, 0.2, 0.5);
  gl4duTranslatef(5, 3, -3);
  glUseProgram(_pId);
  gl4duSendMatrices();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texId);
  glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
  glUniform4fv(glGetUniformLocation(_pId, "lumpos"), 1, lumpos);
  glUniform4fv(glGetUniformLocation(_pId, "scolor"), 1, jaune);
  glUniform4fv(glGetUniformLocation(_pId, "lcolor"), 1, blanc);
  gl4dgDraw(_cubeId);

  gl4duBindMatrix("mod");
  gl4duLoadIdentityf();
  gl4duScalef(0.2, 0.2, 0.2);
  gl4duTranslatef(5.5,  3,-6);
  glUseProgram(_pId);
  gl4duSendMatrices();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texId);
  glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
  glUniform4fv(glGetUniformLocation(_pId, "lumpos"), 1, lumpos);
  glUniform4fv(glGetUniformLocation(_pId, "scolor"), 1, jaune);
  glUniform4fv(glGetUniformLocation(_pId, "lcolor"), 1, blanc);
  gl4dgDraw(_cubeId);

   gl4duBindMatrix("mod");
  gl4duLoadIdentityf();
  gl4duScalef(0.15, 0.2, 0.5);
  gl4duTranslatef(9, 3, -3);
  glUseProgram(_pId);
  gl4duSendMatrices();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texId);
  glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
  glUniform4fv(glGetUniformLocation(_pId, "lumpos"), 1, lumpos);
  glUniform4fv(glGetUniformLocation(_pId, "scolor"), 1, jaune);
  glUniform4fv(glGetUniformLocation(_pId, "lcolor"), 1, blanc);
  gl4dgDraw(_cubeId);

  /*gl4duBindMatrix("mod");
  gl4duLoadIdentityf();
  gl4duTranslatef(3.0f, 0, 0);
  //gl4duRotatef(70, 1, 0, 0);
  gl4duSendMatrices();
  glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
  glUniform4fv(glGetUniformLocation(_pId, "scolor"), 1, bleu);
  glUniform4fv(glGetUniformLocation(_pId, "lcolor"), 1, blanc);
  gl4dgDraw(_quadId);*/


  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D, 0);

}

void draw(void) {
  static double t0 = 0.0;
  double t = gl4dGetElapsedTime() / 1000.0, dt = t - t0;
  t0 = t;
  static GLfloat a = 0.0f, y = 0.0f;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  
  gl4duBindMatrix("view");
  gl4duLoadIdentityf();
  gl4duLookAtf(4.0f,- 5, 0.0f, 0, 0, 0, 0, 1.0f, 0);
  scene(dt);

  a += 180.0f * dt;
  //y += 0.5f * dt;
}

void sortie(void) {
  if(_texId) {
    glDeleteTextures(1, &_texId);
    _texId = 0;
  }
  gl4duClean(GL4DU_ALL);
}
