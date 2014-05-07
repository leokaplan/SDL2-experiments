#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define PI	(3.1415926535898)
#define STEP (1.059463)
#define ROOT (440)

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
	
	Synth *tmp = (Synth*)malloc(sizeof(Synth));
	if(tmp == NULL)
	{
		printf("Couldn't allocate memory for Synth, exiting.\n");
		exit(EXIT_FAILURE);
	}
	
	tmp->fs = 44100;
	tmp->Ts = 1 / tmp->fs;
	
	tmp->phi = 0;
	
	tmp->f = 0;
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
void change_note(Synth* synth, int n){
	synth->f *= pow(STEP,n);
}	
void reset_note(Synth* synth){
	synth->f = ROOT;
}	

