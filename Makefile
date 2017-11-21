TH=-pthread
COM=g++ -g -c -Wall -Wextra -pedantic
C14=-std=c++14
ADDSFML=-lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
ERROR=2> errors.txt

NAME_D=-o sfml-app
RUN_D=gdb -ex run ./bin/Debug/sfml-app

HDRS=include
SRC=src/
OSRC=obj/Debug/src/

connect:
	mv *.o $(OSRC)
	g++ $(OSRC)*.o -o3 $(NAME_D) $(ADDSFML) $(TH)

rebuild: compile connect
	mv sfml-app bin/Debug/

build: rebuild run

run:
	$(RUN_D)
	
compile: current systems cores managers states adds

current:
	$(COM) $(C14) -I$(HDRS) $(SRC)S_Movement.cpp $(ERROR)

systems:
	$(COM) $(C14) -I$(HDRS) $(SRC)S_Renderer.cpp $(ERROR)	
	$(COM) $(C14) -I$(HDRS) $(SRC)S_State.cpp $(ERROR)
	$(COM) $(C14) -I$(HDRS) $(SRC)S_Control.cpp $(ERROR)
	$(COM) $(C14) -I$(HDRS) $(SRC)S_SheetAnimation.cpp $(ERROR)
	$(COM) $(C14) -I$(HDRS) $(SRC)S_Collision.cpp $(ERROR)

cores:
	$(COM) $(C14) -I$(HDRS) $(SRC)Map.cpp $(ERROR)
	$(COM) $(C14) -I$(HDRS) $(SRC)Game.cpp $(ERROR)
	$(COM) $(C14) -I$(HDRS) $(SRC)Window.cpp $(ERROR)
	$(COM) $(C14) -I$(HDRS) $(SRC)main.cpp $(ERROR)

managers:
	$(COM) $(C14) -I$(HDRS) $(SRC)EntityManager.cpp $(ERROR)
	$(COM) $(C14) -I$(HDRS) $(SRC)SystemManager.cpp $(ERROR)
	$(COM) $(C14) -I$(HDRS) $(SRC)EventManager.cpp $(ERROR)
	$(COM) $(C14) -I$(HDRS) $(SRC)ResourceManager.cpp $(ERROR)
	$(COM) $(C14) -I$(HDRS) $(SRC)StateManager.cpp $(ERROR)

states:
	$(COM) $(C14) -I$(HDRS) $(SRC)State_Loading.cpp $(ERROR)
	$(COM) $(C14) -I$(HDRS) $(SRC)State_Game.cpp $(ERROR)
	$(COM) $(C14) -I$(HDRS) $(SRC)State_MainMenu.cpp $(ERROR)
	$(COM) $(C14) -I$(HDRS) $(SRC)State_Intro.cpp $(ERROR)


adds:
	$(COM) $(C14) -I$(HDRS) $(SRC)FPSrate.cpp $(ERROR)	

NOUSEFULL:
	$ gdb program-cmd
	(gdb) run
	(gdb) backtrace
	gdb -ex run --args ./a.out arg1 arg2 ...