BASENAME=weightedPrice
BASENAME2=DataGenerator


SRC=$(BASENAME).cpp 
SRC2=$(BASENAME2).cpp 

OBJ=$(BASENAME).o
OBJ2=$(BASENAME2).o

CC=g++

EXE=$(BASENAME)
EXE2=$(BASENAME2)

all: $(OBJ) $(OBJ2) 
		$(CC) $(OBJ) -o $(EXE)
		$(CC) $(OBJ2) -o $(EXE2)

$(OBJ): $(SRC)
		$(CC) -g -std=c++11 -c $(SRC) -o $(OBJ)

$(OBJ2): $(SRC2)
		$(CC) -g -std=c++11 -c $(SRC2) -o $(OBJ2)


clean: 
		rm -f $(OBJ) $(EXE) 
		rm -f $(OBJ2) $(EXE2) 
		




