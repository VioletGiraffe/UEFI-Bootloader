#include <UEFI/UEFI.h>

EFI_STATUS EFIAPI efi_main(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
	auto conOut = SystemTable->ConOut;

	conOut->ClearScreen(conOut);

	conOut->SetCursorPosition(conOut, 0, 0);
	conOut->OutputString(conOut, u"Press any key to type.");

	conOut->SetCursorPosition(conOut, 0, 1);
	conOut->OutputString(conOut, u"Or press F12 to quit this application.");

	conOut->SetCursorPosition(conOut, 0, 3);
	conOut->EnableCursor(conOut, TRUE);

	auto conIn = SystemTable->ConIn;

	for (;;)
	{
		UINTN index = 0;
		auto keyEvent = conIn->WaitForKey;
		SystemTable->BootServices->WaitForEvent(1, &keyEvent, &index);

		EFI_INPUT_KEY key{ 0, 0 };
		if (conIn->ReadKeyStroke(conIn, &key) != EFI_SUCCESS)
			continue;

		if (key.UnicodeChar == 0)
		{
			// Non-printable character
			switch (key.ScanCode)
			{
			case Cursor_Up:
				break;
			case Cursor_Down:
				break;
			case Function12:
				return EFI_SUCCESS;
			default:
				break;
			}

			continue;
		}

		CHAR16 text[2] = { key.UnicodeChar, L'\0' };
		conOut->OutputString(conOut, text);
	}

	return EFI_SUCCESS;
}