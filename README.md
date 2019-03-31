# UEFI-Bootloader
A simple UEFI bootloader written in C++17 that does not need any third-party support code like Tianocore EDK or gnu-efi; only needs a handful EFI standard definitions that are provided by a sub-module.


At the moment, compilation with Microsoft Visual Studio is supported (tested with MSVC 2017). Compilation with clang is also possible.