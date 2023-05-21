#include <stdio.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4dp.h>
#include <GL4D/gl4duw_SDL2.h>
#include <SDL_mixer.h>
#include <GL4D/gl4df.h>
#include <SDL_image.h>
#include <SDL.h>
#include <unistd.h>

	#define ANIMATION_DURATION 12.0
	#define ANIM_DURATION 10.0

	static double _time = 0.0;

	static void init(void);
	static void loadTexture(GLuint id, const char * filename);
	static void resize(int w, int h);
	static void draw(void);
	static void quit(void);
	static int _wW = 1200, _wH = 1000;
	static GLuint _pId = 0;
	static GLuint _lune = 0, 
	_credit =0,
	_ciel = 0,
	_mercure = 0, 
	_venus = 0, 
	_sphere = 0, 
	_cube = 0, 
	_quad = 0, 
	_mars = 0, 
	_jupiter = 0, 
	_saturne = 0, 
	_uranus = 0, 
	_neptune = 0, 
	_tex[18] = {0};
	static GLuint _screen = 0;
	static Mix_Music * _mmusic = NULL;
	#define ECHANTILLONS 1024

	static void initAudio(const char * filename) {
		int mixFlags = MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_MOD, res;
		res = Mix_Init(mixFlags);
		if( (res & mixFlags) != mixFlags ) {
			fprintf(stderr, "Mix_Init: Erreur lors de l'initialisation de la bibliotheque SDL_Mixer\n");
			fprintf(stderr, "Mix_Init: %s\n", Mix_GetError());
		}
		if(Mix_OpenAudio(44100, AUDIO_S16LSB, 2, 1024) < 0)
			exit(4);
		if(!(_mmusic = Mix_LoadMUS(filename))) {
			fprintf(stderr, "Erreur lors du Mix_LoadMUS: %s\n", Mix_GetError());
			exit(5);
		}
		Mix_SetPostMix(NULL, NULL);
		if(!Mix_PlayingMusic())
			Mix_PlayMusic(_mmusic, 1);
	} 
	static void debute(const char * filename) {
		glViewport(0, 0, _wW, _wH);
		_screen = gl4dpInitScreen();
		initAudio(filename);
	}
	static void init(void) {
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		_pId = gl4duCreateProgram("<vs>shaders/depTex.vs", "<fs>shaders/depTex.fs", NULL);
		gl4duGenMatrix(GL_FLOAT, "modelViewMatrix");
		gl4duGenMatrix(GL_FLOAT, "projectionMatrix");
		resize(_wW, _wH);

		_credit =gl4dgGenQuadf();
		_ciel =gl4dgGenQuadf();
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

		glGenTextures(sizeof _tex / sizeof *_tex, _tex);
		loadTexture(_tex[1], "images/cube.jpg");
		loadTexture(_tex[0], "images/sphere.jpg");
		loadTexture(_tex[2], "images/quad.jpg");
		loadTexture(_tex[3], "images/terre.jpg");
		loadTexture(_tex[7], "images/mercure.jpg");
		loadTexture(_tex[8], "images/venus.jpg");
		loadTexture(_tex[9], "images/mars.jpg");
		loadTexture(_tex[10], "images/jupiter.jpg");
		loadTexture(_tex[11], "images/saturne.jpg");
		loadTexture(_tex[12], "images/uranus.jpg");
		loadTexture(_tex[13], "images/neptune.jpg");
		loadTexture(_tex[14], "images/ciel.jpg");
		loadTexture(_tex[17], "images/scu.jpg");
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

	static void resize(int w, int h) {
		_wW = w; _wH = h;
		glViewport(0, 0, _wW, _wH);
		gl4duBindMatrix("projectionMatrix");
		gl4duLoadIdentityf();
		gl4duFrustumf(-0.5, 0.5, -0.5 * _wH / _wW, 0.5 * _wH / _wW, 1.0, 1000.0);
		gl4duBindMatrix("modelViewMatrix");
	}

	static void displayCredits(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		gl4duPushMatrix(); {
		gl4duTranslatef(-1.0f, 5.0f, -12.0f);
		gl4duRotatef(190, 100, 10, 1);
		gl4duScalef(19,20.0f, 0.0f);
		gl4duSendMatrices();
		} gl4duPopMatrix();
		glBindTexture(GL_TEXTURE_2D, _tex[17]);
		gl4dgDraw(_credit);

		}

		static void draw(void) {
			double currentTime = SDL_GetTicks() / 1000.0;
			_time = currentTime;
			if(_time> ANIM_DURATION && _time< ANIMATION_DURATION) {
			displayCredits();
			return;
			}

			else if (_time > ANIMATION_DURATION) {
				exit(0);
				return;
			}

			static GLfloat a = 0;
			a = _time * 1500.0 / ANIMATION_DURATION; 
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			gl4duBindMatrix("modelViewMatrix");
			gl4duLoadIdentityf();
			glUseProgram(_pId);
			glActiveTexture(GL_TEXTURE0);
			glUniform1i(glGetUniformLocation(_pId, "tex"), 0);

			gl4duTranslatef(0, 0, -20.0);
			gl4duPushMatrix(); {
				gl4duTranslatef(0, -10000000.0, 0.0);
				gl4duRotatef(-90, 1, 0, 0);
				gl4duScalef(5.0f, 5, 5);
				gl4duSendMatrices();
			} gl4duPopMatrix();
			glBindTexture(GL_TEXTURE_2D, _tex[2]);
			gl4dgDraw(_quad);

				gl4duTranslatef(0, 0, -10.0);
			gl4duPushMatrix(); {
				gl4duTranslatef(0, 0.0, -100.0);
				gl4duRotatef(0.01*a, 0, 0, 1);
				gl4duScalef(90.0f,90, 90);
				gl4duSendMatrices();
			} gl4duPopMatrix();
			glBindTexture(GL_TEXTURE_2D, _tex[14]);
			gl4dgDraw(_ciel);

			gl4duTranslatef(0, -1, 0);
			gl4duSendMatrices();
			glBindTexture(GL_TEXTURE_2D, _tex[0]);
			gl4dgDraw(_sphere);

			gl4duPushMatrix(); { 
				gl4duRotatef(2.3*a, 0, 1, 0);
				gl4duTranslatef(2, 0, 0);
				gl4duScalef(0.3f, 0.3f, 0.3f);
				gl4duSendMatrices();
			} gl4duPopMatrix();
			glBindTexture(GL_TEXTURE_2D, _tex[7]);
			gl4dgDraw(_mercure);

				gl4duPushMatrix(); { 
				gl4duRotatef(2*a, 0, 1, 0);
				gl4duTranslatef(4, 0, 0);
				gl4duScalef(0.8f, 0.8f, 0.8f);
				gl4duSendMatrices();
			} gl4duPopMatrix();
			glBindTexture(GL_TEXTURE_2D, _tex[8]);
			gl4dgDraw(_venus);

				gl4duPushMatrix(); { 
				gl4duRotatef(1.7*a, 0, 1, 0);
				gl4duTranslatef(8, 0, 0);
				gl4duScalef(0.7f, 0.7f, 0.7f);
				gl4duSendMatrices();
			} gl4duPopMatrix();
			glBindTexture(GL_TEXTURE_2D, _tex[9]);
			gl4dgDraw(_mars);

				gl4duPushMatrix(); { 
				gl4duRotatef(1.4*a, 0, 1, 0);
				gl4duTranslatef(10, 0, 0);
				gl4duScalef(1.4f, 1.4f, 1.4f);
				gl4duSendMatrices();
			} gl4duPopMatrix();
			glBindTexture(GL_TEXTURE_2D, _tex[10]);
			gl4dgDraw(_jupiter);

				gl4duPushMatrix(); { 
				gl4duRotatef(1.1*a, 0, 1, 0);
				gl4duTranslatef(12, 0, 0);
				gl4duScalef(1.2f, 1.2f, 1.2f);
				gl4duSendMatrices();
			} gl4duPopMatrix();
			glBindTexture(GL_TEXTURE_2D, _tex[11]);
			gl4dgDraw(_saturne);

				gl4duPushMatrix(); { 
				gl4duRotatef(0.8*a, 0, 1, 0);
				gl4duTranslatef(14, 0, 0);
				gl4duScalef(1.0f, 1.0f, 1.0f);
				gl4duSendMatrices();
			} gl4duPopMatrix();
			glBindTexture(GL_TEXTURE_2D, _tex[12]);
			gl4dgDraw(_uranus);

				gl4duPushMatrix(); { 
				gl4duRotatef(0.5*a, 0, 1, 0);
				gl4duTranslatef(16, 0, 0);
				gl4duScalef(1.0f, 1.0f, 1.0f);
				gl4duSendMatrices();
			} gl4duPopMatrix();
			glBindTexture(GL_TEXTURE_2D, _tex[13]);
			gl4dgDraw(_neptune);

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

				gl4duRotatef(2 * a++, 1, 0, 1);
				gl4duPushMatrix(); {
				gl4duTranslatef(0, -1.6, 0.0);
				gl4duRotatef(-90, 1, 0, 0);
				gl4duScalef(0.3f, 0.3, 0.3);
				gl4duSendMatrices();
			} gl4duPopMatrix(); 
			glBindTexture(GL_TEXTURE_2D, _tex[2]);
			gl4dgDraw(_lune);
		}


int main(int argc, char ** argv) {
	if(argc != 2) {
		fprintf(stderr, "usage: %s <audio_file>\n", argv[0]);
		return 2;
	}
	if(!gl4duwCreateWindow(argc, argv, "GL4Dummies", 10, 10, 
		_wW, _wH, GL4DW_SHOWN))
		return 1;
			init();

	debute(argv[1]);
	atexit(quit);
		gl4duwResizeFunc(resize);

	gl4duwDisplayFunc(draw);
	gl4duwMainLoop();
	return 0;
}
static void quit(void) {
	glDeleteTextures(sizeof _tex / sizeof *_tex, _tex);
	gl4duClean(GL4DU_ALL);
	if(_mmusic) {
		if(Mix_PlayingMusic())
			Mix_HaltMusic();
		Mix_FreeMusic(_mmusic);
		_mmusic = NULL;
	}
	Mix_CloseAudio();
	Mix_Quit();
	if(_screen) {
		gl4dpSetScreen(_screen);
		gl4dpDeleteScreen();
		_screen = 0;
	}
	gl4duClean(GL4DU_ALL);
}
