void setup() {}

void loop() {
}

#ifdef _WIN32
int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* pCmdLine,
                      int nCmdShow) {
#else
int main(int argc, char** argv) {
#endif
    return 0;
}