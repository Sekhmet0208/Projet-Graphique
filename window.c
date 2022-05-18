// Démo Api8 d'un système solaire en mouvement et accompagné d'une musique. 
// Par Fliou Abdelrahman et Florian Posez élèves de L2X

/*!\file window.c
 * \brief géométries lumière diffuse, transformations de base et chargement de textures en GL4Dummies */
#include <stdio.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4df.h>
#include <GL4D/gl4duw_SDL2.h>
#include <SDL_image.h>
#include <SDL.h>
#include <SDL_mixer.h>

/* Prototypes des fonctions statiques contenues dans ce fichier C */
static void init(void);
static void loadTexture(GLuint id, const char * filename);
static void resize(int w, int h);
static void draw(void);
static void quit(void);
/*!\brief dimensions de la fenêtre */
static int _wW = 1200, _wH = 1000;
/*!\brief identifiant du programme GLSL */
static GLuint _pId = 0;
/*!\brief quelques objets géométriques */
static GLuint _lune = 0, _mercure = 0, _venus = 0, _sphere = 0, _cube = 0, _quad = 0, _torus = 0, _mars = 0, _jupiter = 0, _saturne = 0, _uranus = 0, _neptune = 0, _tex[15] = {0};
/*!\brief La fonction principale créé la fenêtre d'affichage,
 * initialise GL et les données, affecte les fonctions d'événements et
 * lance la boucle principale d'affichage.*/
int main(int argc, char ** argv) {
  if(!gl4duwCreateWindow(argc, argv, "GL4Dummies", 0, 0, 
			 _wW, _wH, GL4DW_RESIZABLE | GL4DW_SHOWN))
    return 1;

  init();
  atexit(quit);
  gl4duwResizeFunc(resize);
  gl4duwDisplayFunc(draw);
  gl4duwMainLoop();

  
  return 0;
}
/*!\brief initialise les paramètres OpenGL et les données */
static void init(void) {
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  _pId  = gl4duCreateProgram("<vs>shaders/depTex.vs", "<fs>shaders/depTex.fs", NULL);
  gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");
  gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
  resize(_wW, _wH);
  //Génération des planetes
  _sphere = gl4dgGenSpheref(30, 30);
  _lune = gl4dgGenSpheref(30,30);
  _cube = gl4dgGenCubef();
  _mercure = gl4dgGenSpheref(30,30);
  _venus = gl4dgGenSpheref(30,30);
  _mars = gl4dgGenSpheref(30,30);
  _jupiter = gl4dgGenSpheref(30,30);
  _saturne = gl4dgGenSpheref(30,30);
  _uranus = gl4dgGenSpheref(30,30);
  _neptune = gl4dgGenSpheref(30,30);
  _quad = gl4dgGenQuadf();
  _torus = gl4dgGenTorusf(300, 30, 0.1f);
  /* génération de plusieurs identifiants de texture */
  glGenTextures(sizeof _tex / sizeof *_tex, _tex);
  /* chargement et transfert des images dans des textures OpenGL */
  loadTexture(_tex[0], "images/sphere.jpg");
  loadTexture(_tex[1], "images/cube.jpg");
  loadTexture(_tex[2], "images/quad.jpg");
  loadTexture(_tex[3], "images/terre.jpg");
  loadTexture(_tex[6], "images/torus.jpg");
  //Mercure
  loadTexture(_tex[7], "images/mercure.jpg");
  //Venus
  loadTexture(_tex[8], "images/venus.jpg");
  //Mars
  loadTexture(_tex[9], "images/mars.jpg");
  //Jupiter
  loadTexture(_tex[10], "images/jupiter.jpg");
  //Saturne
  loadTexture(_tex[11], "images/saturne.jpg");
  //Uranus
  loadTexture(_tex[12], "images/uranus.jpg");
  //Neptune
  loadTexture(_tex[13], "images/neptune.jpg");




  /* une dernière texture vide pour y faire des calculs d'effets 
  glBindTexture(GL_TEXTURE_2D, _tex[4]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _wW / 2, _wH, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glEnable(GL_TEXTURE_2D);*/
}
static void loadTexture(GLuint id, const char * filename) {
  SDL_Surface * t;
  glBindTexture(GL_TEXTURE_2D, id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  if( (t = IMG_Load(filename)) != NULL ) {
#ifdef __APPLE__
    int mode = t->format->BytesPerPixel == 4 ? GL_BGRA : GL_BGR;
#else
    int mode = t->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
#endif       
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->w, t->h, 0, mode, GL_UNSIGNED_BYTE, t->pixels);
    SDL_FreeSurface(t);
  } else {
    fprintf(stderr, "can't open file %s : %s\n", filename, SDL_GetError());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  }
}
/*!\brief Cette fonction paramétre la vue (viewport) OpenGL en
 * fonction des dimensions de la fenêtre.*/
static void resize(int w, int h) {
  _wW  = w; _wH = h;
  glViewport(0, 0, _wW, _wH);
  gl4duBindMatrix("projectionMatrix");
  gl4duLoadIdentityf();
  gl4duFrustumf(-0.5, 0.5, -0.5 * _wH / _wW, 0.5 * _wH / _wW, 1.0, 1000.0);
  gl4duBindMatrix("modelViewMatrix");
}
/*!\brief dessine dans le contexte OpenGL actif. */
static void draw(void) {
  static GLfloat a = 0;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  gl4duBindMatrix("modelViewMatrix");
  gl4duLoadIdentityf();
  glUseProgram(_pId);
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(glGetUniformLocation(_pId, "tex"), 0);
// Plan
  gl4duTranslatef(0, -1, -20.0);
  gl4duPushMatrix(); {
    gl4duTranslatef(0, -10000000.0, 0.0);
    gl4duRotatef(-90, 1, 0, 0);
    gl4duScalef(5.0f, 5, 5);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glBindTexture(GL_TEXTURE_2D, _tex[2]);
  gl4dgDraw(_quad);
  // Soleil
  gl4duTranslatef(0, -1, 0);
  gl4duSendMatrices();
  glBindTexture(GL_TEXTURE_2D, _tex[0]);
  gl4dgDraw(_sphere);

 //Mercure
  gl4duPushMatrix(); { 
    gl4duRotatef(2.3*a, 0, 1, 0);
  gl4duTranslatef(2, 0, 0);
    gl4duScalef(0.3f, 0.3f, 0.3f);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glBindTexture(GL_TEXTURE_2D, _tex[7]);
  gl4dgDraw(_mercure);
//Venus
    gl4duPushMatrix(); { 
    gl4duRotatef(2*a, 0, 1, 0);
  gl4duTranslatef(4, 0, 0);
    gl4duScalef(0.8f, 0.8f, 0.8f);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glBindTexture(GL_TEXTURE_2D, _tex[8]);
  gl4dgDraw(_venus);

  //Mars
    gl4duPushMatrix(); { 
    gl4duRotatef(1.7*a, 0, 1, 0);
  gl4duTranslatef(8, 0, 0);
    gl4duScalef(0.7f, 0.7f, 0.7f);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glBindTexture(GL_TEXTURE_2D, _tex[9]);
  gl4dgDraw(_mars);

  //Jupiter

      gl4duPushMatrix(); { 
    gl4duRotatef(1.4*a, 0, 1, 0);
  gl4duTranslatef(10, 0, 0);
    gl4duScalef(1.4f, 1.4f, 1.4f);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glBindTexture(GL_TEXTURE_2D, _tex[10]);
  gl4dgDraw(_jupiter);

  //Saturne
      gl4duPushMatrix(); { 
    gl4duRotatef(1.1*a, 0, 1, 0);
  gl4duTranslatef(12, 0, 0);
    gl4duScalef(1.2f, 1.2f, 1.2f);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glBindTexture(GL_TEXTURE_2D, _tex[11]);
  gl4dgDraw(_saturne);



  //Uranus

      gl4duPushMatrix(); { 
    gl4duRotatef(0.8*a, 0, 1, 0);
  gl4duTranslatef(14, 0, 0);
    gl4duScalef(1.0f, 1.0f, 1.0f);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glBindTexture(GL_TEXTURE_2D, _tex[12]);
  gl4dgDraw(_uranus);
//Neptune
      gl4duPushMatrix(); { 
    gl4duRotatef(0.5*a, 0, 1, 0);
  gl4duTranslatef(16, 0, 0);
    gl4duScalef(1.0f, 1.0f, 1.0f);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glBindTexture(GL_TEXTURE_2D, _tex[13]);
  gl4dgDraw(_neptune);

  //Terre
  gl4duRotatef(1.3*a--, 0, 1, 0);
  gl4duTranslatef(6, 0, 0);
  gl4duPushMatrix(); {
    gl4duScalef(0.4f, 0.4f, 0.4f);
    gl4duSendMatrices();
  } gl4duPopMatrix();
  glBindTexture(GL_TEXTURE_2D, _tex[1]);
  gl4duRotatef(-2 * a++, 0, 1, 0);
  gl4duSendMatrices();
  glBindTexture(GL_TEXTURE_2D, _tex[3]);
  gl4dgDraw(_sphere);

 //Lune 
  gl4duRotatef(2 * a++, 1, 0, 1);
  gl4duPushMatrix(); {
    gl4duTranslatef(0, -1.6, 0.0);
    gl4duRotatef(-90, 1, 0, 0);
    gl4duScalef(0.3f, 0.3, 0.3);
    gl4duSendMatrices();
  } gl4duPopMatrix();  
  glBindTexture(GL_TEXTURE_2D, _tex[2]);
  gl4dgDraw(_lune);



  /* une série de filtres GL4D pour obtenir un effet sur la moitié de l'écran */
/*   glViewport(0, 0, _wW / 2, _wH);
  gl4dfBlur(0, _tex[4], 3, 1, 0, GL_FALSE);
  gl4dfSobelSetResultMode(GL4DF_SOBEL_RESULT_INV_LUMINANCE);
  gl4dfSobelSetMixMode(GL4DF_SOBEL_MIX_NONE);
  gl4dfSobel(_tex[4], _tex[4], GL_FALSE);
  gl4dfScattering(0, 0, 5, 0, 0, GL_FALSE);
  gl4dfMedian(0, 0, 3, GL_FALSE);
  gl4dfOpSetOp(GL4DF_OP_MULT);
  gl4dfOp(0, _tex[4], 0, GL_FALSE);
  glViewport(0, 0, _wW, _wH); */
}
/*!\brief appelée au moment de sortir du programme (atexit), libère les éléments utilisés */
static void quit(void) {
  /* suppression de plusieurs identifiants de texture */
  glDeleteTextures(sizeof _tex / sizeof *_tex, _tex);
  gl4duClean(GL4DU_ALL);
}
