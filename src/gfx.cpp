#include <UEFI/UEFI.h>

#define EFI_ERROR(status) ((status) != EFI_SUCCESS)

void printInt(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *conOut, int value) {
	CHAR16 out[32];
	CHAR16 *ptr = out;
	if (value == 0) {
		conOut->OutputString(conOut, u"0"_u);
		return;
	}

	ptr += 31;
	*--ptr = 0;
	int tmp = value;// >= 0 ? value : -value; 

	while (tmp) {
		*--ptr = '0' + tmp % 10;
		tmp /= 10;
	}
	if (value < 0) *--ptr = '-';
	conOut->OutputString(conOut, ptr);
}

/**
 * efi_main - The entry point for the EFI application
 * @image: firmware-allocated handle that identifies the image
 * @SystemTable: EFI system table
 */
EFI_STATUS
efi_main(EFI_HANDLE /*image*/, EFI_SYSTEM_TABLE *systemTable)
{
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *conOut = systemTable->ConOut;

	conOut->ClearScreen(conOut);
	conOut->SetCursorPosition(conOut, 0, 0);

	EFI_GUID gfxProtocolGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *graphicsProtocol = nullptr;
	EFI_STATUS status = systemTable->BootServices->LocateProtocol(&gfxProtocolGuid, NULL,
		(void**)&graphicsProtocol);

	if (EFI_ERROR(status) || graphicsProtocol == NULL) {
		conOut->OutputString(conOut, u"Failed to init gfx!\r\n"_u);
		return status;
	}

	//Switch to current mode so gfx is started.
	status = graphicsProtocol->SetMode(graphicsProtocol, graphicsProtocol->Mode->Mode);
	if (EFI_ERROR(status)) {
		conOut->OutputString(conOut, u"Failed to set default mode!\r\n"_u);
		return status;
	}

	conOut->OutputString(conOut, u"Current mode: "_u);
	printInt(conOut, graphicsProtocol->Mode->Mode);
	conOut->OutputString(conOut, u"\r\n"_u);

	for (UINT32 i = 0, modeCount = graphicsProtocol->Mode->MaxMode; i < modeCount; i++)
	{
		conOut->OutputString(conOut, u"\r\n"_u);
		printInt(conOut, i);
		conOut->OutputString(conOut, u": "_u);
		EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
		UINTN SizeOfInfo;
		status = graphicsProtocol->QueryMode(graphicsProtocol, i, &SizeOfInfo, &info);

		if (EFI_ERROR(status)) {
			conOut->OutputString(conOut, u" Failure to query mode: "_u);
			printInt(conOut, (int)status);
			continue;
		}
		printInt(conOut, info->HorizontalResolution);
		conOut->OutputString(conOut, u" x "_u);
		printInt(conOut, info->VerticalResolution);

		switch (info->PixelFormat) {
		case PixelRedGreenBlueReserved8BitPerColor:
			conOut->OutputString(conOut, u" RGB(R)"_u);
			break;
		case PixelBlueGreenRedReserved8BitPerColor:
			conOut->OutputString(conOut, u" BGR(R)"_u);
			break;
		case PixelBitMask:
			conOut->OutputString(conOut, u" BitMask "_u);
			printInt(conOut, info->PixelInformation.RedMask);
			conOut->OutputString(conOut, u"R "_u);
			printInt(conOut, info->PixelInformation.GreenMask);
			conOut->OutputString(conOut, u"G "_u);
			printInt(conOut, info->PixelInformation.BlueMask);
			conOut->OutputString(conOut, u"B "_u);
			printInt(conOut, info->PixelInformation.ReservedMask);
			conOut->OutputString(conOut, u"Reserved "_u);
			break;
		case PixelBltOnly:
			conOut->OutputString(conOut, u" (blt only)"_u);
			break;
		default:
			conOut->OutputString(conOut, u" (Invalid pixel format)"_u);
			break;
		}
		conOut->OutputString(conOut, u" Pixel per scanline: "_u);
		printInt(conOut, info->PixelsPerScanLine);
	}

	EFI_EVENT event = systemTable->ConIn->WaitForKey;
	UINTN index = 0;
	systemTable->BootServices->WaitForEvent(1, &event, &index);
	return EFI_SUCCESS;
}