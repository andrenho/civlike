CXX=clang++

CPPFLAGS += -O0 -ggdb -MMD -Wall -Wextra -Icommon -Iruleset -fPIC $(shell sdl2-config --cflags)
CXXFLAGS += --std=c++20 -fmodules-ts
OBJ := common/geometry.o ruleset/ruleset.o # ruleset/gameparam.o

all: libcivlike.so uiproto cltests

%.o: %.cppm
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $^

libcivlike.so: ${OBJ}
	$(CXX) $^ -shared -o $@

uiproto: ui/uiproto.o libcivlike.so
	$(CXX) $^ -o uiproto $(shell sdl2-config --static-libs)

clean:
	rm -f libcivlike.so uiproto ${OBJ} $(OBJ:.o=.d)

-include $(OBJ:.o=.d)