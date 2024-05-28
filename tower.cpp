// Tower
// 2024 M. Gerloff

#include "tower.hpp"
#include "assets.hpp"

using namespace blit;

int state = 1;
int xx;
int width = 80;
int step;
int score;

struct Fade
{
    int alpha;
    int c1;
    int c2;
    int x;
    int y;
    int width;
};

struct Block
{
    int x1;
    int x2;
};

Block block[21];
Fade fade;

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
        screen.pen = Pen(fade.c1,fade.c2,fade.c1);
	    screen.rectangle(Rect(fade.x, fade.y, fade.width, 5));
        screen.alpha = 255;
    }

	screen.pen = Pen(150,150,150);
    screen.text("score", minimal_font, Point(1, 0));

	screen.pen = Pen(250,250,250);
    screen.text(std::to_string(score), minimal_font, Point(30, 0));
	if (state == 2)
		screen.text("game over", minimal_font, Point(58, 57));
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
			    xx = -xx;

		    if (buttons.pressed & Button::A)
		    {
			    if (block[step].x2 <= block[step - 1].x1 || block[step].x1 >= block[step - 1].x2)
			    {
                    step--;
				    state = 2;
			    }
                else 
                {
                    if (block[step].x1 > block[step - 1].x1)
			        {
				        score += 1;

                        fade.alpha = 255;
                        fade.c1 = step * 10;
                        fade.c2 = step * 5 + 150;
                        fade.x = block[step - 1].x2;
                        fade.y = 115 - (step * 6);
                        fade.width = block[step].x1 - block[step - 1].x1;

				        block[step].x2 = block[step - 1].x2;
			        }
                    else if (block[step].x1 < block[step - 1].x1)
			        {
				        score += 1;

                        fade.alpha = 255;
                        fade.c1 = step * 10;
                        fade.c2 = step * 5 + 150;
                        fade.x = block[step].x1;
                        fade.y = 115 - (step * 6);
                        fade.width = block[step - 1].x1 - block[step].x1;

				        block[step].x1 = block[step - 1].x1;
			        }
                    else
			        {
				        score += 5;

                        fade.alpha = 255;
                        fade.c1 = 255;
                        fade.c2 = 255;
                        fade.x = block[step].x1;
                        fade.y = 115 - (step * 6);
                        fade.width = block[step].x2 - block[step].x1;
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
}
