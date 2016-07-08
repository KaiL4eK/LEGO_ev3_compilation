include ../common.mk

library_file := lib/libev3c.a

install_directory := $(ev3c_lib_directory)

all: $(library_file) bin/test_sensor bin/test_motor bin/test_lcd bin/test_button bin/test_led bin/test_battery

obj/%.o: src/%.c include/%.h include/ev3c.h Makefile
	mkdir -p $(@D)
	$(CC) -c -o $@ $< $(cflags) -Iinclude

$(library_file): obj/ev3c_core.o obj/ev3c_lcd.o obj/ev3c_sensor.o obj/ev3c_motor.o obj/ev3c_button.o obj/ev3c_led.o obj/ev3c_battery.o
	mkdir -p $(@D)
	$(AR) rsc $@ $^

bin/test_sensor: src/test_sensor.c $(library_file)
	mkdir -p $(@D)
	$(CC) -o $@ $< $(library_file) $(cflags) -Iinclude

bin/test_motor: src/test_motor.c $(library_file)
	mkdir -p $(@D)
	$(CC) -o $@ $< $(library_file) $(cflags) -Iinclude

bin/test_lcd: src/test_lcd.c $(library_file)
	mkdir -p $(@D)
	$(CC) -o $@ $< $(library_file) $(cflags) -Iinclude

bin/test_button: src/test_button.c $(library_file)
	mkdir -p $(@D)
	$(CC) -o $@ $< $(library_file) $(cflags) -Iinclude

bin/test_led: src/test_led.c $(library_file)
	mkdir -p $(@D)
	$(CC) -o $@ $< $(library_file) $(cflags) -Iinclude

bin/test_battery: src/test_battery.c $(library_file)
	mkdir -p $(@D)
	$(CC) -o $@ $< $(library_file) $(cflags) -Iinclude

documentation: include/*.h
	mkdir -p /tmp/ev3DocumentationTemp
	naturaldocs -i . -xi ./ev3c_documentation -o HTML ev3c_documentation -p /tmp/ev3DocumentationTemp

.PHONY: all clean

install: all $(install_directory)
	cp -r include $(install_directory)
	cp -r lib $(install_directory)

$(install_directory):
	mkdir -p $@

clean:
	rm -rf obj lib bin
	rm -f *~
