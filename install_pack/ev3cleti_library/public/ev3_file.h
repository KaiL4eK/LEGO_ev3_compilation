#ifndef EV3_FILE_H
#define EV3_FILE_H

short File_save(char* fileName, char* buffer, short append);
char* File_load(char* fileName);
char* File_load_string(char* fileName, int indexOfString);
void File_Clear(char* fileName);
short File_S_Compare(char* fileName0, char* fileName1);

#endif //!EV3_FILE_H