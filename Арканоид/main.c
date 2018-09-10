#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#define SCREEN_WIDTH 640
#define SCREEN_HIGHT 520

struct object {
	short hp;
	short r;
	short g;
	short b;
	short dead;
	SDL_Rect bnd;
};

void draw_rect (SDL_Renderer *, SDL_Rect *, int, int, int);
void Ball_move (SDL_Rect *, SDL_Rect, struct object *, short *, short *, short *, short *, int);
void fill_rect (SDL_Rect *, int, int, int, int);
void create_object (int, int, short, struct object *);

int main(int argc, char *argv[]) {
	SDL_Window *Mainw = NULL;
	SDL_Renderer *Ren = NULL;
	SDL_Event Windowe;
	struct object *Blocks = NULL;
	SDL_Rect Player, Ball, Back, HUD;
	SDL_Rect *Lifebar;
	const int FPS = 120;
	const int Frame = 1000 / 120;
	Uint32 start;
	short directionx = 1;
	short directiony = 1;
	short enterc;
	short php;
	short shp;
	short level;
	short game;
	short sc;
	short b;
	short c;
	short d;
	short i;
	int n;
	int x;
	int y;
	int dc;
	int et;
	int buttonid;
	int lx;
	const SDL_MessageBoxButtonData buttons[] = {
		{ 0, 0, "Hard" },
		{ 0, 1, "Normal" },
		{ 0, 2, "Easy" },
	};
	const SDL_MessageBoxColorScheme colorScheme = {
		{
			{ 255, 0, 0 },
			{ 0, 255, 0 },
			{ 255, 255, 0 },
			{ 0, 0, 255 },
			{ 255,   0, 255 }
		}
	};
	const SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_INFORMATION,
		NULL,
		"Difficulty menu",
		"Select difficulty",
		3,
		buttons,
		&colorScheme
	};
	SDL_ShowMessageBox(&messageboxdata, &buttonid);
	srand(time(NULL));
	switch (buttonid) {
	case 0:
		et = 4;
		php = 3;
		break;
	case 1:
		et = 3;
		php = 4;
		break;
	case 2:
		et = 2;
		php = 5;
		break;
	}
	shp = php;
	lx = 10;
	n = 28;
	dc = 0;
	x = 50;
	y = 50;
	sc = 1;
	b = 1;
	enterc = 0;
	level = 0;
	game = 1;
	Blocks = (struct object *)malloc(n * sizeof(struct object));
	Lifebar= (SDL_Rect *)malloc(php * sizeof(SDL_Rect));
	Mainw = SDL_CreateWindow("Arcanoid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HIGHT, SDL_WINDOW_SHOWN);
	Ren = SDL_CreateRenderer(Mainw, -1, SDL_RENDERER_ACCELERATED);
	for (c = 0; c < n; c++) {
		create_object(x, y, rand() % et, &Blocks[c]);
		if ((c + 1) % 7 == 0) {
			y += 45;
			x = 50;
		}
		else {
			x += 80;
		}
	}
	for (c = 0; c < php; c++) {
		fill_rect(&Lifebar[c], lx, 490, 10, 20);
		lx += 15;
	}
	fill_rect(&Ball, 350, 430, 10, 10);
	fill_rect(&Player, 320, 440, 70, 10);
	fill_rect(&Back, 0, 0, SCREEN_WIDTH, 480);
	fill_rect(&HUD, 0, 480, SCREEN_WIDTH, 40);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Arcanoid", "Level 1", NULL);
	while (game) {
		while (1) {
			if ((php == 0) 
				|| (dc == n)) {
				break;
			}
			dc = 0;
			start = SDL_GetTicks();
			if (SDL_PollEvent(&Windowe)) {
				if (SDL_QUIT == Windowe.type) {
					game = 0;
					php = -1;
					break;
				}else if (SDL_KEYDOWN == Windowe.type) {
					switch (Windowe.key.keysym.sym) {
					case SDLK_RIGHT:
						if (Player.x < SCREEN_WIDTH - Player.w) {
							Player.x += 10;
							if (enterc == 0) {
								Ball.x += 10;
							}
						}

						break;
					case SDLK_LEFT:
						if (Player.x > 0) {
							Player.x -= 10;
							if (enterc == 0) {
								Ball.x -= 10;
							}
						}

						break;
					case SDLK_SPACE:
						if (enterc == 0) {
							enterc++;
							directionx = 0;
							directiony = 1;
						}
						break;
					}
				}
			}
			draw_rect(Ren, &Back, 240, 255, 240);
			draw_rect(Ren, &Player, 0, 0, 0);
			if ((enterc != 0)) {
				Ball_move(&Ball, Player, Blocks, &directionx, &directiony, &enterc, &php, n);
			}
			for (c = 0; c < n; c++) {
				if (Blocks[c].dead == 0) {
					draw_rect(Ren, &Blocks[c].bnd, Blocks[c].r, Blocks[c].g, Blocks[c].b);
				}else {
					dc++;
				}
			}
			draw_rect(Ren, &Ball, 255, 0, 0);
			draw_rect(Ren, &HUD, 255, 255, 255);
			for (c = 0; c < php; c++) {
				draw_rect(Ren, &Lifebar[c], 255, 0, 0);
			}
			SDL_RenderPresent(Ren);
			if (Frame > SDL_GetTicks() - start) {
				SDL_Delay(Frame - (SDL_GetTicks() - start));
			}
		}
		if (php == 0) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Arcanoid", "You loose!", NULL);
			break;
		}else if (php > 0) {
			level++;
			php++;
			if (php > shp) {
				Lifebar = realloc(Lifebar, php * sizeof(SDL_Rect));
				fill_rect(&Lifebar[php-1], lx, 490, 10, 20);
				lx += 15;
				shp++;
			}
			dc = 0;
			enterc = 0;
			Ball.x = 350;
			Ball.y = 430;
			Player.x = 320;
			Player.y = 440;
			directionx = 0;
			directiony = 0;
			free(Blocks);
			switch (level) {
			case 1:
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Arcanoid", "Level 2", NULL);
				n = 36;
				x = SCREEN_WIDTH / 2 - 25;
				y = 50;
				Blocks = (struct object *)malloc(n * sizeof(struct object));
				for (c = 0; c < n; c++) {
					create_object(x, y, rand() % et, &Blocks[c]);
					if ((c + 1) % b == 0) {
						sc += 2;
						b += sc;
						y += 15;
						x = Blocks[0].bnd.x - Blocks[c].bnd.w*(sc / 2);
					}else {
						x += 50;
					}
				}
				break;
			case 2:
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Arcanoid", "Level 3", NULL);
				n = 39;
				dc = 0;
				Blocks = (struct object *)malloc(n * sizeof(struct object));
				for (i = 0; i < 3; i++) {
					sc = 1;
					d = 2;
					b = 1;
					switch (i) {
					case 0:
						x = 200;
						y = 50;
						break;
					case 1:
						x = 400;
						y = 50;
						break;
					case 2:
						x = 300;
						y = 95;
						break;
					}
					for (c = 0; c < 13; c++) {
						create_object(x, y, rand() % et, &Blocks[c+13*i]);
						if ((c + 1) % b == 0) {
							sc += d;
							b += sc;
							y += 15;
							x = Blocks[0+13* i].bnd.x - Blocks[c].bnd.w*(sc / 2);
							if (b == 9) {
								d = -d;
							}
						}else {
							x += 50;
						}
					}
				}
				break;
			case 3:
				game = 0;
				break;
				}
			}
			if (game == 0 && php > 0) {
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Arcanoid", "You win!", NULL);
				break;
			}
		}
	SDL_DestroyRenderer(Ren);
	SDL_DestroyWindow(Mainw);
	if (game!=0) {
		free(Blocks);
	}
	free(Lifebar);
	SDL_Quit();
	return 0;
}

void create_object (int x, int y, short type, struct object *a) {
	a->dead = 0;
	switch (type) {
	case 0:
		a->hp = 1;
		a->r = 100;
		a->g = 149;
		a->b = 237;
		break;
	case 1:
		a->hp = 2;
		a->r = 255;
		a->g = 215;
		a->b = 0;
		break;
	case 2:
		a->hp = 3;
		a->r = 0;
		a->g = 255;
		a->b = 0;
		break;
	case 3:
		a->hp = 4;
		a->r = 148;
		a->g = 0;
		a->b = 211;
		break;
	}
	fill_rect(&a->bnd, x, y, 50, 15);
}

void fill_rect(SDL_Rect *a, int x, int y, int w, int h) {
	a->h = h;
	a->w = w;
	a->x = x;
	a->y = y;
}

void Ball_move (SDL_Rect *b, SDL_Rect p, struct object *e, short *directionx, short *directiony, short *enterc, short *php, int n) {//physic
	short br;
	short bt;
	short bl;
	short bd;
	short upc;
	short dwc;
	short lfc;
	short rgc;
	short rn;
	short c;
	int right;
	int left;
	int top;
	int down;
	int hit;
	rn = rand()%2;
	bd = b->y + b->w;
	bt = b->y;
	bl = b->x;
	br = b->x + b->h;
	c = 0;
	upc = 0;
	dwc = 0;
	lfc = 0;
	rgc = 0;
	for (c; c < n; c++) {
		if (e[c].dead == 0) {
			right = e[c].bnd.x + e[c].bnd.w;
			left = e[c].bnd.x;
			top = e[c].bnd.y;
			down = e[c].bnd.y + e[c].bnd.h;
			hit = 0;
			if ((((bl >= left) && (bl <= right)) 
				|| ((br >= left) && (br <= right))) 
				&& (bd == top)) {
				if ((bl == right) 
					&& (bd == top)
					&& ((*directionx == *directiony))) {
					continue;
				}else if ((br == left) 
					&& (bd == top) 
					&& (*directionx == -*directiony)) {
					continue;
				}else {
					hit = 1;
					dwc++;
					*directiony = -*directiony;
				}
			}else if ((((bl >= left) && (bl <= right)) 
				|| ((br >= left) && (br <= right))) 
				&& (bt == down)) {
				if (bl == right && bt == down && *directionx == -*directiony) {
					continue;
				}else if (br == left && bt == down && *directionx == *directiony) {
					continue;
				}else {
					hit = 1;
					upc++;
					*directiony = -*directiony;
				}
				if (*directionx == 0) {
					*directionx = 1;
					if (rn == 0) {
						*directionx = -1;
					}
				}
			}else if ((((bt > top) && (bt <= down)) 
				|| ((bd > top) && (bd <= down))) 
				&& (bl == right)) {
				hit = 1;
				lfc++;
			    *directionx = -*directionx;
			}else if ((((bt > top) && (bt <= down)) 
				|| ((bd > top) && (bd <= down))) 
				&& (br == left)) {
				hit = 1;
				rgc++;
				*directionx = -*directionx;
				if (*directionx == 0) {
					*directionx = -1;
				}
			}
			if (hit == 1) {
				e[c].hp--;
				switch (e[c].hp) {
				case 0:
					e[c].dead = 1;
					break;
				case 1:
					e[c].r = 100;
					e[c].g = 149;
					e[c].b = 237;
					break;
				case 2:
					e[c].r = 255;
					e[c].g = 215;
					e[c].b = 0;
					break;
				case 3:
					e[c].r = 0;
					e[c].g = 255;
					e[c].b = 0;
					break;
				}
			}
		}
	}
	if (upc>1 || dwc>1) {
		*directiony = -*directiony;
	}else if (lfc>1 || rgc>1) {
		*directionx = -*directionx;
	}
	if ((p.y == bd)
		&&(p.y+p.h>bd) 
		&& (((bl >= p.x) && (bl <= p.x + p.w)) 
		|| ((br >= p.x) && (br <= p.x + p.w)))) {
		*directiony = -*directiony;
	}else if (b->y <= 0) {
		*directiony = -*directiony;
		if (*directionx == 0) {
			*directionx = 1;
			if (rn == 0) {
				*directionx = -1;
			}
		}
		if (bl <= 0 
			|| br >= SCREEN_WIDTH) {
			*directionx = -*directionx;
		}
	}else if (b->x < 0) {
		*directionx = -*directionx;
	}else if (b->x >SCREEN_WIDTH - b->w) {
		*directionx = -*directionx;
	}else if (b->y >= SCREEN_HIGHT-20) {
		*enterc = 0;
		*php -= 1;
		b->x = p.x + 30;
		b->y = p.y - b->h;
		*directionx = 0;
		*directiony = 0;
	}
	b->x += *directionx;
	b->y += *directiony;
}

void draw_rect (SDL_Renderer *Ren, SDL_Rect *rect, int r, int g, int b) {
	SDL_SetRenderDrawColor(Ren, r, g, b, 255);
	SDL_RenderFillRect(Ren, rect);
}