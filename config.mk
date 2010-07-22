CC = gcc
CFLAGS = -O2

OBJECTS = cursor_ops.o \
    db_ops.o \
    env_ops.o \
    flags.o \
    lock_ops.o \
    log_ops.o \
    mem_pool_ops.o \
    mutex_ops.o \
    replication_ops.o \
    sequence_ops.o \
    txn_ops.o \
    bdb.o
