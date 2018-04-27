
.PHONY: all
.SUFFIXES: .cpp .C .h .o

DATE := $(shell date +%F)
UNAME := $(shell uname)

ROOTDIR := .
SRCDIR = src
INCLUDE = include
OBJDIR := obj
LOCAL_LIBDIR = ./lib

HT := /usr
HDSO := /usr/lib64

VPATH = src:include

AKA_LIB = $(LOCAL_LIBDIR)/libChortle.a

OFILES = \
	utils.o \
	math_functions.o \
	OGLRenderer.o \

OBJS = $(patsubst %, $(OBJDIR)/%, $(OFILES))

LIBRARY_DIRS := \
	-L$(LOCAL_LIBDIR) \
	-L/usr/lib \

LIBRARIES := \
	-lChortle \

INCLUDES := \
	-I. \
	-I./include \

INC_PYTHON := \
	-I/usr/include/python2.7 -I/usr/lib/python2.7/config \

GL_INCLUDES := \
	-I. \

CXX = g++
CFLAGS = -Wall -g -O2 -std=gnu++11 -fPIC
COMPILE_FLAGS = -Wall -g -O2 -std=gnu++11 -fPIC
OFLAGS = -c $(INCLUDES)
GLFLAGS = -lGL -lGLEW

SWIGLD = $(CXX) $(CFLAGS) -shared
SWIGEXEC = swig

all: $(OBJDIR) $(OBJS)
	ar rv $(AKA_LIB) $(OBJS)

$(OBJDIR):
	@if [ ! -d $(OBJDIR) ]; then \
		echo "-----------------------------"; \
		echo "ERROR: Object directory does not exist"; \
		echo "Creating directory: $(OBJDIR)"; \
		echo "-----------------------------"; \
		mkdir $(OBJDIR); \
	fi

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCLUDE)/%.h
	$(CXX) $< $(CFLAGS) $(OFLAGS) -o $@

genswig: swig/chortle.i
	$(SWIGEXEC) -c++ -python -shadow -I./include swig/chortle.i
	$(CXX) $(CFLAGS) -c swig/chortle_wrap.cxx $(INCLUDES) $(INC_PYTHON) -o swig/chortle_wrap.o
	$(SWIGLD) swig/chortle_wrap.o -o swig/_chortle.so $(LIBRARY_DIRS) $(LIBRARIES) $(GLFLAGS)

clean:
	-rm $(OBJDIR)/*.o $(LOCAL_LIBDIR)/*.a

