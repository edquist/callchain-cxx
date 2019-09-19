CXXFLAGS = -O3 -Wall
CPPFLAGS += -std=c++11

ALL = examples

all: $(ALL)

clean:
	$(RM) $(ALL)
