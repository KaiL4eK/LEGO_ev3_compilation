
	uint8_t thread_Init();
	void thread_Create(void (func)(char*),uint8_t indexOfThread,char* cmd);
	void thread_Join(uint8_t indexOfThread);
	void thread_Detach(uint8_t indexOfThread);
	void thread_ProtectOn();
	void thread_ProtectOff();