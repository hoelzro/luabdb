CC = gcc
CFLAGS = -O2
OBJECTS = cursor_ops.o \
    db_config.o \
    db_ops.o \
    env_config.o \
    env_ops.o \
    lock_config.o \
    lock_ops.o \
    log_config.o \
    log_ops.o \
    mem_pool_config.o \
    mem_pool_ops.o \
    mutex_config.o \
    mutex_ops.o \
    replication_config.o \
    replication_ops.o \
    sequence_config.o \
    sequence_ops.o \
    txn_config.o \
    txn_ops.o \
    bdb.o

bdb.so: $(OBJECTS)
	gcc -o bdb.so -shared $^ -llua -lm -ldl -ldb

%.o : %.c
	$(CC) $(CFLAGS) -c -fPIC $<

clean:
	rm -f *.o *.so
