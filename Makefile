TARGETS = server client server-api
CPPFLAGS = -g -Wall -Werror -pthread

SRC_CPP_FILES := $(wildcard *.cpp)

$(OBJS_CPP): $(SRC_CPP_FILES)
	$(CXX) $(CPPFLAGS) $^ -c
	
all: $(TARGETS)

helper: helper.cpp candidate.o voter.o
	$(CXX) $(CPPFLAGS) $^ -o $@
	
server: server.cpp helper.o candidate.o voter.o
	$(CXX) $(CPPFLAGS) $^ -o $@

client: client.cpp
	$(CXX) $(CPPFLAGS) $^ -o $@

	
server-api: server-api.cpp helper.o candidate.o voter.o
	$(CXX) $(CPPFLAGS) $^ -o $@
	
.PHONY : clean
clean::
	rm -fv $(TARGETS) *~ *.o