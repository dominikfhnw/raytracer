#define RED	{ 0.8, 0,  0 }
#define GREEN	{ 0, 0.8,  0 }
#define BLUE	{ 0, 0, 0.8 }
#define CYAN	{ 0.5, 0.8, 0.8 }
#define GRAY	{ 0.6, 0.6, 0.6 }
#define WHITE	{ 1, 1,  1 }
#define YELLOW	{ 0.8, 0.8,  0 }

#define EYE	{ 0, 0, -4 }
#define LOOK	{ 0, 0,  6 }
#define UP	{ 0, 1, 0 }
#define FOV	36

sphere scene[] = {
	{ {-1001,     0,    0}, 1000, RED    },	// a 0
	{ { 1001,     0,    0}, 1000, BLUE   },	// b 1
	{ {    0,     0, 1001}, 1000, GRAY   },	// c 2
	{ {    0, -1001,    0}, 1000, GRAY   },	// d 3
	{ {    0,  1001,    0}, 1000, WHITE  },	// e 4
	{ { -0.6,  -0.7, -0.6},  0.3, YELLOW },	// f 5
	{ {  0.3,  -0.4,  0.3},  0.6, CYAN   },	// g 6
};
