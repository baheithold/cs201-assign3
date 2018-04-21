#Makefile modified from magarwal on the Beastie forums.
#Created 03/23/2018.

OBJS 		  = integer.o real.o string.o sll.o dll.o queue.o scanner.o bst.o
MAIN 		  = avl.o vertex.o edge.o
TESTS 		  = $(BINOMIALtests) $(PRIMtests)
PRIM  		  = prim
OOPTS 		  = -Wall -Wextra -std=c99 -g -c
LOPTS 		  = -Wall -Wextra -std=c99 -g
PRIMtests 	  = p-0-0 p-0-1 p-0-2 p-0-3 p-0-4 p-0-5 p-0-6 p-0-7 p-0-8 p-0-9 p-0-10
BINOMIALtests = binomial-0-0 binomial-0-1 binomial-0-2 binomial-0-3 \
				binomial-0-4 binomial-0-5 binomial-0-6 binomial-0-7 binomial-0-8 \
				binomial-0-9 binomial-0-10

all: 	$(OBJS) $(MAIN) $(PRIM)

################################################################################
#                                                 	Classes for Primitive Types
#INTEGER
integer.o: 	integer.c integer.h
	gcc $(OOPTS) integer.c

#REAL
real.o: real.c real.h
	gcc $(OOPTS) real.c

#STRING
string.o: 	string.c string.h
	gcc $(OOPTS) string.c

################################################################################
#                                                                         VERTEX

vertex.o: 	vertex.c vertex.h
	gcc $(OOPTS) vertex.c

################################################################################
#                                                                         EDGE

edge.o: 	edge.c edge.h
	gcc $(OOPTS) edge.c

################################################################################
#                                                                         SLL

sll.o: 	sll.c sll.h
	gcc $(OOPTS) sll.c

################################################################################
#                                                                         DLL

dll.o: 	dll.c dll.h
	gcc $(OOPTS) dll.c

################################################################################
#                                                                         QUEUE

queue.o: 	queue.c queue.h sll.h
	gcc $(OOPTS) queue.c

################################################################################
#                                                                         BST

bst.o: 	bst.c bst.h queue.h
	gcc $(OOPTS) bst.c

################################################################################
#                                                                         AVL

avl.o: 	avl.c avl.h bst.h
	gcc $(OOPTS) avl.c

################################################################################
#                                                                      scanner

scanner.o: 	scanner.c
	gcc $(OOPTS) scanner.c

################################################################################
#                                                                         prim

prim: 	prim.c $(OBJS) $(MAIN)
	gcc $(LOPTS) prim.c $(OBJS) $(MAIN) -o prim

################################################################################
#                                                						Test

test: 	all
	./prim ./Testing/0/p-0-0.data

################################################################################
#                                            							Valgrind

valgrind: 	all
	valgrind ./prim ./Testing/0/p-0-0.data

################################################################################
#                                                         				Clean

clean:
	rm -f *.o vgcore.* $(MAIN) $(TESTS) $(PRIM)
