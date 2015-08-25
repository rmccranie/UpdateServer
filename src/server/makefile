
#------------------------------------------------------------------------------

SOURCE=$(wildcard *.cpp) $(wildcard ../common/*.cpp)
 
MYPROGRAM=updateServer
MYINCLUDES=../common -I.

MYLIBRARIES=boost_filesystem-mt -lpthread
CC=g++

#------------------------------------------------------------------------------



all: $(MYPROGRAM)



$(MYPROGRAM): $(SOURCE)

	$(CC) -I$(MYINCLUDES) $(SOURCE) -o$(MYPROGRAM) -l$(MYLIBRARIES)

clean:

	rm -f $(MYPROGRAM)

