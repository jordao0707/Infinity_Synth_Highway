#include "objectLoader.h"
#include <vector>

std::vector<vec3> vertices;
std::vector<vec3> normals;
std::vector<face> faces;
vec3 position;
std::vector<vec3> colors = {vec3(1.0, 0.0, 1.0), vec3(1.0, 1.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(1.0, 0.4, 0.0)};
vec3 ObjectLoader::getVertex(std::string s)
{
    float x, y, z;
    sscanf(s.c_str(), "v %f %f %f", &x, &y, &z);
    vec3 vertice(x, y, z);
    return vertice;
}
vec3 ObjectLoader::getNormal(std::string s)
{

    float x, y, z;
    sscanf(s.c_str(), "vn %f %f %f", &x, &y, &z);
    vec3 normal(x, y, z);
    return normal;
}
face ObjectLoader::getFace(std::string s)
{

    int v1, v2, v3, v4, f;
    int t1, t2, t3, t4;
    sscanf(s.c_str(), "f %d//%d %d//%d %d//%d %d//%d", &v1, &f, &v2, &f, &v3, &f, &v4, &f);
    face face(v1, v2, v3, v4, f);
    return face;
}

void ObjectLoader::loadObject(const char *file_path)
{
    std::fstream arq(file_path);
    std::string line = "";
    if (vertices.size() > 0)
        vertices.clear();
    if (normals.size() > 0)
        normals.clear();
    if (faces.size() > 0)
        faces.clear();

    if (!arq.is_open())
    {
        std::cout << "Não foi possivel abrir o arquivo " << file_path << "\n";
    }

    while (getline(arq, line))
    {
        if (line.find("v ") != std::string::npos)
        {
            vec3 temp_vertex = getVertex(line);
            vertices.push_back(temp_vertex);
        }
        if (line.find("vn ") != std::string::npos)
        {
            vec3 temp_normal = getNormal(line);
            normals.push_back(temp_normal);
        }
        if (line.find("f ") != std::string::npos)
        {
            face temp_face = getFace(line);
            faces.push_back(temp_face);
        }
    }
  }

void ObjectLoader::drawObject(unsigned &id, vec3 pos, int color_id)
{
    glNewList(id, GL_COMPILE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glPushMatrix();

    glTranslated(pos.x, pos.y, pos.z);

    for (int i = 0; i < faces.size(); i++)
    {
        unsigned index = faces[i].index - 1;
        int v1 = faces[i].vertex[0] - 1;
        int v2 = faces[i].vertex[1] - 1;
        int v3 = faces[i].vertex[2] - 1;

        glBegin(GL_LINES);
        glColor3fv(&colors[color_id].x);
        glVertex3fv(&vertices[v1].x);
        glVertex3fv(&vertices[v2].x);

        glVertex3fv(&vertices[v2].x);
        glVertex3fv(&vertices[v3].x);

        glVertex3fv(&vertices[v3].x);
        glVertex3fv(&vertices[v1].x);

        glEnd();
    }
    glDisable(GL_CULL_FACE);
    glPopMatrix();
    glEndList();
}