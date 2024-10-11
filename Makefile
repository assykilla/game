
all: triassic_pinball 
triassic_pinball: triassic_pinball.cpp group/xreyes.cpp group/abotello.cpp group/abotello.h group/isamara.cpp group/gtorres.cpp
	g++ triassic_pinball.cpp group/xreyes.cpp group/abotello.cpp group/isamara.cpp group/gtorres.cpp group/libggfonts.a -Wall -otriassic_pinball -lX11 -lGL -lGLU -lm
clean:
	rm -f triassic_pinball

