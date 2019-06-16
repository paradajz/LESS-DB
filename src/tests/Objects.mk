TESTS=dbms
TESTS_EXT := $(addprefix build/,$(TESTS))
TESTS_EXT := $(addsuffix .out,$(TESTS_EXT))

.DEFAULT_GOAL := all
all: $(TESTS_EXT)

OBJECTS_dbms := $(addprefix build/,$(SOURCES_dbms))
OBJECTS_dbms := $(OBJECTS_dbms:.c=.o)
OBJECTS_dbms := $(OBJECTS_dbms:.cpp=.o)
-include $(OBJECTS_dbms:%.o=%.d)

build/dbms.out: $(OBJECTS_dbms) build/gtest_main.a
	@echo Linking
	@$(CXX) $(CPP_FLAGS) -lpthread $^ -o $@
	@echo Created executable: $@
