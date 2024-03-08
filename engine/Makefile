BASE_NAME := engine
TARGET_DIR = .
TARGET := lib$(BASE_NAME).so

TESTS_RUNNER := $(BASE_NAME)-tests-runner
TEST_SRC_FILES := engine_test.cpp
TEST_OBJ_FILES := engine_test.o camera.o cube.o directional_light.o light.o material.o mesh.o my_engine.o node.o object.o ortho_camera.o ovo_parser.o perspective_camera.o point_light.o sphere.o spot_light.o texture.o

MAIN_SRC_FILES := camera.cpp cube.cpp directional_light.cpp light.cpp material.cpp mesh.cpp my_engine.cpp node.cpp object.cpp ortho_camera.cpp ovo_parser.cpp perspective_camera.cpp point_light.cpp sphere.cpp spot_light.cpp texture.cpp
MAIN_OBJ_FILES := camera.o cube.o directional_light.o light.o material.o mesh.o my_engine.o node.o object.o ortho_camera.o ovo_parser.o perspective_camera.o point_light.o sphere.o spot_light.o texture.o

DEPENDENCIES_DIR := dependencies

CXX = g++
CXX_FLAGS = -c -fPIC -std=c++11 -O2
LD_FLAGS = -shared
LIBS = -lglut -lGL -lGLU -lfreeimage

install: $(TARGET)
	@echo "all done!"

$(TARGET): $(MAIN_OBJ_FILES) $(TESTS_RUNNER)
	$(CXX) $(LD_FLAGS) -o $@ $(MAIN_OBJ_FILES) $(LIBS)
	@echo "$(TARGET) build done!"

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

.PHONY: clean install
