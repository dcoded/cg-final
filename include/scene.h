#ifndef INCLUDE_CGFINAL_SCENE_H
#define INCLUDE_CGFINAL_SCENE_H

#include <vector>
#include <algorithm>

class Scene {
public:
    Scene();
    bool Add(Polygon* polygon);
    bool Del(Polygon* polygon);
    void Draw(GLuint program);
private:
    typedef std::vector<Polygon*> ptrvec_t;

    ptrvec_t polygons_;
};

Scene::Scene() {}

bool Scene::Add(Polygon* polygon) {

    auto it = std::find(polygons_.begin(), polygons_.end(), polygon);
    
    if (it == polygons_.end()) {
        polygons_.push_back(polygon);
        return true;
    }
    return false;
}

bool Scene::Del(Polygon* polygon) {

    auto it = std::find(polygons_.begin(), polygons_.end(), polygon);

    if (it != polygons_.end()) {
        polygons_.erase(it);
        return true;
    }
    return false;
}

void Scene::Draw(GLuint program) {
    for(auto& polygon : polygons_)
    {
        polygon->Draw(program);
    }
}

#endif