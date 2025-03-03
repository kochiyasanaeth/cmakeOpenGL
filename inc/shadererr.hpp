  
namespace myShader{
    class shaderErr
    {
    public:
        shaderErr() = delete;
        static void printShaderLog(GLuint shader);
        static void printProgramLog(GLuint shader);
        static bool checkGLerr();
    };
}