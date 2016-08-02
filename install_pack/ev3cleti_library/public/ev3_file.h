#ifndef EV3_FILE_H
#define EV3_FILE_H

short File_save(char* fileName, char* buffer, short append);
char* File_load(char* fileName);
char* File_load_string(char* fileName);

#endif //!EV3_FILE_H