
#------------------------------------------------------------------------------

SOURCE=$(wildcard *.cpp) $(wildcard ../common/*.cpp)
MYPROGRAM=updateClient
MYINCLUDES=../common

MYLIBRARIES=boost_filesystem-mt -lcurl
CC=g++

#------------------------------------------------------------------------------



all: $(MYPROGRAM)



$(MYPROGRAM): $(SOURCE)

	$(CC) -Wall -g -I$(MYINCLUDES) $(SOURCE) -o$(MYPROGRAM) -l$(MYLIBRARIES)

clean:

	rm -f $(MYPROGRAM)

