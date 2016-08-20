


uint8_t InitThread();
void CreateThread(void (func)(char*),uint8_t indexOfThread,char* cmd);
void JoinThread(uint8_t indexOfThread);
void ProtectOn();
void ProtectOff();