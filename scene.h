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

sphere scene[] = {
	//{ {-1001, -1000,-1000},    1, BLACK,  BLACK   },	// -1
	{ {-1001,     0,    0}, 1000, RED,    BLACK   },	// a 0
	{ { 1001,     0,    0}, 1000, BLUE,   BLACK   },	// b 1
	{ {    0,     0, 1001}, 1000, GRAY,   BLACK   },	// c 2
	{ {    0, -1001,    0}, 1000, GRAY,   BLACK   },	// d 3
	{ {    0,  1001,    0}, 1000, WHITE,  WHITE2  },	// e 4
	{ { -0.6,  -0.7, -0.6},  0.3, YELLOW, BLACK   },	// f 5
	{ {  0.3,  -0.4,  0.3},  0.6, CYAN,   BLACK   },	// g 6
};
