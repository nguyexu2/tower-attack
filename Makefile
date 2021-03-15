LOGIN=nguyexu2
CXX=g++
CXXFLAGS=-Wall -pedantic -std=c++11 -O2 -g
LDLIBS=-lncurses

HPP=./src/hpp/
CPP=./src/cpp/
OBJ=./obj/

DEPENDENCY=ls -l $(CPP) | tail -n +2 | awk '{printf("$$(OBJ)%s ", $$NF)}' | sed s/.cpp/.o/g
#cut off "amount in folder" line from ls
#add $(OBJ) infront, add a space instead of '\n' at end
#substitute suffix .cpp to .o
#note: $$ to escape makefile $sign
#------------------------------------------------------------------------------------------------

all: compile doc

compile: $(LOGIN)

run: compile
	./$(LOGIN)
 
.PHONY: clean
clean:
	rm -rf $(LOGIN) $(OBJ) doc/ 2>/dev/null

doc: $(HPP)
	doxygen Doxyfile

#------------------------------------------------------------------------------------------------
#copy the generated dependency into $(login) dependency
dependency:
	$(DEPENDENCY)

$(LOGIN): $(OBJ)basic.o $(OBJ)chargeTower.o $(OBJ)defensive.o $(OBJ)gameEngine.o $(OBJ)gameState.o $(OBJ)gate.o $(OBJ)goal.o $(OBJ)input.o $(OBJ)joker.o $(OBJ)main.o $(OBJ)map.o $(OBJ)nodes.o $(OBJ)object.o $(OBJ)tower.o $(OBJ)towerAttack.o $(OBJ)wall.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDLIBS)

obj:
	mkdir $(OBJ)

valgrind: compile
	valgrind --leak-check=full --show-leak-kinds=all ./$(LOGIN)

val: compile
	valgrind --leak-check=full ./$(LOGIN)

#$(OBJ)%.o: $(CPP)%.cpp | obj
#$(CXX) $(CXXFLAGS) -c $< -o $@
#$(CXX) -MM $(CXXLAGS) $(CPP)$*.cpp > $(OBJ)$*.d


#------------------------------------------------------------------------------------------------
#g++ -MM src/cpp * >> Makefile

$(OBJ)basic.o: $(CPP)basic.cpp $(HPP)basic.h $(HPP)object.h $(HPP)nodes.h $(HPP)exceptions.h | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)chargeTower.o: $(CPP)chargeTower.cpp $(HPP)chargeTower.h $(HPP)tower.h $(HPP)object.h $(HPP)nodes.h $(HPP)exceptions.h | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)defensive.o: $(CPP)defensive.cpp $(HPP)defensive.h $(HPP)basic.h $(HPP)object.h $(HPP)nodes.h $(HPP)exceptions.h | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)gameEngine.o: $(CPP)gameEngine.cpp $(HPP)gameEngine.h $(HPP)map.h $(HPP)nodes.h $(HPP)basic.h $(HPP)object.h $(HPP)all_static.h $(HPP)wall.h $(HPP)gate.h $(HPP)all_moving.h $(HPP)defensive.h $(HPP)joker.h $(HPP)goal.h $(HPP)tower.h $(HPP)chargeTower.h $(HPP)gameState.h $(HPP)input.h $(HPP)towerAttack.h $(HPP)exceptions.h | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)gameState.o: $(CPP)gameState.cpp $(HPP)gameState.h $(HPP)nodes.h $(HPP)gate.h $(HPP)object.h $(HPP)all_moving.h $(HPP)basic.h $(HPP)defensive.h $(HPP)joker.h $(HPP)input.h $(HPP)towerAttack.h $(HPP)exceptions.h | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)gate.o: $(CPP)gate.cpp $(HPP)gate.h $(HPP)object.h $(HPP)nodes.h $(HPP)all_moving.h $(HPP)basic.h $(HPP)defensive.h $(HPP)joker.h $(HPP)exceptions.h | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)goal.o: $(CPP)goal.cpp $(HPP)goal.h $(HPP)object.h $(HPP)nodes.h | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)input.o: $(CPP)input.cpp $(HPP)input.h | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)joker.o: $(CPP)joker.cpp $(HPP)joker.h $(HPP)basic.h $(HPP)object.h $(HPP)nodes.h | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)main.o: $(CPP)main.cpp $(HPP)towerAttack.h | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)map.o: $(CPP)map.cpp $(HPP)map.h $(HPP)nodes.h $(HPP)basic.h $(HPP)object.h $(HPP)all_static.h $(HPP)wall.h $(HPP)gate.h $(HPP)all_moving.h $(HPP)defensive.h $(HPP)joker.h $(HPP)goal.h $(HPP)tower.h $(HPP)chargeTower.h $(HPP)towerAttack.h $(HPP)exceptions.h | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)nodes.o: $(CPP)nodes.cpp $(HPP)nodes.h $(HPP)object.h | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)object.o: $(CPP)object.cpp $(HPP)object.h $(HPP)nodes.h $(HPP)map.h $(HPP)basic.h $(HPP)all_static.h $(HPP)wall.h $(HPP)gate.h $(HPP)all_moving.h $(HPP)defensive.h $(HPP)joker.h $(HPP)goal.h $(HPP)tower.h $(HPP)chargeTower.h $(HPP)exceptions.h | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)tower.o: $(CPP)tower.cpp $(HPP)tower.h $(HPP)object.h $(HPP)nodes.h | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)towerAttack.o: $(CPP)towerAttack.cpp $(HPP)towerAttack.h $(HPP)gameEngine.h $(HPP)map.h $(HPP)nodes.h $(HPP)basic.h $(HPP)object.h $(HPP)all_static.h $(HPP)wall.h $(HPP)gate.h $(HPP)all_moving.h $(HPP)defensive.h $(HPP)joker.h $(HPP)goal.h $(HPP)tower.h $(HPP)chargeTower.h $(HPP)gameState.h $(HPP)input.h | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ)wall.o: $(CPP)wall.cpp $(HPP)wall.h $(HPP)object.h $(HPP)nodes.h | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@