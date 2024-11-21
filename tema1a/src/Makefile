PROJECT=tema1
SOURCES=utils/helpers.cpp utils/MapWorker/map_worker.cpp \
utils/SharedResources/resources.cpp utils/MapReduceIntermediary/map_containers.cpp \
utils/ReduceWorker/reduce_worker.cpp
LIBPATHS=pthread
INCPATHS=utils/
CFLAGS=-c -Wall -Werror -Wno-error=unused-variable
DEBUGFLAGS=-g
CC=g++

# ------------ MAGIC BEGINS HERE -------------

# Automatic generation of some important lists
OBJECTS=$(SOURCES:.cpp=.o)
INCFLAGS=$(foreach TMP,$(INCPATHS),-I$(TMP))
LIBFLAGS=$(foreach TMP,$(LIBPATHS),-L$(TMP))

# Set up the output file names for the different output types
#BINARY=$(PROJECT)
build: all
all: $(SOURCES) tema1

#$(BINARY): $(OBJECTS)
tema1: $(OBJECTS)
	$(CC) $(DEBUGFLAGS) $(LIBFLAGS) $(INCFLAGS) main.cpp $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(DEBUGFLAGS) $(LIBFLAGS) $(INCFLAGS) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) tema1