
directories = ev3c_library ev3cleti_library

all:
	$(foreach dir, $(directories), make -C $(dir);)

install:
	$(foreach dir, $(directories), make install -C $(dir);)

clean:
	$(foreach dir, $(directories), make clean -C $(dir);)
