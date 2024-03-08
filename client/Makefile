BASE_NAME := rush-hour
TARGET_DIR = .
TARGET := $(BASE_NAME)
TARGET_ARCHIVE := $(BASE_NAME).tar.gz

TESTS_RUNNER := $(BASE_NAME)-tests-runner
TEST_SRC_FILES := $(wildcard *_test.cpp)
TEST_OBJ_FILES := $(patsubst %.cpp,%.o,$(TEST_SRC_FILES))

MAIN_SRC_FILES = $(filter-out $(TEST_SRC_FILES), $(wildcard *.cpp))
MAIN_OBJ_FILES := $(filter-out $(TEST_OBJ_FILES), $(patsubst %.cpp,%.o,$(MAIN_SRC_FILES)))

ENGINE_BASE_NAME := engine
ENGINE_DIR := $(ENGINE_BASE_NAME)
ENGINE_LIB_NAME := $(ENGINE_BASE_NAME)
ENGINE_LIB_FILENAME := lib$(ENGINE_LIB_NAME).so
DEPENDENCIES_DIR := dependencies

CXX = g++
CXX_FLAGS = -fPIC -std=c++11 -O2
CXX_FLAGS += -I../$(ENGINE_DIR)
LD_FLAGS = -L../$(ENGINE_DIR) -l$(ENGINE_LIB_NAME)
TAR_GZ = tar zcvf
LIBS = -lglut -lGL -lGLU

package: $(TARGET_ARCHIVE)
	@echo "all done!"

$(TARGET_ARCHIVE): $(TARGET)
	$(TAR_GZ) $@ $(TARGET) ../$(ENGINE_DIR)/$(ENGINE_LIB_FILENAME)
	@echo "$(TARGET) package done!"

$(TARGET): $(MAIN_OBJ_FILES)
	$(CXX) $(MAIN_OBJ_FILES) $(LD_FLAGS) -o $@
	@echo "$(TARGET) compilation completed."

$(TESTS_RUNNER): $(TEST_OBJ_FILES)
	$(CXX) -o $@ $(TEST_OBJ_FILES) $(LIBS)
	@touch $(TESTS_RUNNER)
	@echo "$(TESTS_RUNNER) build done!"
	./$(TESTS_RUNNER)
	@echo "tests execution done!"

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

clean:
	@rm -f $(TESTS_RUNNER)
	@rm -f $(TEST_OBJ_FILES)
	@rm -f $(TARGET)
	@rm -f $(MAIN_OBJ_FILES)
	@rm -f $(TARGET_ARCHIVE)

.PHONY: clean package
