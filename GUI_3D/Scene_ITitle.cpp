#include "Scene_ITitle.h"
#include "Scene_Title.h"

namespace title {

std::unique_ptr<Scene_ITitle> getScene() {
    return std::make_unique<Scene_Title>();
}

}
