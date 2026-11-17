# Challenge 9: Infiltración (2). El cerebro de los guardias. Desafío.
#
#  make                 Compila el juego (para depuración, build/debug/)
#  make release         Compila el juego optimizado (build/release/)
#  make run             Lanza el juego (otra semilla aleatoria: SEED=1234; si no, 2026)
#  make run CON=ej01    Lanza el juego con tu versión de un ejercicio. Varios:
#                       make run CON="ej01 ej03"
#  make trace           Lanza el juego, sin ventana, y muestra la huella tras TRACE ticks
#  make trace-equipo    La misma traza con vuestro código: todos los ejercicios que usa
#                       el juego. Si sale la misma huella, el juego funciona con él
#  make ej01 ... ej10   Un ejercicio: sus pruebas P1 y P2
#  make dist            Crea un paquete distribuible del juego (en dist/)
#
# Cada .cpp de src/ se compila a su propio .o (build/<config>/obj/, con las carpetas
# de src/) y después se enlaza todo: los .o, la librería lib/libarena.a (el resto del
# motor, ya compilado; sus .hpp están en src/) y Raylib. Si enlazas un .cpp con una
# función que también está en la librería, se usa la tuya: así funcionan CON y las
# pruebas de cada ejercicio.
#
CXX      ?= g++
STD      := -std=c++26
WARN     := -Wall -Wextra -pedantic -Wconversion
CONFIG   ?= debug
ifeq ($(CONFIG),release)
CXXFLAGS ?= -O2 -DNDEBUG
LDFLAGS  ?= -static-libstdc++ -static-libgcc    # runs where there is no g++ 16
else
CXXFLAGS ?= -O0 -g
endif
RAYINC   := $(shell pkg-config --cflags raylib 2>/dev/null)
RAYLIB   := $(shell pkg-config --libs raylib 2>/dev/null || echo -lraylib)
SEED     ?= 2026
TRACE    ?= 1800
VERSION  ?= 1.0
CON      ?=

OUT  := build/$(CONFIG)
OBJ  := $(OUT)/obj
BIN  := $(OUT)/bin
LIB  := $(wildcard lib/libarena.a)
GAME := $(filter-out src/main.cpp src/ejercicios/%,$(wildcard src/*.cpp src/*/*.cpp))
OBJS := $(GAME:src/%.cpp=$(OBJ)/%.o)
HDRS := $(wildcard src/*/*.hpp src/*/*/*.hpp)

# The exercises that the game uses: the code of the team (make trace-equipo)
TEAM := ej01 ej02 ej03 ej04 ej05 ej08 ej10

.PHONY: all release run trace trace-equipo dist clean prueba juego

all: $(BIN)/arena

release:
	$(MAKE) CONFIG=release

# One translation unit -> one object file
$(OBJ)/%.o: src/%.cpp $(HDRS)
	@mkdir -p $(dir $@)
	$(CXX) $(STD) $(WARN) $(CXXFLAGS) -Isrc $(RAYINC) -c $< -o $@

# The game. The rpath lets it find a libraylib.so placed next to it, in lib/ (dist)
$(BIN)/arena: $(OBJ)/main.o $(OBJS) $(LIB)
	@mkdir -p $(BIN)
	$(CXX) $(LDFLAGS) $^ -o $@ $(RAYLIB) -Wl,-rpath,'$$ORIGIN/lib'

# The game with the exercises of CON: linked again every time, in its own folder
CON_BIN := $(OUT)/con/arena
.PHONY: $(CON_BIN)
$(CON_BIN): $(OBJ)/main.o $(OBJS) $(CON:%=$(OBJ)/ejercicios/%.o) $(LIB)
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) $^ -o $@ $(RAYLIB) -Wl,-rpath,'$$ORIGIN/lib'

ifeq ($(strip $(CON)),)
run: $(BIN)/arena
	./$(BIN)/arena $(SEED)
trace: $(BIN)/arena
	./$(BIN)/arena --trace $(TRACE) $(SEED)
else
run: $(CON_BIN)
	./$(CON_BIN) $(SEED)
trace: $(CON_BIN)
	./$(CON_BIN) --trace $(TRACE) $(SEED)
endif

trace-equipo: $(BIN)/arena
	@$(MAKE) -s $(CON_BIN) CON="$(TEAM)"
	@a=$$(./$(BIN)/arena --trace $(TRACE) $(SEED) | tail -1); \
	 b=$$(./$(CON_BIN) --trace $(TRACE) $(SEED) | tail -1); \
	 echo "El motor:       $$a"; echo "Vuestro código: $$b"; \
	 if [ "$$a" = "$$b" ]; then \
	    echo "✅ La misma partida: el juego funciona con vuestro código"; \
	 else \
	    echo "❌ Otra partida: algún ejercicio de los que usa el juego hace otra cosa"; \
	 fi

# The package: the optimised game, the Raylib it uses and the licences
PKG := dist/infiltracion-$(VERSION)-linux-x86_64
dist:
	$(MAKE) CONFIG=release
	rm -rf $(PKG) && mkdir -p $(PKG)/lib
	cp build/release/bin/arena README.md $(PKG)/
	cp -L $$(ldd build/release/bin/arena | awk '/libraylib/ { print $$3 }') $(PKG)/lib/
	-cp /usr/share/licenses/raylib/LICENSE $(PKG)/LICENSE-raylib
	tar -czf $(PKG).tar.gz -C dist $(notdir $(PKG))
	@echo "$(PKG).tar.gz: unpack it anywhere and run ./arena"

# The tests of one exercise: its program (pruebas/pNN.cpp), its file (ejNN.cpp) and
# the rest of the engine. Exercise 9 is a type (ej09.hpp): its test includes it, so it
# is not linked
$(BIN)/p%: $(OBJ)/ejercicios/pruebas/p%.o $(OBJ)/ejercicios/ej%.o $(OBJS) $(LIB)
	@mkdir -p $(BIN)
	$(CXX) $(LDFLAGS) $^ -o $@ $(RAYLIB)
$(BIN)/p09: $(OBJ)/ejercicios/pruebas/p09.o $(OBJS) $(LIB)
	@mkdir -p $(BIN)
	$(CXX) $(LDFLAGS) $^ -o $@ $(RAYLIB)

# An exercise: its tests, P1 and P2
ej%:
	@$(MAKE) -s $(BIN)/p$* && ./$(BIN)/p$*

# For the debugger of VS Code (.vscode/), with the file open in the editor, CASO (an
# exercise, ej01, or its test, p01): make prueba builds its tests as bin/prueba, and
# make juego the game with it, as con/arena
CASO ?=
ID   := $(patsubst ej%,%,$(patsubst p%,%,$(CASO)))
prueba:
	@test -f src/ejercicios/pruebas/p$(ID).cpp || { echo "❌ Abre en el editor \
	el fichero de un ejercicio (ej01.cpp…), y vuelve a empezar"; exit 1; }
	@$(MAKE) -s $(BIN)/p$(ID)
	@cp $(BIN)/p$(ID) $(BIN)/prueba
juego:
	@test -f src/ejercicios/ej$(ID).cpp || { echo "❌ Abre en el editor el \
	fichero .cpp de un ejercicio (ej01.cpp…), y vuelve a empezar"; exit 1; }
	@$(MAKE) -s $(CON_BIN) CON=ej$(ID)

clean:
	rm -rf build dist
