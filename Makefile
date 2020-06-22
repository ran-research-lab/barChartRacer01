all:
	g++ 16_true_type_fonts.cpp `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image
