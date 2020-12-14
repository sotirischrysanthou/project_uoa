CXX=       	g++
CXXFLAGS= 	-g -gdwarf-2 -std=gnu++11 -Wall -Iinclude -fPIC
LDFLAGS=	-Llib
AR=		ar
ARFLAGS=	rcs

LIB_HEADERS=	$(wildcard ./include/*.h)
LIB_SOURCE=	$(wildcard ./src/*.cpp)
LIB_OBJECTS=	$(LIB_SOURCE:.cpp=.o)
LIB_STATIC=	lib/libproject.a

SHELL_SOURCE=	$(wildcard ./shell/main.cpp)
TEST_SOURCE=	$(wildcard ./shell/test.cpp)
SHELL_OBJECTS=	$(SHELL_SOURCE:.cpp=.o)
TEST_OBJECTS=	$(TEST_SOURCE:.cpp=.o)
SHELL_PROGRAM=	./exe
TEST_PROGRAM=	./test

all:    $(LIB_STATIC) $(SHELL_PROGRAM) $(TEST_PROGRAM)

%.o:	%.cpp $(LIB_HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $< -lm

$(LIB_STATIC):		$(LIB_OBJECTS) $(LIB_HEADERS)
	mkdir -p lib
	$(AR) $(ARFLAGS) $@ $(LIB_OBJECTS)

$(SHELL_PROGRAM):	$(SHELL_OBJECTS) $(LIB_STATIC)
	$(CXX) $(LDFLAGS) -g -o $@ $(SHELL_OBJECTS) -lproject

$(TEST_PROGRAM):	$(TEST_OBJECTS) $(LIB_STATIC)
	$(CXX) $(LDFLAGS) -g -o $@ $(TEST_OBJECTS) -lproject

clean:
	rm -f $(LIB_OBJECTS) $(LIB_STATIC) $(SHELL_OBJECTS) $(SHELL_PROGRAM)

.PHONY: all clean