CC = gcc

SRCS = audisen_sim.c ams.c frame.c amp.c
OBJS = $(SRCS:.c=.o)
EXEC = audisen


$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(OBJS)
%.o: %.c
	$(CC) -c $< -o $@



clean:
	rm -f $(OBJS) $(EXEC)
