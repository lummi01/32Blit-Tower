// Tower
// 2024 M. Gerloff

#include "tower.hpp"
#include "assets.hpp"

using namespace blit;

Font font(font3x5);

int state = 1;
int xx;
int step;
int score;

struct Block
{
	int x;
	int width;
};

Block block[20];

void start()
{
	block[0].x = 40;
	block[0].width = 80;
	block[1].x = 0;
	block[1].width = 80;
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
	    screen.pen = Pen(10 + (i * 8), 150 + (i * 5), 10 + (i * 8));
	    screen.rectangle(Rect(block[i].x, 115 - (i * 6), block[i].width, 5));
	}

	screen.pen = Pen(225,225,225);
    screen.text("Score " + std::to_string(score), font, Point(1, 1));
	if (state == 2)
		screen.text("Game Over", font, Point(61, 57));
}

// update
void update(uint32_t time) 
{
	if (state == 1)
	{
		block[step].x += xx;
		if (block[step].x <= 0 || block[step].x + block[step - 1].width >= 159)
			xx = -xx;

		if (buttons.pressed & Button::A)
		{
			block[step].width -= abs(block[step - 1].x - block[step].x);

			if (block[step].width == block[step - 1].width)
			{
				score += 5;
			}
			else if (block[step].width > 0)
			{
				score += 1;
				if (block[step].x < block[step - 1].x)
					block[step].x = block[step - 1].x;
			}
			else
			{
				state = 2;
			}

			step++;
			if (step > 19)
			{
				score += block[step - 1].width;
				start();
			}
			else
			{
				block[step].x = block[step - 1].x;
				block[step].width = block[step - 1].width;
			}
		}
	}
	else if (state == 2 && buttons.pressed & Button::A)
	{
			score = 0;
			start();
			state = 1;
	}
}