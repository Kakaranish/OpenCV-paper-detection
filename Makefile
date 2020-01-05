# Recepta kompilacji dla przykładowych programów. W zależności od tego
# z jakiej biblioteki korzystają trzeba przy wywoływaniu kompilatora
# podawać różne opcje. Najpierw więc specyfikujemy rzeczy wspólne dla
# wszystkich programów (w sposób pozwalający zmodyfikować lub nadpisać
# je z linii komend przy uruchamianiu make)...

ifeq ($(origin CC),default)
CC := gcc
endif
ifeq ($(origin CXX),default)
CXX := g++
endif
CFLAGS := -std=c99 -pedantic -Wall $(CFLAGS)
CXXFLAGS := -std=c++11 -pedantic -Wall $(CXXFLAGS)
LDFLAGS := -Wl,--as-needed $(LDFLAGS)

# ... a potem niektórym z nich uzupełniamy opcje. Znak procentu pełni tu
# rolę identyczną jak gwiazdka we wzorcach nazw plików.

gd% : LDLIBS := -lgd $(LDLIBS)

cairo% : CFLAGS := $(shell pkg-config --cflags cairo) $(CFLAGS)
cairo% : LDLIBS := $(shell pkg-config --libs cairo) $(LDLIBS)

opencv% : CXXFLAGS := $(shell pkg-config --cflags opencv) $(CXXFLAGS)
opencv% : LDLIBS := $(shell pkg-config --libs opencv) $(LDLIBS)

# osg% : CXXFLAGS := $(shell pkg-config --cflags openscenegraph) $(CXXFLAGS)
# osg% : LDLIBS := $(shell pkg-config --libs openscenegraph) $(LDLIBS)

# Pierwszy cel tradycyjnie nazywa się "all" i powoduje skompilowanie
# wszystkich programów w przypadku wywołania make bez żadnego argumentu.

CSOURCES := $(wildcard *.c)
CXXSOURCES := $(wildcard *.cpp)
PROGRAMS := $(CSOURCES:.c=) $(CXXSOURCES:.cpp=)

all : $(PROGRAMS)

# Jeden z programów dołącza dodatkowe pliki obiektowe, zbudowane z plików
# ze shaderami GLSL.

osg05 : $(patsubst %.glsl,%.o,$(wildcard phong_*.glsl))

%.o : %.glsl
	./txt2cstr $^ | $(CC) $(CFLAGS) -c -x c - -o $@

# Na koniec recepta na posprzątanie po sobie.

clean :
	rm -f $(PROGRAMS) *.o *~ wynik*

.PHONY : all clean
