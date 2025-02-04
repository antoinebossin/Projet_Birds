
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

	Vec2<T>& operator*=(T scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}

	// TO DO fonction abs qui fait la valeur absolue d'un vecteur 
	float norme(Vec2<T> other) {
		return sqrt(((other.x)**2) + ((other.y) * *2));
	}

	Vec2<T> normalize(Vec2<T> other) const {
		float norme = norme(other);
		if (norme == 0) return Vec2<T>(0, 0);
		return Vec2<T>(x / norme, y / norme);
	}

};