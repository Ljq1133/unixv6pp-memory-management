include Makefile.inc

.PHONY : all build deploy run prepare $(DIRS)
.NOTPARALLEL:

PROGRAMS =	cat	\
			cp	\
			ls	\
			mkdir \
			rm	\
			date \
			perf \
			rm

all: build deploy 

.PHONY: qemug
qemug: all
	qemu-system-i386 -m 32M -audiodev pa,id=hda -machine pcspk-audiodev=hda -rtc base=localtime \
	-boot c -drive file=../targets/UNIXV6++/c.img,if=ide,index=0,media=disk,format=raw \
	-s -S

.PHONY: qemu
qemu: all
	qemu-system-i386 -m 32M -audiodev pa,id=hda -machine pcspk-audiodev=hda -rtc base=localtime \
	-boot c -drive file=../targets/UNIXV6++/c.img,if=ide,index=0,media=disk,format=raw 

prepare:
	mkdir -p ../targets/objs ../targets/img ../targets/UNIXV6++ lib/objs shell/OBJS program/objs ../tools/v6pp-fs-edit-2022/workspace/programs/bin

#no program directory
$(DIRS) : | prepare 
	$(MAKE) --directory=$@
	
build	: $(DIRS)	
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET)/kernel.exe	
	$(OBJCOPY) -O binary -j .text -j .rdata -j .data $(TARGET)/kernel.exe $(TARGET)/kernel.bin
	$(NM) $(TARGET)/kernel.exe > $(TARGET)/kernel.sym
	$(OBJDUMP) -d $(TARGET)/kernel.exe  > $(TARGET)/kernel.asm

deploy	: build

	cp -f $(TARGET)/boot.bin $(IMGTARGET)/boot.bin
	cp -f $(TARGET)/kernel.bin $(IMGTARGET)/kernel.bin
	cp -f $(TARGET)/kernel.sym  $(IMGTARGET)/kernel.sym
	cp -f $(TARGET)/kernel.asm  $(IMGTARGET)/kernel.asm

#	cd $(IMGTARGET) && partcopy boot.bin 0 200 c.img 0
#	cd $(IMGTARGET) && partcopy kernel.bin 0 13000 c.img 200

#	copy $(TARGET)/boot.bin $(MakeImageDebug)\boot.bin
#	copy $(TARGET)/kernel.bin $(MakeImageDebug)\kernel.bin
#	copy $(IMGTARGET)\c.img $(MakeImageDebug)\c.img

#	cd $(MakeImageDebug) && build.exe c.img boot.bin kernel.bin programs
#	copy $(MakeImageDebug)\$(IMG) $(UNIXV6PPTARGET)\$(IMG)

	cp -f $(TARGET)/boot.bin $(V6PP_FS_EDIT_WORKSPACE)/boot.bin
	cp -f $(TARGET)/kernel.bin $(V6PP_FS_EDIT_WORKSPACE)/kernel.bin

	$(PYTHON) $(V6PP_FS_IMAGE_BUILDER) $(V6PP_FS_EDIT_WORKSPACE) $(UNIXV6PPTARGET)/$(IMG)

clean	:	
	rm -f  $(TARGET)/*.o
	rm -f  $(TARGET)/*.exe
	rm -f  $(TARGET)/*.bin
	rm -f  $(TARGET)/*.sym
	rm -f  $(TARGET)/*.asm
	
	rm -f  $(IMGTARGET)/*.bin
	rm -f  $(IMGTARGET)/*.sym
	rm -f  $(IMGTARGET)/*.asm
	
	rm -f  $(UNIXV6PPTARGET)/c.img
