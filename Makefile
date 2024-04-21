CFLAGS := -Wall -Wpedantic -ggdb
CXXFLAGS := -Wall -Wpedantic -ggdb -std=c++20

CXXLIBS := $$(pkg-config --cflags --libs tk)

SOURCE.d := source/
OBJECT.d := object/

SOURCE := main.cpp tk_display.c solver.cpp
OBJECT := $(addsuffix .o,$(basename ${SOURCE}))
SOURCE := $(addprefix ${SOURCE.d}, ${SOURCE})
OBJECT := $(addprefix ${OBJECT.d}, ${OBJECT})

object/%.o: source/%.c
	${COMPILE.c} ${CFLAGS} $+ -o $@

object/%.o: source/%.cpp
	${COMPILE.cpp} ${CXXFLAGS} $+ -o $@

main: ${OBJECT}
	${LINK.cpp} ${CXXFLAGS} $+ ${CXXLIBS}

clean:
	-rm ${OBJECT.d}/*.o
