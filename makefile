#Makefile modified from magarwal on the Beastie forums.
#Created 03/23/2018.

OBJS 		  = integer.o sll.o dll.o queue.o scanner.o bst.o avl.o binomial.o \
				vertex.o edge.o
OOPTS 		  = -Wall -Wextra -std=c99 -g -c
LOPTS 		  = -Wall -Wextra -std=c99 -g
PRIMtests 	  = p-0-0 p-0-1 p-0-2 p-0-3 p-0-4 p-0-5 p-0-6 p-0-7 p-0-8 p-0-9 p-0-10

all: 	$(OBJS) prim

################################################################################
#                                                 	Classes for Primitive Types
#INTEGER
integer.o: 	integer.c integer.h
	gcc $(OOPTS) integer.c

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
#                                                                         BINOMIAL

binomial.o: 	binomial.c binomial.h queue.h dll.h
	gcc $(OOPTS) binomial.c

################################################################################
#                                                                      scanner

scanner.o: 	scanner.c
	gcc $(OOPTS) scanner.c

################################################################################
#                                                                         prim

prim: 	prim.c $(OBJS)
	gcc $(LOPTS) prim.c $(OBJS) -o prim -lm

################################################################################
#                                                						Test

test: 	all
	@echo Testing p-0-0...
	@./prim ./Testing/0/p-0-0.data > ./Testing/0/actual/p-0-0.actual
	@diff ./Testing/0/expected/p-0-0.expected ./Testing/0/actual/p-0-0.actual
	@echo Testing p-0-1...
	@./prim ./Testing/0/p-0-1.data > ./Testing/0/actual/p-0-1.actual
	@diff ./Testing/0/expected/p-0-1.expected ./Testing/0/actual/p-0-1.actual
	@echo Testing p-0-2...
	@./prim ./Testing/0/p-0-2.data > ./Testing/0/actual/p-0-2.actual
	@diff ./Testing/0/expected/p-0-2.expected ./Testing/0/actual/p-0-2.actual
	@echo Testing p-0-3...
	@./prim ./Testing/0/p-0-3.data > ./Testing/0/actual/p-0-3.actual
	@diff ./Testing/0/expected/p-0-3.expected ./Testing/0/actual/p-0-3.actual
	@echo Testing p-0-4...
	@./prim ./Testing/0/p-0-4.data > ./Testing/0/actual/p-0-4.actual
	@diff ./Testing/0/expected/p-0-4.expected ./Testing/0/actual/p-0-4.actual
	@echo Testing p-0-5...
	@./prim ./Testing/0/p-0-5.data > ./Testing/0/actual/p-0-5.actual
	@diff ./Testing/0/expected/p-0-5.expected ./Testing/0/actual/p-0-5.actual
	@echo Testing p-0-6...
	@./prim ./Testing/0/p-0-6.data > ./Testing/0/actual/p-0-6.actual
	@diff ./Testing/0/expected/p-0-6.expected ./Testing/0/actual/p-0-6.actual
	@echo Testing p-0-7...
	@./prim ./Testing/0/p-0-7.data > ./Testing/0/actual/p-0-7.actual
	@diff ./Testing/0/expected/p-0-7.expected ./Testing/0/actual/p-0-7.actual
	@echo Testing p-0-8...
	@./prim ./Testing/0/p-0-8.data > ./Testing/0/actual/p-0-8.actual
	@diff ./Testing/0/expected/p-0-8.expected ./Testing/0/actual/p-0-8.actual
	@echo Testing p-0-9...
	@./prim ./Testing/0/p-0-9.data > ./Testing/0/actual/p-0-9.actual
	@diff ./Testing/0/expected/p-0-9.expected ./Testing/0/actual/p-0-9.actual
	@echo Testing p-0-10...
	@./prim ./Testing/0/p-0-10.data > ./Testing/0/actual/p-0-10.actual
	@diff ./Testing/0/expected/p-0-10.expected ./Testing/0/actual/p-0-10.actual

################################################################################
#                                            							Valgrind

valgrind: 	all
	valgrind ./prim ./Testing/0/p-0-0.data

################################################################################
#                                                         				Clean

clean:
	rm -f *.o vgcore.* prim
