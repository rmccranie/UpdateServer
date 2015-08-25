
#------------------------------------------------------------------------------

SOURCE=$(wildcard *.cpp) $(wildcard ../common/*.cpp)
 
MYPROGRAM=updateServer
MYINCLUDES=../common

MYLIBRARIES=boost_filesystem-mt
CC=g++

#------------------------------------------------------------------------------



all: $(MYPROGRAM)



$(MYPROGRAM): $(SOURCE)

	$(CC) -I$(MYINCLUDES) $(SOURCE) -o$(MYPROGRAM) -l$(MYLIBRARIES)

clean:

	rm -f $(MYPROGRAM)

