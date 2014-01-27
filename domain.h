#define POS(r,c) ((r)*cols+(c))
#define ROW(p) ((p)/cols)
#define COL(p) ((p)%cols)

typedef struct tState_ {
	  int cols,rows,goals;
	  char **array;
} tState;
