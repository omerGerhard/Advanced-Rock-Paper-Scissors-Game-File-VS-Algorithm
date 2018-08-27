COMP = g++-5.3.0

OBJS = main.o Flag.o Game.o Joker.o Paper.o Piece.o Player.o Rock.o Scissors.o AutoPlayerAlgorithm.o FilePlayerAlgorithm.o MyBoard.o MyFightInfo.o MyJokerChange.o MyMove.o MyPiecePosition.o
EXEC = ex2
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra -Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@

main.o: main.cpp Game.h FilePlayerAlgorithm.h AutoPlayerAlgorithm.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
AutoPlayerAlgorithm.o: AutoPlayerAlgorithm.cpp AutoPlayerAlgorithm.h MyPoint.h MyPiecePosition.h Piece.h MyMove.h MyJokerChange.h Joker.h MyBoard.h PlayerAlgorithm.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
FilePlayerAlgorithm.o: FilePlayerAlgorithm.cpp FilePlayerAlgorithm.h Game.h Piece.h Player.h Joker.h MyPoint.h MyPiecePosition.h MyMove.h MyJokerChange.h PlayerAlgorithm.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Flag.o: Flag.cpp Flag.h Piece.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Game.o: Game.cpp Game.h Player.h Piece.h MyFightInfo.h Joker.h FilePlayerAlgorithm.h AutoPlayerAlgorithm.h MyBoard.h MyPoint.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Joker.o: Joker.cpp Joker.h Piece.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
MyBoard.o: MyBoard.cpp MyBoard.h Point.h Player.h Piece.h Board.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
MyFightInfo.o: MyFightInfo.cpp MyFightInfo.h Player.h Piece.h FightInfo.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
MyJokerChange.o: MyJokerChange.cpp MyJokerChange.h JokerChange.h MyPoint.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
MyMove.o: MyMove.cpp MyMove.h MyPoint.h Move.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
MyPiecePosition.o: MyPiecePosition.cpp MyPiecePosition.h Piece.h PiecePosition.h MyPoint.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Paper.o: Paper.cpp Paper.h Piece.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Piece.o: Piece.cpp Piece.h Player.h Game.h Scissors.h Joker.h Rock.h Bomb.h Paper.h Flag.h MyFightInfo.h MyPoint.h Point.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Player.o: Player.cpp Player.h Piece.h Joker.h MyFightInfo.h PiecePosition.h PlayerAlgorithm.h MyPoint.h Game.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Rock.o: Rock.cpp Rock.h Piece.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Scissors.o: Scissors.cpp Scissors.h Piece.h
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
clean:
	rm -f $(OBJS) $(EXEC)