
#include "maths.h"
#include <random>
/*
struct Bird {
	Vec2 position;
	Vec2 speed; 

	void update(int posx, int posy , double spdx , double spdy) {
		position.x = posx ; position.y = posy;
		speed.x = spdx; speed.y = spdy;
	}

	Bird(int posx, int posy, double spdx, double spdy) {  //constructeur pour initialiser un oiseau 
		position.x = posx; position.y = posy;
		speed.x = spdx; speed.y = spdy;
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

	T* pop() {
		T* s = tab[top];
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
		std::cout << "destructeur appel�" << std::endl;
	};

	// creation fonction pour remplir la stack avec n oiseau
	// � la main , al�atoirement, rang� les uns � cot� des autres

};
*/