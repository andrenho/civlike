CPPFLAGS += -O0 -ggdb -MMD -Wall -Wextra -fPIC $(shell pkg-config --cflags lua)
CXXFLAGS += --std=c++20
OBJ := civlike/civlike.o civlike/rules.o civlike/luavalue.o civlike/game.o

all: libcivlike.so

libcivlike.so: ${OBJ}
	$(CXX) $^ -shared -o $@ $(shell pkg-config --libs lua)

cltests: civlike/tests.o libcivlike.so
	$(CXX) $^ -o cltests $(shell pkg-config --libs lua)

clean:
	rm libcivlike.so civlike/*.o

-include $(OBJ:.o=.d)