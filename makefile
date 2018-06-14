EXE=a
SOURCE=a.cpp

all: $(SOURCE) $(EXE)

$(EXE): $(SOURCE)
	g++ -Wall -g -o $(EXE) $(SOURCE) `root-config --cflags --evelibs`


