# All Targets
all: studio

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.

studio: bin/Workout.o bin/Customer.o bin/Trainer.o bin/Action.o bin/Studio.o bin/main.o
	@echo 'Building target: studio'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/studio bin/Workout.o bin/Customer.o bin/Trainer.o bin/Action.o bin/Studio.o bin/main.o
	@echo 'Finished building target: studio'
	@echo ' '

# Depends on the source and header files
bin/Workout.o: src/Workout.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Workout.o src/Workout.cpp

# Depends on the source and header files
bin/Customer.o: src/Customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Customer.o src/Customer.cpp

# Depends on the source and header files
bin/Trainer.o: src/Trainer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Trainer.o src/Trainer.cpp

# Depends on the source and header files
bin/Action.o: src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

# Depends on the source and header files
bin/Studio.o: src/Studio.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Studio.o src/Studio.cpp

# Depends on the source and header files
bin/main.o: src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

#Clean the build directory
clean:
	rm -f bin/*