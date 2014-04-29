#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <SDL2/SDL.h>

#define NCOEF	(32)
#define PI	(3.1415926535898)

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	500
#define SCREEN_DEPTH	32

typedef struct
{
	double fs;	/* sampling freq */
	double Ts;	/* sampling period */
	double phi;	/* global phase (for fondamental oscillator) */
	
	double f;	/* fundamental freq */
	double coefs[NCOEF];	/* current oscillator coeficients */
	
	SDL_AudioSpec as,as2;	/* SDL audio stuff */
	
} Synth;

Synth* make_synth(void)
{	
	int i;
	
	Synth *tmp = malloc(sizeof(Synth));
	if(tmp == NULL)
	{
		printf("Couldn't allocate memory for Synth, exiting.\n");
		exit(EXIT_FAILURE);
	}
	
	tmp->fs = 44100;
	tmp->Ts = 1 / tmp->fs;
	
	tmp->phi = 0;
	
	tmp->f = 220;
	for(i = 0 ; i < NCOEF ; i++)
	{
		tmp->coefs[i] = 1.0/(i+1.0)/(i+1.0);
	}
	
	return tmp;
}

double make_sample(Synth *s)
{
	int i;
	double samp = 0.0;
	
	/* Compute value */
	for (i = 0; i < NCOEF; i++)
	{
		samp += s->coefs[i] * sin( (i+1) * s->phi);	/* i starts at 0, harmonics at 1 */
	}
	
	/* advance phase */
	s->phi += 2 * PI * s->f * s->Ts;
	
	return samp;
}

static void make_chunk(void *userdata, Uint8 *stream, int len)
{
	int i;
	Sint16 *buf;
	int nsample = len/sizeof(*buf);
	
	buf = (Sint16 *) stream;
	
	for (i = 0; i < nsample ; i++)
	{
		buf[i] = (Sint16) floor(make_sample((Synth *)userdata) * 2048);
	}
}

int open_audio(Synth *s)
{
	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
		return -2;

	s->as.freq = s->fs;
	s->as.format = AUDIO_S16SYS;
	s->as.channels = 1;
	s->as.samples = 1024;
	s->as.callback = make_chunk;
	s->as.userdata = (void *) s;
	if(SDL_OpenAudio(&(s->as), &(s->as2)) < 0)
		return -3;
	
	printf("freq:%d\nsamples:%d\nformat:%d:%d\n", s->as2.freq, s->as2.samples,s->as.format,s->as2.format);
	printf("silence:%d\nsamples:%d\nsize:%d\n",s->as2.silence,s->as2.samples,s->as2.size);
	
	return 0;
}

void treat_click(Uint16 x, Uint16 y, Synth *s)
{
	int harmo = x / (SCREEN_WIDTH/NCOEF);
	double value = 1.0 - ((double) y / SCREEN_HEIGHT);
	
	/* change the coeff */
	SDL_LockAudio();
	
	s->coefs[harmo] = value;
	
	SDL_UnlockAudio();
}

int main(void)
{
	Synth *synth;
	
	SDL_Surface *screen;
	SDL_Event event;
	
	int i;
	int quit;
	
	/* SDL initialization */
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("SDL window", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == nullptr){
			return 2;
		}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (renderer == nullptr){
			return 3;
		}
	/* Synth init */
	
	synth = make_synth();
	
	printf("Synth: %f Hz %f s\n", synth->fs, synth->Ts);
	
	open_audio(synth);
	SDL_PauseAudio(0);
	
	/* Main loop */
	
	quit = 0;
	while(!quit)
	{
		/* Events */
		while ( SDL_PollEvent(&event) )
		{
			if (event.type == SDL_QUIT)
			{
				printf("Exiting...\n");
				quit = 1;
			}
			else if(event.type == SDL_MOUSEBUTTONDOWN)
			{
				treat_click(event.button.x, event.button.y, synth);
			}
			else if(event.type == SDL_MOUSEMOTION && event.motion.state != 0)
			{
				treat_click(event.motion.x, event.motion.y, synth);
			}
		}
		
		/* Drawing */
		SDL_Rect bg;
		bg.w = SCREEN_WIDTH;
    	bg.h = SCREEN_HEIGHT;
    	bg.x = 0;
    	bg.y = 0;
		SDL_SetRenderDrawColor(renderer, 0,0,0, 0xFF);
        SDL_RenderFillRect(renderer, &bg);
		
		for (i = 0; i < NCOEF ; i++)
		{
			SDL_Rect rect;
			
			rect.x = i * SCREEN_WIDTH / NCOEF;
			rect.w = SCREEN_WIDTH / NCOEF;
			
			rect.y = (1.0 - synth->coefs[i]) * SCREEN_HEIGHT;
			rect.h = synth->coefs[i] * SCREEN_HEIGHT;	
			SDL_SetRenderDrawColor(renderer, 255,0,0, 0xFF);
            SDL_RenderFillRect(renderer, &rect);
		}
		

	SDL_RenderPresent(renderer);
	}


	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	free(synth);
	return 0;
}
