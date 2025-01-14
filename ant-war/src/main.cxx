

#ifdef _WIN32
#include <SDL.h>

#define not !

#else

#include <SDL2/SDL.h>

#endif

#include "it_s_work.h"
#include <iostream>
#include <random>  
#include <vector>
#include <cmath>



int const WIDTH = 800;
int const HEIGHT = 600;
double const PI = 3.1415927; // TODO: better PI   
int nb_birds = 15;
double deltaT = 0.1;
const float COHESION_FACTOR = 0.01f;
const float SEPARATION_FACTOR = 0.05f;
const float ALIGNMENT_FACTOR = 0.05f;

template<typename T>

struct Vec2 {

	T x;
	T y;

	Vec2() {
		x = 0;
		y = 0;
	}

	Vec2(T xcord, T ycord) {
		x = xcord;
		y = ycord;
	}

	Vec2<T> operator + (const Vec2<T>& other) {
		return Vec2<T>(x + other.x, y + other.y);
	}

	Vec2<T> operator - (const Vec2<T>& other) {
		return Vec2<T>(x - other.x, y - other.y);
	}

	Vec2<T>& operator+=(const Vec2<T>& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2<T>& operator*=(T scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}
	float norme() {
		return std::sqrt(x * x + y * y);
	}

	Vec2<T> normalize(Vec2<T> other) const {
		float norme = norme(other);
		if (norme == 0) return Vec2<T>(0, 0);
		return Vec2<T>(x / norme, y / norme);
	}

};

template<typename N>
struct BirdsStack {
	int size;
	int top;
	N** tab;

	void push(N valeur) {
		tab[top] = &valeur;
		top += 1;
	};

	N* pop() {
		N* s = tab[top];
		tab[top] = 0; // pointeur nul 
		top -= 1;
		return s;
	};

	bool empty() {
		if (top = 0) {
			return true;
		}
		else {
			return false;
		};
	};

	BirdsStack(int n) {
		top = 0;
		size = n;
		tab = new N * [size];
		for (int i = 0; i < size; i = i + 1) {
			tab[i] = 0;
		}
	};


	~BirdsStack() {
		delete tab;
		std::cout << "destructeur appelé" << std::endl;
	};

	// creation fonction pour remplir la stack avec n oiseau
	// à la main , aléatoirement, rangé les uns à coté des autres

};


struct Object {
	virtual void draw(SDL_Renderer* renderer) const = 0;
	virtual ~Object() = default;
	virtual void update() = 0;
};

struct Bird : public Object {
	Vec2<float> position;
	Vec2<float> speed;

	//constructeur pour initialiser un oiseau 
	Bird(int posx, int posy, double spdx, double spdy) {  
		position.x = posx; position.y = posy;
		speed.x = spdx; speed.y = spdy;
	}
	
	Vec2<float> centregravite(std::vector<std::shared_ptr<Bird>>& birdflock) {
		Vec2<float> res;
		for (auto& other : birdflock) {
			res += other->position;
		}
		return res *= (1 / nb_birds);
	}

	Vec2<float> centrevitesse(std::vector<std::shared_ptr<Bird>>& birdflock) {
		Vec2<float> res;
		for (auto& other : birdflock) {
			res += other->speed;
		}
		return res *= (1 / nb_birds);
	}

	void update(std::vector<std::shared_ptr<Bird>>& flock) {
		Vec2<float> cohesion(0, 0);
		Vec2<float> separation(0, 0);
		Vec2<float> alignment(0, 0);

		//règle cohésion 
		Vec2<float> c = centregravite(flock);
		cohesion = c - position;

		//regle de séparation 
		float distancelim = 3;
		int closebird = 0;
		for (const auto& other : flock) {
			if (other.get() == this) continue;
			Vec2<float> diff = other->position - position;
			float distance = diff.norme();


			if (distance < distancelim) {
				closebird += 1;
				separation += diff;
			}

		}
		separation *= (1 / closebird);

		//regle d'alignement
		alignment = centrevitesse(flock);

		//on édite les positions et les vitesses avec les vecteurs calculés 
		alignment *= ALIGNMENT_FACTOR;
		cohesion *= COHESION_FACTOR;
		separation *= SEPARATION_FACTOR;
		speed = (alignment + separation + cohesion);
		position += (speed *= deltaT);

	};

	void draw(SDL_Renderer* renderer) const override {
		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderDrawPoint(renderer, static_cast<int>(position.x), static_cast<int>(position.y));
	}

	
};





struct global_t {
	SDL_Window * window = NULL;
	SDL_Renderer * renderer = NULL;

	// random
	std::random_device rd;
	std::default_random_engine eng;
	std::uniform_real_distribution<float> rand;

};

global_t g;
// TO DO peindre un oiseau 
//TO DO peindre nuée d'oiseau 
//void paint_it_s_work(int ox, int oy, int scale = 20) {
//	SDL_SetRenderDrawColor(g.renderer, 0u, 0u, 0u, SDL_ALPHA_OPAQUE);   //def la couleur des rectangles à peindre 
//	for (int j = 0; j < px::height; ++j) {
//		for (int i = 0; i < px::width; ++i) {   //parcours les rectangles 
//			if (px::header_data[j*px::width+i] == 0) {     // dans GIMP grande matrice avec des O et des 1 pour savoir si colorié
//				SDL_Rect r = { i*scale+ox, j*scale+oy, 20, 20 }; //20 et 20 taille des cotés du rectangle et leur position au début
//				SDL_RenderFillRect(g.renderer, &r); //dessine un rectangle
//			}
//		}
//	}
//}

void paint_bird(std::vector<std::shared_ptr<Bird>>& flock) {
	SDL_SetRenderDrawColor(g.renderer, 0u, 0u, 0u, SDL_ALPHA_OPAQUE);   //def la couleur des rectangles à peindre 
	for (const auto& other : flock) {
				SDL_Rect r = {other->position.x, other->position.y, 5, 5 }; //20 et 20 taille des cotés du rectangle et leur position au début
				SDL_RenderFillRect(g.renderer, &r); //dessine un rectangle
			}
	}


void do_render(std::vector<std::shared_ptr<Bird>>& flock) { //peint une fenêtre blanche avec le clear 
	SDL_SetRenderDrawColor(g.renderer, 255u, 255u, 255u, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(g.renderer);

	//paint_bird(flock); //appel de la fonction pour peindre 

	SDL_RenderPresent(g.renderer);
}
//TO DO modifier les positions oiseaux à peindre et leur direction en fonction des calculs 
//TO DO éléments intéractifs (en dernier) 



//
/*


BirdsStack close(birdsstack,bird) {
	float distancelim = 3; //def la distance à laquelle on considère les oiseaux proches 
	BirdsStack(birdsstack.size) Stackclose;
	for (int i = 0; i < birdsstack.top; i++) {
		double dist = abs(birdsstack[i].position - bird.position);
		if (dist < 0.01) {}
		else {Stackclose.push(birdsstack[i]) }
	};
}
*/
void do_update(std::vector<std::shared_ptr<Bird>> flock) {

	for (auto& bird : flock) {
		bird->update(flock);
	}
	/*
	Vec2 cohesion;
	Vec2 separation;
	Vec2 alignement;

	for (int i = 0; i < birdsstack.top; i++) { //boucle pour faire evoluer tous les oiseaux
		//règle cohésion 
		Vec2 c = centregravite(birdsstack);
		cohesion = c - birdsstack[i].position;

		//regle de séparation 
		BirdsStack Stackclose = close(birdsstack, birdsstack[i]);
		Vec2 S; 
		for (int j = 0; j < Stackclose.top; j++) {
			S += Stackclose[j].position - birdsstack[i].position;
		};
		separation = S / Stackclose.top;

		//regle d'alignement
		alignement = centrevitesse(birdsstack);

		// TO DO pondération 

		//on édite les positions et les vitesses avec les vecteurs calculés 
		birdsstack[i].vitesse = (alignement + separation + cohesion) / 3;
		birdsstack[i].position += deltaT * birdsstack[i].vitesse;

	}
	*/

}

int main(int argc, char ** argv)
{

	std::vector<std::shared_ptr<Bird>> birdflock;

	
	for (int i = 0; i < nb_birds; ++i) {
		float x = static_cast<float>(rand() % WIDTH);
		float y = static_cast<float>(rand() % HEIGHT);
		float vx = static_cast<float>(rand() % 3 - 1);
		float vy = static_cast<float>(rand() % 3 - 1);
		birdflock.push_back(std::make_shared<Bird>(x, y, vx, vy));
	}

	int status;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) { //initialise SDL
		return 1;
	}

	g.window = SDL_CreateWindow("Ant War",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (not g.window) {
		return 1;
	}

	// get the default renderer
	g.renderer = SDL_CreateRenderer(g.window, -1, 0);
	if (not g.renderer) {
		return 1;
	}

	bool end = false;
	while (not end) {  //boucle infinie qui attend un événement en checkant toutes les 20ms 
		SDL_Event event;
		if (SDL_WaitEventTimeout(&event, 20)) {
			switch (event.type) {
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
					case SDL_WINDOWEVENT_CLOSE:  //fermer la fenêtre avec la croix 
						end = true;
						break;
					case SDL_WINDOWEVENT_SIZE_CHANGED:   //on ne change pas la taille pour l'instant c'est galère 
						// Should never happen
						break;
					default:
						break;
				}
				break;
			case SDL_KEYDOWN:           
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					end = true;
				}
				break;
			case SDL_KEYUP: 
				break;
			}
		} else {
			// Got time out or error
			char const * e = SDL_GetError();
			if (e != NULL) {
				if (strlen(e) != 0) {
					// Got error
					return 1;
				}
			}

			do_update(birdflock);
			do_render(birdflock);
		}
	}

	SDL_DestroyRenderer(g.renderer);
	SDL_DestroyWindow(g.window);
	SDL_CloseAudio();
	SDL_Quit();
	return 0;
}

// if (inexpected_situation)
//		throw exception 

