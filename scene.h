#define RED	{ 0.8,   0,   0 }
#define GREEN	{   0, 0.8,   0 }
#define BLUE	{   0,   0, 0.8 }
#define CYAN	{ 0.5, 0.8, 0.8 }
#define GRAY	{ 0.6, 0.6, 0.6 }
#define WHITE	{   1,   1,   1 }
#define YELLOW	{ 0.8, 0.7,   0 }
#define BLACK	{   0,   0,   0 }
#define WHITE2	{   2,   2,   2 }

#define UP	{   0,   1,   0 }

#if PERSPECTIVE2
#define EYE	{-0.9,-0.5, 0.9 }
#define LOOK	{   0,   0,   0 }
#define FOV	110
#else
#define EYE	{   0,   0,  -4 }
#define LOOK	{   0,   0,   6 }
#define FOV	36
#endif

#if 1
sphere scene[] = {
	{ {    0,     0,    0},    0, BLACK,  BLACK   },	// null object - color used when no ray hits
	{ {-1001,     0,    0}, 1000, RED,    BLACK   },	// a 0
	{ { 1001,     0,    0}, 1000, BLUE,   BLACK   },	// b 1
	{ {    0,     0, 1001}, 1000, GRAY,   BLACK   },	// c 2
	{ {    0, -1001,    0}, 1000, GRAY,   BLACK   },	// d 3
	{ {    0,  1001,    0}, 1000, WHITE,  WHITE2  },	// e 4
	{ { -0.6,  -0.7, -0.6},  0.3, YELLOW, BLACK   },	// f 5
	{ {  0.3,  -0.4,  0.3},  0.6, CYAN,   BLACK   },	// g 6
};

#else
// scene from https://www.kevinbeason.com/smallpt/
// will also need new eye/lookat vectors
sphere scene[] = {
	{ { 1e5+1,40.8,81.6 }, 1e5, {.75,.25,.25}, BLACK },
	{ {-1e5+99,40.8,81.6}, 1e5, {.25,.25,.75}, BLACK },
	{ {50,40.8, 1e5     }, 1e5, {.75,.75,.75}, BLACK },
	//{ {50,40.8,-1e5+170 }, 1e5, BLACK        ,{12,12,12} },
	{ {50, 1e5, 81.6    }, 1e5, {.75,.75,.75}, BLACK },
	{ {50,-1e5+81.6,81.6}, 1e5, {.75,.75,.75}, BLACK },
	{ {27,16.5,47       }, 1e5, {0.999,0.999,0.999},BLACK },
	{ {73,16.5,78       }, 1e5, {0.999,0.999,0.999},BLACK },
	{ {50,681.6-.27,81.6}, 600, BLACK, {12,12,12} },
};

/*
 Sphere spheres[] = {//Scene: radius, position, emission, color, material
   Sphere(1e5, Vec( 1e5+1,40.8,81.6), Vec(),Vec(.75,.25,.25),DIFF),//Left
   Sphere(1e5, Vec(-1e5+99,40.8,81.6),Vec(),Vec(.25,.25,.75),DIFF),//Rght
   Sphere(1e5, Vec(50,40.8, 1e5),     Vec(),Vec(.75,.75,.75),DIFF),//Back
   Sphere(1e5, Vec(50,40.8,-1e5+170), Vec(),Vec(),           DIFF),//Frnt
   Sphere(1e5, Vec(50, 1e5, 81.6),    Vec(),Vec(.75,.75,.75),DIFF),//Botm
   Sphere(1e5, Vec(50,-1e5+81.6,81.6),Vec(),Vec(.75,.75,.75),DIFF),//Top
   Sphere(16.5,Vec(27,16.5,47),       Vec(),Vec(1,1,1)*.999, SPEC),//Mirr
   Sphere(16.5,Vec(73,16.5,78),       Vec(),Vec(1,1,1)*.999, REFR),//Glas
   Sphere(600, Vec(50,681.6-.27,81.6),Vec(12,12,12),  Vec(), DIFF) //Lite
 };
*/

#endif
