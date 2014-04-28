CXX = g++
# Update these paths to match your installation
# You may also need to update the linker option rpath, which sets where to look for
# the SDL2 libraries at runtime to match your install
SDL_LIB = -L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib
SDL_INCLUDE = -I/usr/local/include
# You may need to change -std=c++11 to -std=c++0x if your compiler is a bit older
CXXFLAGS = -Wall -c -g -std=c++0x $(SDL_INCLUDE)
LDFLAGS = $(SDL_LIB)
EXE = visual

OBJ =  error.o main.o rect.o

all: $(EXE)

$(EXE): $(OBJ) 
	$(CXX) $(OBJ) $(LDFLAGS) -o $@

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXE)

