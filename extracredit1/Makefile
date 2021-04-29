TARGETS = server client
CPPFLAGS = -g -Wall -Werror -pthread

SRC_CPP_FILES := $(wildcard *.cpp)

$(OBJS_CPP): $(SRC_CPP_FILES)
	$(CXX) $(CPPFLAGS) $^ -c

all: $(TARGETS)

helper2: helper2.cpp candidate.o voter.o
	$(CXX) $(CPPFLAGS) $^ -o $@

server: server.cpp helper2.o candidate.o voter.o
	$(CXX) $(CPPFLAGS) $^ -o $@

client: client.cpp helper2.o candidate.o voter.o
	$(CXX) $(CPPFLAGS) $^ -o $@

.PHONY : clean
clean::
	rm -fv $(TARGETS) *~ *.o