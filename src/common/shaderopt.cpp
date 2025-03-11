#include <iostream>
#include <cstdlib>
#include "glew.h"
#include "glfw3.h"
#include <gl/gl.h>
#include "shaderopt.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include <utility> 
#include <cstdint>
#include <vector>
namespace myShader {
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
    
    struct _getGlShadertype {
        GLuint operator() (shaderSource::shaderType _t) const{
            GLuint ret = GL_INVALID_ENUM;
            if(_t < shaderSource::maxShader)
            {
                GLuint gltype[] = {GL_VERTEX_SHADER,GL_FRAGMENT_SHADER,GL_GEOMETRY_SHADER,GL_TESS_CONTROL_SHADER,\
                    GL_TESS_EVALUATION_SHADER,GL_COMPUTE_SHADER};
                    ret = gltype[_t];
            }
            return ret;
        }
    };

    struct _stepShaderCompile {
        GLint operator() (char *_cstr,GLuint _sdt,GLuint &_sd) {
            GLint ret = 0;
            _sd = glCreateShader(_sdt);
            glShaderSource(_sd,1,&_cstr,NULL);
            glCompileShader(_sd);
            glGetShaderiv(_sd,GL_COMPILE_STATUS,&ret);
            printf("_sd:%d\n",_sd);
            return ret;
        }
    };

    struct _stepShaderLink {
        GLint operator() (std::vector<GLuint> _v,GLuint &_sp) {
            GLint ret = 0;
            _sp = glCreateProgram();
            for(auto i = _v.begin();i != _v.end();i++) {
                std::cout <<__LINE__ << "linker"<< *i << std::endl;
                glAttachShader(_sp,*i);
            }
            glLinkProgram(_sp);
            glGetProgramiv(_sp,GL_LINK_STATUS,&ret);
            return ret;
        }
    };
    shaderLoad::shaderLoad(std::string path) {
        std::filesystem::path _pth(path);
        if(!std::filesystem::exists(_pth)) {
            std::cout << __FILE__ << ':'  << __LINE__ << ' ' << path << "not exist" << std::endl;
        }
        std::ifstream _file(path, std::ios::in);
        if(!_file.is_open()) {
            std::cout << __FILE__ << ':'  << __LINE__ << ' ' << path << "open fail" << std::endl;
        }
        this->_shdrContent =  std::string(std::istreambuf_iterator<char>(_file), std::istreambuf_iterator<char>());
    }

    shaderLoad& shaderLoad::shaderImportContent(std::string path){
        std::filesystem::path _pth(path);
        if(!std::filesystem::exists(_pth)) {
            std::cout << __FILE__ << ':'  << __LINE__ << ' ' << path << "not exist fail" << std::endl;
        }
        std::ifstream _file(path);
        if(!_file.is_open()) {
            std::cout << __FILE__ << ':'  << __LINE__ << ' ' << path << "open fail" << std::endl;
        }
        this->_shdrContent =  std::string(std::istreambuf_iterator<char>(_file), std::istreambuf_iterator<char>());
        return *this;
    }

    std::string shaderLoad::shaderGetContent(void) {
        if(this->_shdrContent.size() != 0) {
            return this->_shdrContent;
        }
        return nullptr;
    }

    char *shaderLoad::shaderGetCharContent(void) {
        printf("output\n%s",this->_shdrContent.data());
        return this->_shdrContent.data();
    }

    size_t shaderLoad::shaderGetContentSize(void) {
        return this->_shdrContent.size();
    }

    void shaderSource::_resetShadersContent() {
        for(int i = 0;i < maxShader; i++){
            _shaderV[i].first = maxShader;
        }
    }

    class shaderSource::_shaderProgram { //pImpl
        public:
            void _pushShader(GLuint _sdr) {
                __sdshader.push_back(_sdr);
            }
            size_t _getShaderVecSize() {
                return __sdshader.size();
            }
            GLuint _getShaderVecEle(int _i) {
                return __sdshader[_i];
            }
            std::vector<GLuint> _shaderVecData(void){
                return __sdshader;
            }
            void _clearShaderVec() {
                __sdshader.clear();
            }
            void _setGlProgram(GLuint _pro) {
                this->__sdprogram = _pro;
            }
            GLuint _getGlProgram() {
                return this->__sdprogram;
            }
            void _setErrObj(GLuint n) {
                this->__errobj = n;
            }
            int _getErrObj(void) {
                return this->__errobj;
            }
            GLuint* _getSdProgramData() {
                return & (this->__sdprogram);
            }
            _shaderProgram() {};
        private:
            GLuint __errobj;
            GLuint __sdprogram;
            std::vector<GLuint> __sdshader;
    };

    shaderSource::shaderSource(std::initializer_list<std::pair<shaderType,std::string>> _list): _sdptr{std::make_unique<_shaderProgram>()},\
    _shaderV(std::vector<std::pair<shaderType,std::string>>(maxShader)) ,\
    _errTag(std::vector<int32_t>()),_errInfo(std::vector<std::string>())
    {
        _resetShadersContent();
        if(_list.size() >= maxShader) {
            static_assert("input out of range");
            return;
        }
        for(auto i = _list.begin(); i != _list.end();i++)
        {
            if(i->first < maxShader) {
                this->_shaderV[i->first].first = i->first;
                this->_shaderV[i->first].second = i->second;
            }
        }
    }
    shaderSource::~shaderSource()
    {
        for(auto i = 0;i < _sdptr->_getShaderVecSize();i++) {
            _sdptr->_getShaderVecEle(i);
            glDeleteShader(_sdptr->_getShaderVecEle(i));
        }
        glDeleteProgram(_sdptr->_getGlProgram());
    }
    shaderSource::shaderSource() : _sdptr{std::make_unique<_shaderProgram>()} {
        _resetShadersContent();
    }

    void* shaderSource::data() {
        return static_cast<void *>(this->_sdptr->_getSdProgramData());
    }
    bool shaderSource::_checkGlErr(void){
        bool iserr = false;
        GLenum glErr = glGetError();
        while(glErr != GL_NO_ERROR) {
            iserr = true;
            glErr = glGetError();
            std::cout << "err" << glErr << std::endl;
            _errTag.push_back(glErr);
        }
        return iserr;
    }
    
    void shaderSource::_getErrLogInfo(logType _t) {
        int lenth;
        int chWritn = 0;
        char *log = NULL;
        std::string retstr;
        glGetShaderiv(_sdptr->_getErrObj(), GL_INFO_LOG_LENGTH,&lenth);
        if(lenth > 0) {
            log = new char[lenth];
            if(_t == shaderSource::shader) {
            glGetShaderInfoLog(_sdptr->_getErrObj(),lenth,&chWritn,log);
            }
            else if(_t == shaderSource::program) {
                glGetProgramInfoLog(_sdptr->_getErrObj(),lenth,&chWritn,log);
            }
            //printf("%u:%s",__LINE__,log);
            retstr = std::string(log);
            _errInfo.push_back(retstr);
            std::cout << _errInfo.size() << std::endl;
            delete log;
        }
    }

    std::vector<int> shaderSource::errLogTagData(void) {
        return this->_errTag;
    }

    std::vector<std::string> shaderSource::errLogInfoData(void) {
        return this->_errInfo;
    }
    shaderSource& shaderSource::shadersCompileProgramLink(void) {
        int32_t _errsdnum;
        GLuint _sd;
        GLuint _sdProgram;
        GLuint _sdComplied;
        GLuint _sdLinked;
        std::string _temp; 
        for(int i = 0;i < maxShader;i++) {
            if(_shaderV[i].first != maxShader) {
                std::cout <<  i << ':' << _shaderV[i].first << std::endl;
                shaderLoad _sld(_shaderV[i].second);
                if(_sld.shaderGetContentSize() == 0) break;
                char* _sdsource = _sld.shaderGetCharContent();
                _sdComplied = _stepShaderCompile()(_sdsource,_getGlShadertype()(_shaderV[i].first),_sd);
                if(_sdComplied != 1) {
                    _errsdnum = i;
                    break;
                }
                _sdptr->_pushShader(_sd);
            }
        }
        _checkGlErr();
        if(_sdComplied != 1) {
            _sdptr->_setErrObj(_sd);
            _getErrLogInfo(shader);
            return *this;
        }
        _sdLinked = _stepShaderLink()(_sdptr->_shaderVecData(),_sdProgram);
        _sdptr->_setGlProgram(_sdProgram);
        _checkGlErr();
        if(_sdComplied != 1) {
            _getErrLogInfo(program);
            return *this;
        }
        return *this;
    }
}