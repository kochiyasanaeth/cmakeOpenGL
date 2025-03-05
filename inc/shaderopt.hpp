namespace myShader{
    class shaderErr
    {
    public:
        shaderErr() = delete;
        static void printShaderLog(GLuint shader);
        static void printProgramLog(GLuint shader);
        static bool checkGLerr();
    };
    
    class shaderLoad
    {
        public:
        shaderLoad() = delete;
        shaderLoad(std::string path);
        std::string shaderGetContent(void);
        size_t shaderGetContentSize(void);
        char *shaderGetCharContent(void);
        shaderLoad& shaderImportContent(std::string path);
        private:
        std::string _shdrContent;
    };
}