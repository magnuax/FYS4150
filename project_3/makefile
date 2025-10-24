
#------------------------------
# Some default flags and paths
#------------------------------
BUILD 		:= build
LIB			:= 
INCL 		:= -I./include
CXXFLAGS 	:= -DARMA_USE_HDF5 -std=c++17 -O2 
LDFLAGS 	:= -larmadillo
PY			:= 
EXE			:= 
LOGFILE 	:= problem9.log

#------------------------------
# OS specific settings
#------------------------------

# Distinguishing between mac/linux and windows:
UNAME 		:= $(strip $(OS))

# Checking whether folder exists or not is different between mac/linux and windows
ifeq ($(UNAME),Windows_NT)
$(info I am sorry for using Windows)
INCL 		+= -I"C:/vcpkg/installed/x64-mingw-dynamic/include"
LIB 		:= -L"C:/vcpkg/installed/x64-mingw-dynamic/lib"
LDFLAGS   	+= -lhdf5 -lhdf5_hl
DELETE		:= del /Q
PY 			+= py 
EXE			:= .exe

define MKDIR 
	if not exist "$(1)" mkdir "$(1)"
endef 

else 
$(info I can't update my mac but I love it so much)
CXXFLAGS += $(shell pkg-config --cflags hdf5)
LDFLAGS  += $(shell pkg-config --libs hdf5)
DELETE		:= rm -f
PY 			+= python3

define MKDIR 
	mkdir -p "$(1)"
endef 

endif 

# --- fixed definitions for Windows ---
problem_8 = "$(BUILD)/main$(EXE)" --problem_8 $(1)
problem_9 = "$(BUILD)/main$(EXE)" --problem_9 --n_steps 4000 --n_particles 100 --n_freq 101 $(1) >> $(LOGFILE) 2>&1 


define LOGTIME
	@echo "Log time: $$(date)" $(LOG_APPEND)
endef



#------------------------------
# Makefile rules
#------------------------------

OS_message:
	@echo $(UNAME)

outfolder:
	@$(call MKDIR,$(BUILD))

compile:
	g++ -c $(wildcard src/*.cpp) $(INCL) $(LIB) $(CXXFLAGS)
	g++ -c main.cpp -o main.o	 $(INCL) $(LIB) $(CXXFLAGS) 

link:
	g++ $(wildcard *.o) $(LIB) $(LDFLAGS) -o $(BUILD)/main

clean:
	-$(DELETE) *.o

all: outfolder compile link clean

.PHONY: run_problem_8 run_problem_9 py_plots project

run_problem_8:
	@echo "Running Problem 8 simulations..."
	$(call problem_8)

run_problem_9:
	@echo "Running Problem 9 simulations..."

	$(call problem_9,--amplitude 0.1)
	$(call problem_9,--amplitude 0.4)
	$(call problem_9,--amplitude 0.7)

	$(call problem_9,--amplitude 0.1 --freq_range 1.0 1.7)
	$(call problem_9,--amplitude 0.4 --freq_range 1.0 1.7)
	$(call problem_9,--amplitude 0.7 --freq_range 1.0 1.7)

	$(call problem_9,--amplitude 0.1 --freq_range 1.0 1.7 --non-interacting)
	$(call problem_9,--amplitude 0.4 --freq_range 1.0 1.7 --non-interacting)
	$(call problem_9,--amplitude 0.7 --freq_range 1.0 1.7 --non-interacting)

py_plots:
	$(PY) plot_errors.py
	$(PY) plot_trajectories.py
	$(PY) plot_trapped.py

# Build first (via 'all'), then run everything and plot
project: all run_problem_8 run_problem_9 py_plots
	@echo "Complete project finished."