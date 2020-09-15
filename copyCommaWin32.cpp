// copyCommaWin32.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	int oc = OpenClipboard(NULL);
	if (oc) {
		HANDLE ch = GetClipboardData(CF_TEXT);
		if (ch) {
			const char* pc = (const char*)ch;

			// determine the length of the string
			unsigned int len = strlen(pc);

			// get a pointer to the end of it
			char* end = const_cast<char *>(pc + len);

			// trim trailing spaces from the string
			while(--end > pc && isspace(*end)) {
				*end = 0;
			}

			// determine the Length of the Commandline Args
			unsigned int argsLen = strlen(lpCmdLine);

			void *dstbuf = NULL;
			unsigned int dstlen = len;
			if (argsLen <= 2) {
				// if the argsLen <= 2, no additional Mem needs to be allocated
				// and we can do a 1 by 1 exchange of the delimiters.
				dstbuf = (void*)pc;
			} else {
				// we will need more Mem, let's try with about 20% more...
				dstlen = (unsigned int)(dstlen * 1.2);
				dstbuf = malloc((unsigned int)dstlen);
			}

			char* src = const_cast<char *>(pc);
			char* dst = (char*)dstbuf;
			while (src <= end) {
				if (*src != 10 && *src != 13) {
					while (dst + argsLen + 1 >= (char*)dstbuf + dstlen) {
						unsigned int tmplen = (unsigned int)(dstlen * 1.2);
						tmplen = tmplen == dstlen ? 5 : tmplen;
						char* tmp = (char*)malloc(tmplen);
						memcpy(tmp, dstbuf, dstlen);
						dst = (char*)tmp + (dst - (char*)dstbuf);
						if (dstbuf != (void*)pc) free(dstbuf);
						dstbuf = tmp;
						dstlen = tmplen;
					}
					*dst++ = *src++;
				} else {
					while (dst + argsLen + 1 >= (char*)dstbuf + dstlen) {
						unsigned int tmplen = (unsigned int)(dstlen * 1.2);
						tmplen = tmplen == dstlen ? 5 : tmplen;
						char* tmp = (char*)malloc(tmplen);
						memcpy(tmp, dstbuf, dstlen);
						dst = (char*)tmp + (dst - (char*)dstbuf);
						if (dstbuf != (void*)pc) free(dstbuf);
						dstbuf = tmp;
						dstlen = tmplen;
					}
					memcpy(dst, lpCmdLine, argsLen);
					dst += argsLen;

					// Jump forward one or two characters, depending on if
					// the next character is a Newline Character, too.
					char* next = src + 1;
					if (*next == 10 || *next == 13) {
						src += 2;
					} else {
						src++;
					}
				}
			}

			*dst = '\0';
			
//			while(--end > pc) {
//				if (*end == 13) {
//					*end = ',';
//				}
//				if (*end == 10) {
//					*end = ' ';
//				}
//			}

			EmptyClipboard(); 
			unsigned int siz = strlen((const char*)dstbuf);
			
			// Allocate a global memory object for the text. 
 			void* hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (siz + 1) * sizeof(char)); 
			if (hglbCopy == NULL) 
			{ 
				CloseClipboard(); 
				return FALSE; 
			} 
 
			// Lock the handle and copy the text to the buffer. 
 			char* lptstrCopy = (char*) GlobalLock(hglbCopy); 
			memcpy(lptstrCopy, dstbuf, siz * sizeof(char)); 
			lptstrCopy[siz] = (char) 0;    // null character 
			GlobalUnlock(hglbCopy); 

			// free dstbuf if allocated
			if (dstbuf != (void*)pc) {
				free(dstbuf);
				dstbuf = NULL;
			}
 
			// Place the handle on the clipboard. 
 			SetClipboardData(CF_TEXT, lptstrCopy); 
		}

		CloseClipboard();
	}
	return 0;
}



