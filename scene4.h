/*

Scene optimized for fbdev:
 - no textures
 - focus on front balls

*/

#define RED	{ 0.5,   0,   0 }
#define BLUE	{   0,   0, 0.6 }
#define CYAN	{ 0.0, 0.8, 0.8 }
#define GRAY	{ 0.5, 0.5, 0.5 }
#define WHITE	{   1,   1,   1 }
#define BLACK	{   0,   0,   0 }
#define WHITE2	{   2,   2,   2 }
#define WHITE10	{  6,  6,  6 }

#define GREEN	{   0, 0.5,   0 }
#define PINK	{ 0.8,   0, 0.8 }
#define YELLOW	{   0, 0.8, 0.8 }
//#define YELLOW	{ 0.8, 0.8,   0 }

//#define FOCAL_LENGTH  3.8
#define FOCAL_LENGTH  3.4

#define UP	{   0,  1,   0 }

#define EYE	{   0,   0,  -4 }
#define LOOK	{   0,   0,   6 }
#define FOV	36

CONSTV __attribute__((aligned(1))) sphere scene[] = {
	{ {    0,       0,    0}, BLACK,  BLACK,           0, 0 },	// null object - color used when no ray hits
	{ {    0, -1000.8,    0}, GRAY,   BLACK,   1000*1000, 0 },	// d 3
	{ {    0,    1001,    0}, BLACK,  BLACK,   1000*1000, 0 },	// e 4
	{ {    0,    4.95,    0}, BLACK,  WHITE10,       4*4, 0 },	// e 4
	{ { -0.5,    -0.5, -0.6}, YELLOW, BLACK,     0.3*0.3, 2 },	// f 5
	{ {  0.6,    -0.6, -0.6}, GREEN,  BLACK,     0.2*0.2, 2 },	// f 5
	{ {  0.3,    -0.2,  0.3}, PINK,   BLACK,     0.6*0.6, 1 },	// g 6
};


