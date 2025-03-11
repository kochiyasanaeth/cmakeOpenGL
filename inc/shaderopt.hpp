#include<vector>
#include<utility> 
#include<string>

namespace myShader{
    class shaderSource
    {
        public:
        using shaderType = enum {
            vertexShader,
            fragmentShader,
            geometryShader,
            tessellationControlShader,
            tessellationEvaluationShader,
            computeShader,
            maxShader,
        };
        shaderSource();
        shaderSource(std::initializer_list<std::pair<shaderType,std::string>> _list);
        shaderSource& shadersCompileProgramLink(void);
        std::vector<int> errLogTagData(void);
        std::vector<std::string> errLogInfoData(void);
        void * data();
        ~shaderSource();
        private:
        using logType = enum {
            shader,
            program,
        };
        class _shaderProgram;
        std::unique_ptr<_shaderProgram> _sdptr;
        std::vector<std::pair<shaderType,std::string>> _shaderV;
        std::vector<int32_t> _errTag;
        std::vector<std::string> _errInfo;
        void _getErrLogInfo(logType _t);
        void _resetShadersContent(void);
        bool _checkGlErr(void);
    };
}