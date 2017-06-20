CXX = c++
CPP_FLAGS =
CXX_FLAGS = -std=c++14 -fopenmp
OPT_FLAGS = -O3 -Wno-strict-overflow
DEBUG_FLAGS = -g -Wall -Wextra -pedantic
LD_FLAGS =
HEADER_PATHS =
SRCS =
GTEST_SRCS =

# Runtime mode.
ifeq ($(NOVIZ),1)
SRCS_MAIN = src/driver.cpp
else
SRCS_MAIN = src/visual_driver.cpp
LD_FLAGS += -L/usr/X11R6/lib -lm -lpthread -lX11
SRCS +=	src/viz/SolutionViewer.cpp
endif

# TEST=1 should be an argument if google unit tests are desired.
# Make this first so that no make arguments are required.
ifeq ($(TEST),1)
all: lbmpp lbmpp_tests
	./lbmpp_tests
else
all: lbmpp
endif

ifeq ($(TEST),1)
# Define GTEST_DIR according to your setup!
GTEST_DIR = ../googletest/googletest/
# Don't need to touch the following.
HEADER_PATHS += -I$(GTEST_DIR)/include/
GTEST_FLAGS = -isystem $(GTEST_DIR)/include -g -Wall -Wextra -pthread
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(GTEST_FLAGS) -I$(GTEST_DIR) -c $(GTEST_DIR)/src/gtest-all.cc
gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(GTEST_FLAGS) -I$(GTEST_DIR) -c $(GTEST_DIR)/src/gtest_main.cc
gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^
gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^
endif

# Source files.
SRCS +=	src/sim/Simulator.cpp
SRCS +=	src/cell/Cell.cpp
SRCS +=	src/grid/Grid.cpp
SRCS +=	src/grid/GridLevel.cpp
SRCS +=	src/grid/BoundaryConditions.cpp
# Test files.
ifeq ($(TEST),1)
GTEST_SRCS += src/main.t.cpp
GTEST_SRCS += src/Stopwatch.t.cpp
endif

%.o: %.cpp
	$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) $(OPT_FLAGS) $(DEBUG_FLAGS) \
		-c $< -o $@ \
		$(LD_FLAGS) $(HEADER_PATHS)

OBJS = $(SRCS:.cpp=.o)
OBJS_MAIN = $(SRCS_MAIN:.cpp=.o)

ifeq ($(TEST),1)
GTEST_OBJS = $(GTEST_SRCS:.cpp=.o)
GTEST_OBJS += gtest_main.a
endif

# Finally, creating the main application.
lbmpp: $(OBJS_MAIN) $(OBJS)
	$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) $(OPT_FLAGS) $(DEBUG_FLAGS) \
		$^ -o $@ \
		$(LD_FLAGS) $(HEADER_PATHS)

ifeq ($(TEST),1)
lbmpp_tests: $(GTEST_OBJS) $(OBJS)
	$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) $(OPT_FLAGS) $(DEBUG_FLAGS) \
		$^ -o $@ \
		$(LD_FLAGS) $(HEADER_PATHS)
endif

CLEANFILES = lbmpp lbmpp_tests
CLEANFILES += $(OBJS_MAIN) $(OBJS)
CLEANFILES += src/*.o
CLEANFILES += *.a
CLEANFILES += *.o
# Cluster output files.
CLEANFILES += *.o[0-9][0-9]*
CLEANFILES += *.e[0-9][0-9]*
CLEANFILES += *.btr

clean:
	rm -f *~ $(CLEANFILES)

# eof
