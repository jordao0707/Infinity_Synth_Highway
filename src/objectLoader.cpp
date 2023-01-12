// Jordão Rodrigues Dantas - 403686
// Ciência da Computação
#include "./objectLoader.h"

// CORRES PADRÕES DOS OBJETOS
std::vector<vec3> colors = {vec3(1.0, 0.0, 1.0), vec3(1.0, 1.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(1.0, 0.4, 0.0)};

std::vector<material> materials = {
    // Obsidian
    material(vec3(0.05375f, 0.05f, 0.06625f), vec3(0.18275f, 0.17f, 0.22525f), vec3(0.332741f, 0.328634f, 0.346435f), 38.4f),
    // Rubi
    material(vec3(0.1745f, 0.01175f, 0.01175f), vec3(0.61424f, 0.04136f, 0.04136f), vec3(0.727811f, 0.626959f, 0.626959f), 76.8f),
    // Cyan rubber
    material(vec3(0.0f, 0.05f, 0.05f), vec3(0.4f, 0.5f, 0.5f), vec3(0.04f, 0.7f, 0.7f), 10.0f),
    // Bronze
    material(vec3(0.2125f, 0.1275f, 0.054f), vec3(0.714f, 0.4284f, 0.18144f), vec3(0.393548f, 0.271906f, 0.166721f), 25.6f),

};
vec3 lighting(vec3 light, vec3 cam, vec3 normal, int type)
{
    vec3 l = normalize(light - normal);
    vec3 v = normalize(cam - normal);
    vec3 r = (2 * dot(l, normal) * normal) - l;
    vec3 ambient = materials[type].ambient;
    vec3 diffuse = max(dot(l, normal), 0.0f) * materials[type].diffuse;
    vec3 specular = (float)pow(max(dot(r, v), 0.0f), materials[type].shine) * materials[type].specular;
    vec3 color = ambient / l + diffuse / l + specular / l;
    return color;
}

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
    sscanf(s.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &t1, &f,  &v2, &t2, &f,   &v3, &t3, &f);
   
    face face(v1, v2, v3, 0, f);
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

void ObjectLoader::drawObject(GLuint id, vec3 pos, int color_id, vec3 light, vec3 cam)
{
    glNewList(id, GL_COMPILE);
    glPushMatrix();

    glTranslated(pos.x, pos.y, pos.z);

    for (int i = 0; i < faces.size(); i++)
    {
        unsigned index = faces[i].index - 1;
        int v1 = faces[i].vertex[0] - 1;
        int v2 = faces[i].vertex[1] - 1;
        int v3 = faces[i].vertex[2] - 1;

        vec3 normal = normals[v1] + normals[v2] + normals[v3];

        vec3 color = vec3(0, 0, 0);
        glBegin(GL_TRIANGLES);

        color = lighting(light, cam, normal, 1);
        glColor3fv(&color.x);
        glVertex3fv(&vertices[v1].x);

        color = lighting(light, cam, normal, 1);
        glColor3fv(&color.x);
        glVertex3fv(&vertices[v2].x);

        color = lighting(light, cam, normal, 1);
        glColor3fv(&color.x);
        glVertex3fv(&vertices[v3].x);

        glEnd();
    }
    glPopMatrix();
    glEndList();
}

float max(float a, float b)
{
    return a > b ? a : b;
}