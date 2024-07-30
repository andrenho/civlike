CPPFLAGS += -O0 -ggdb -MMD -Wall -Wextra -fPIC -Icivlike $(shell pkg-config --cflags allegro-5 allegro_main-5 allegro_font-5 lua)
CXXFLAGS += --std=c++20
OBJ := civlike/civlike.o civlike/rules.o civlike/luavalue.o civlike/game.o

all: libcivlike.so uiproto cltests

libcivlike.so: ${OBJ}
	$(CXX) $^ -shared -o $@ $(shell pkg-config --libs lua)

cltests: civlike/tests.o libcivlike.so
	$(CXX) $^ -o cltests $(shell pkg-config --libs lua)

uiproto: ui/uiproto.o libcivlike.so
	$(CXX) $^ -o uiproto $(shell pkg-config --libs allegro-5 allegro_main-5 allegro_font-5 lua)

clean:
	rm -f libcivlike.so civlike/*.o ui/*.o uiproto cltests

-include $(OBJ:.o=.d)