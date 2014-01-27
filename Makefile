SOURCES=queue.c domain.c search.c hash.c
INCLUDES=$(SRCS:%.c=%.h)

search : $(INCLUDES) $(SOURCES)
	gcc -lm -o search $(SOURCES)

debug : $(INCLUDES) $(SOURCES)
	gcc -ggdb -lm -o search $(SOURCES)

domain : 
	rm -f domain.c domain.h
	ln -s $(DOM).c domain.c
	ln -s $(DOM).h domain.h
	touch domain.c

clean :
	rm -f search *.o
