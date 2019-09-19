CXXFLAGS = -O3 -Wall
CPPFLAGS += -std=c++11

examples:

ALL = examples ccx

all: $(ALL)

clean:
	$(RM) $(ALL)
