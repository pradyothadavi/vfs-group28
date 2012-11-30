VFSDIR	=	..

CC	=	gcc

CFLAGS	=	-g -I$(VFSDIR)/include -ansi -pedantic

OBJS	=	$(VFSDIR)/src/freeList.o $(VFSDIR)/src/hashTable.o $(VFSDIR)/src/nAryTree.o $(VFSDIR)/src/binarySearchTree.o $(VFSDIR)/src/createvfs.o $(VFSDIR)/src/mountvfs.o $(VFSDIR)/src/unmountvfs.o $(VFSDIR)/src/fileDescriptor.o $(VFSDIR)/src/makedir.o $(VFSDIR)/src/deletedir.o $(VFSDIR)/src/movedir.o $(VFSDIR)/src/listdir.o $(VFSDIR)/src/addfile.o $(VFSDIR)/src/listfile.o $(VFSDIR)/src/updatefile.o $(VFSDIR)/src/removefile.o $(VFSDIR)/src/movefile.o $(VFSDIR)/src/copyfile.o $(VFSDIR)/src/exportfile.o $(VFSDIR)/src/searchfile.o $(VFSDIR)/src/main.o

TARGET	=	vfs_group28

$(TARGET):	$(OBJS)
	$(CC) -o $(VFSDIR)/bin/$@ $(OBJS)
	mv $(VFSDIR)/src/*.o $(VFSDIR)/bin


$(OBJS) : %.o : %.c
	$(CC) -c $(CFLAGS) -o $@ $<

all: myVfs_exe

clean:
	rm -f $(VFSDIR)/bin/*.o core $(VFSDIR)/src/*.o core $(VFSDIR)/bin/$(TARGET) $(VFSDIR)/bin/*

