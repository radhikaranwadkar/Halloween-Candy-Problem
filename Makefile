CC = gcc

CFLAGS = -std=c99 -O3 -fopenmp
LDFLAGS = -lm -lgomp

#SOURCES
RM  = rm -f
EXEC = SOLVE_FOR_CANDIES

OBJECTS = solution1.o infra.o solve_omp.o

all : $(EXEC)
	time ./$(EXEC) | tee log 

$(EXEC) : $(OBJECTS) touch
	rm -f $(EXEC)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

%.o : %.c infra.h solve_omp.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean

clean:
	$(RM) $(OBJECTS) $(EXEC)

touch: 
	@echo " "
	@echo "Compilation done successfully..................."
	@echo "Executable generated: SOLVE_FOR_CANDIES "
	@echo "Output file: 'output.txt' "
	@echo " "



