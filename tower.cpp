// Tower
// 2024 M. Gerloff

#include <list>

#include "tower.hpp"
#include "assets.hpp"

using namespace blit;

int state = 1;
int xx;
int width = 80;
int step;
int score;
int best;
int sound;

struct Fade
{
    int alpha;
    int block;
};

struct Block
{
    int x1;
    int x2;
};

struct PARTICLE
{
	Vec2 pos;
	Vec2 vel;
	short alpha;
};

Block block[21];
Fade fade;

Timer sound_timer;

static std::list<PARTICLE> particles;

void sound_update(Timer &t)
{
    if (sound < 10)
    {
        sound++;
        channels[2].frequency = 5000 + (sound * 80);
        channels[2].trigger_attack();
    }
    else
    {
        sound = 0;
        sound_timer.stop();
    }
}

void NewParticle(float x, float y, int xx)
{
	for(short n=1; n<4; n++) 
	{
	    PARTICLE p;
        float dx = (1 + rand() %4) * .1f * xx;
        float dy = (-1 + rand() %6) * .1f;  
        p.vel = Vec2(dx, dy);
        p.pos = Vec2(x, y + n);
        p.alpha = 150 + rand() %100;
        particles.push_back(p);
    }
}

void UpdateParticle()
{
    for(auto p = particles.begin(); p != particles.end();) 
	{
        if(p->alpha < 4) 
		{
            p = particles.erase(p);
            continue;
        }
        p->vel += Vec2(0, .01f);
        p->pos += p->vel;
        p->alpha -= 4;
        ++p;
    }
}

void start()
{
	block[0].x1 = 80 - (width / 2);
	block[0].x2 = block[0].x1 + width;
	block[1].x1 = 0;
	block[1].x2 = width;
	xx = 1;
	step = 1;
}

// init
void init() 
{
    set_screen_mode(ScreenMode::lores);
	start();

    if (read_save(best) == false)
        best = 0;

    channels[0].waveforms = Waveform::SQUARE; // Rand 
	channels[0].volume = 1500;
    channels[0].frequency = 2500;
    channels[0].attack_ms = 5;
    channels[0].decay_ms = 10;
    channels[0].sustain = 5;
    channels[0].release_ms = 5;

    channels[1].waveforms = Waveform::SQUARE; // Ablage
    channels[1].volume = 1500;
    channels[1].frequency = 400;
    channels[1].attack_ms = 10;
    channels[1].decay_ms = 50;
    channels[1].sustain = 100;
    channels[1].release_ms = 10;

    channels[2].waveforms = Waveform::NOISE;
    channels[2].volume = 3000;
    channels[2].frequency = 8000;
    channels[2].attack_ms = 5;
    channels[2].decay_ms = 200;
    channels[2].sustain = 100;
    channels[2].release_ms = 100;

    sound_timer.init(sound_update, 3, -1);
    sound_timer.stop();
}

// render
void render(uint32_t time) 
{
    screen.pen = Pen(0, 0, 0);
    screen.clear();

    screen.alpha = 255;
    screen.mask = nullptr;

	for (int i = 0; i <= step; i++)
	{
	    screen.pen = Pen((i * 10), 150 + (i * 5), (i * 10));
	    screen.rectangle(Rect(block[i].x1, 115 - (i * 6), block[i].x2 - block[i].x1, 5));
	}

    if (fade.alpha > 0)
    {
        screen.alpha = fade.alpha;
        screen.pen = Pen(255,255,255);
	    screen.rectangle(Rect(block[fade.block].x1 - 1, 114 - (fade.block * 6), block[fade.block].x2 - block[fade.block].x1 + 2, 7));
        screen.alpha = 255;
    }

    screen.pen = Pen(150,150,150);
    for(auto &p : particles)
	{
		screen.alpha = p.alpha;
        screen.rectangle(Rect(p.pos.x, p.pos.y,2,2));
	}
	screen.alpha = 255;

	screen.pen = Pen(150,150,150);
    screen.text("score", minimal_font, Point(1, 0), true, TextAlign::top_left);
    screen.text("best", minimal_font, Point(160, 0), true, TextAlign::top_right);

	screen.pen = Pen(250,250,250);
    screen.text(std::to_string(score), minimal_font, Point(1, 6), true, TextAlign::top_left);
    screen.text(std::to_string(best), minimal_font, Point(160, 6), true, TextAlign::top_right);
	if (state == 2)
    {
		screen.text("game over", minimal_font, Point(80, 60), true, TextAlign::center_center);
        if (score == best)
		    screen.text("- new highscore -", minimal_font, Point(80, 70), true, TextAlign::center_center);
    }
}

// update
void update(uint32_t time) 
{
	if (state == 1)
	{
        fade.alpha -= 5;

        if (step < 20)
        {
		    block[step].x1 += xx;
		    block[step].x2 += xx;

		    if (block[step].x1 <= 0 || block[step].x2 >= 159)
            {
                channels[0].trigger_attack();

                xx = -xx;
            }

		    if (buttons.pressed & Button::A)
		    {
			    if (block[step].x2 <= block[step - 1].x1 || block[step].x1 >= block[step - 1].x2)
			    {
                    for (int i=block[step].x1; i<block[step].x2; i++)
                    {
                            NewParticle(i, 115 - (step * 6), xx);
                    }

				    if (score > best)
                    {
                        best = score;
					    write_save(best);
                    }

                    step--;
				    state = 2;
			    }
                else 
                {
                    channels[1].trigger_attack();


                    if (block[step].x1 > block[step - 1].x1)
			        {
				        score += 1;

                        for (int i=block[step - 1].x2; i<block[step].x2; i++)
                        {
                            NewParticle(i, 115 - (step * 6), 1);
                        }

				        block[step].x2 = block[step - 1].x2;
			        }
                    else if (block[step].x1 < block[step - 1].x1)
			        {
				        score += 1;

                        for (int i=block[step].x1; i<block[step - 1].x1; i++)
                        {
                            NewParticle(i, 115 - (step * 6), -1);
                        }

				        block[step].x1 = block[step - 1].x1;
			        }
                    else
			        {
                        sound_timer.start();

				        score += 5;

                        NewParticle(block[step].x1, 115 - (step * 6), -1);
                        NewParticle(block[step].x2, 115 - (step * 6), 1);

                        fade.alpha = 255;
                        fade.block = step;
			        }

			        step++;
			        block[step].x1 = block[step - 1].x1;
			        block[step].x2 = block[step - 1].x2;
                }
		    }
        }
        else if (fade.alpha < 0)
        {
		    score += (block[step].x2 - block[step].x1);
            width -= 8;
		    start();
	    }
	}
	else if (state == 2 && buttons.pressed & Button::A)
	{
		score = 0;
        width = 80;
    	start();
		state = 1;
	}

    UpdateParticle();
}
