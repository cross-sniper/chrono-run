make:
	g++ src/main.cpp -o main -DDEBUG_BUILD -Os -O3 -I/usr/include/libpng16 -I/usr/include/harfbuzz -I/usr/include/freetype2 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/sysprof-6 -pthread -I/usr/include/AL -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system -L src/imgui -I src/imgui -limgui -lraylib -Wfatal-errors -Wall
	./main
