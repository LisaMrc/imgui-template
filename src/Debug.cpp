#include "../include/Debug.hpp"
#include <imgui.h>
#include <tiny_obj_loader.h>
#include <iostream>

void display3DObj()
{
    ImGui::Begin("3D Display");
    std::string                      inputfile = "../Objects/Pawn.obj";
    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;
    std::string                      warn;
    std::string                      err;
    bool                             ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());
    if (!ret)
    {
        std::cout << "Failed to load OBJ: " << err << std::endl;
        return;
    }
    ImGui::End();
}