####################################################################
#Directories
SRCD =src/
OBJD =obj/
HDRS =include
OSRC =bin/Debug/

####################################################################
#Libraries
WOA  = WoA
SFML =-lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
C14 = -std=c++14
OMP = -fopenmp

####################################################################
#Output
ER=2> errors.txt
NAME_D=-o  $(OSRC)sfml-app


#####################################################################
#Reading Files
SRC  = $(wildcard $(SRCD)*.cpp)
OBJ  = $(SRC:$(SRCD)%.cpp=$(OBJD)%.o)
OBJL = $(SRC:$(SRCD)%.cpp=%.o)
DEPS := $(OBJ:.o=.d)

#####################################################################
#Compilation Flag
VAL   = valgrind --leak-check=full
DEB   = -Wall -Wextra -pedantic
DGB   = -g -da -O0
LCK   = -MMD -c 
COM   = $(CXX) $(DGB)  $(DEB) $(C14) $(LCK)
BUILD = $(CXX) $(DGB) $(OBJ) $(OPT) $(OMP)
OPT   = -o3

CDB   = $(CXX) $(DGB) $(DEB) $(C14) $(LCK)


#####################################################################
#!/bin/bash
-include $(DEPS)

all: $(WOA)
	@echo Completed : EXIT
	make sfml-app

$(WOA): $(OBJ)
	 $(BUILD) $(NAME_D) $(SFML)

obj/%.o: src/%.cpp
#	rm -f $(OBJD)%.d
	$(COM) $(OMP) -I$(HDRS)  -o  $@   $< $(ER)

sfml-app-val: 
	$(VAL) ./bin/Debug/sfml-app

sfml-app:
	gdb -ex run ./bin/Debug/sfml-app

.PHONY: clean
clean:
	rm -f $(OBJ) $(DEPS) bin/Debug/sfml-app