# Set Compilers
CC=g++

# -c 
# -Wall - Will not let compile with any warnings
# -std=c++11 - Enforce c++11 
CFLAGS=-c -Wall -std=c++11

# LIB_PATHS = -L/srv/home/felipegb94/repositories/PCGen/syoyo-tinyobjloader-c7da237/
# LIBS= /srv/home/felipegb94/repositories/PCGen/syoyo-tinyobjloader-c7da237/libtinyobjloader.a
LIBS = -lm


all: pcgen

pcgen: pcgen.o tinyobjloader.o # main.o factorial.o
	$(CC) tiny_obj_loader.o pcgen.o -o pcgen $(LIB_PATHS) $(LIBS)
	# $(CC) main.o factorial.o pcgen.o -o pcgen

tinyobjloader.o: tinyobjloader/tiny_obj_loader.cc
	$(CC) $(CFLAGS) tinyobjloader/tiny_obj_loader.cc

# factorial.o: factorial.cpp
#     $(CC) $(CFLAGS) factorial.cpp

pcgen.o: pcgen.cpp
	$(CC) $(CFLAGS) pcgen.cpp

clean:
	rm *o pcgen