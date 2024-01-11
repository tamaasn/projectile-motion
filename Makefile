compile:
	gcc main.c -o projectile_motion `sdl2-config --cflags --libs` -lSDL2_ttf -lm -lSDL2_image
run:
	./projectile_motion
