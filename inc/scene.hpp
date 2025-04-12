#include<vector>
#include<string>
#include<functional>
#include<utility> 
#include<unordered_map>
namespace myScene{

class sceneElement {
public:
    sceneElement() = delete;
    sceneElement(std::string name);
    std::string getName(void);
    void setName(std::string name);
private:
    std::string _name;
};


class scene {
public:
    scene() = delete;
    scene(std::string sceneName,uint32_t Wwidth,uint32_t Wheight);
    bool check();
    void createW();
    bool shouldCloseW();
    void setDisplayFunction(std::function<void()>);
    void importElementData(sceneElement element);
    sceneElement getElementData(std::string elename);
    size_t getElementSize();
    void removeElement(std::string);
    void sceneDisplay();
    ~scene();
private:
    uint32_t _Wwidth;
    uint32_t _Wheight;
    class _sceWindow;
    std::unique_ptr<_sceWindow> _Wptr;
    std::string _sceneName;
    std::unordered_map<std::string,sceneElement> _elementslist;
    std::function<void()> _display;
};
}