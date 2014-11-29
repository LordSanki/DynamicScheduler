CC   = gcc
CXX  = g++

INCLUDE     = -I.
CFLAGS  = $(INCLUDE) -g -Wall
LFLAGS  =
LIBS    =

SRC     = main.cpp DispatchQueue.cpp ExecutionQueue.cpp SchedulerQueue.cpp Instruction.cpp
HEADERS = DispatchQueue.h ExecutionQueue.h Instruction.h RegisterFile.h SchedulerQueue.h TraceReader.h

TARGET  = sim

OBJECTS = $(SRC:%.cpp=%.o)

all: BINARY

clean:
	@rm -f $(OBJECTS) $(TARGET) .depend

clobber:
	@rm -f $(OBJECTS)

BINARY: $(OBJECTS) $(HEADERS)
	@$(CXX) $(LFLAGS) $(OBJECTS) $(LIBS) -o $(TARGET)

%.o: %.cpp
	@$(CXX) -c $(CFLAGS) $^ -o $@

