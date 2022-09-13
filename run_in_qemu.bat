copy /Y /B Release\bootx64.efi BOOT\EFI\BOOT\
"C:\Program Files\qemu\qemu-system-x86_64.exe" -cpu qemu64 -net none ^
-drive if=pflash,format=raw,unit=0,file=E:\Development\Tools\UEFI\OVMF\OVMF_CODE.fd,readonly=on ^
-drive file=fat:rw:E:\Development\Projects\Personal\UEFI-Bootloader\BOOT,format=raw