#ifndef INCLUDE_CGFINAL_SHADER_H
#define INCLUDE_CGFINAL_SHADER_H

#include <fstream>
#include <cassert>
#include <map>



class Shader {
public:
    explicit Shader(GLuint program,
                    const std::string vertexfile, const std::string fragfile);
private:
    GLuint program_;

    std::map<
        std::string,        // filename or key
        std::vector<GLchar> // file data
    > resources_;

    void LoadResource(const std::string key, const std::string filename);
    void Log(GLuint id, std::string resource);
};


Shader::Shader(GLuint program,
               const std::string vertfile, const std::string fragfile)
    : program_(program)
{
    GLuint vs = glCreateShader( GL_VERTEX_SHADER );
    GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );

    LoadResource(vertfile, vertfile);
    LoadResource(fragfile, fragfile);

    GLchar* vsrc = &resources_[vertfile][0];
    GLchar* fsrc = &resources_[fragfile][0];

    // Attach the source to the shaders
    glShaderSource(vs, 1, &vsrc, NULL);
    glShaderSource(fs, 1, &fsrc, NULL);

    GLint flag;
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &flag);
    Log(vs, vertfile);
    if (flag == GL_FALSE) {
        std::cout << "[ERROR] " << vertfile << " could not be compiled" << std::endl;
        return;
    }

    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &flag);
    Log(fs, fragfile);
    if (flag == GL_FALSE) {
        std::cout << "[ERROR] " << fragfile << " could not be compiled" << std::endl;
        return;
    }

    glAttachShader(program_, vs);
    glAttachShader(program_, fs);

    // Report any message log information
    Log(program_, "Program");

    // Link the program, and print any message log information
    glLinkProgram(program_);
    glGetProgramiv(program_, GL_LINK_STATUS, &flag);
    Log(program_, "Program");
    if (flag == GL_FALSE) {
        std::cout << "[ERROR] Error linking shader" << std::endl;
        return;
    }
}

void Shader::LoadResource(const std::string key, const std::string filename)
{
    std::ifstream ifs(filename);

    // check that file is readable
    if (!ifs.good())
    {
        std::cout << "[ERROR] " << " Could not read resource: " << filename << std::endl;
        assert (ifs.good());
    }

    // prevent re-reading or overwriting of resource data
    assert (resources_.find(key) == resources_.end());

    ifs >> std::noskipws;
    std::copy
    (
        std::istream_iterator<GLchar>(ifs),
        std::istream_iterator<GLchar>(),
        std::back_inserter(resources_[key])
    );

    resources_[key].push_back('\0');
}

void Shader::Log(GLuint shader, std::string resource)
{
    GLint length;
    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &length );

    char msg[length];

    GLsizei nchars = 0;
    glGetShaderInfoLog( shader, length, &nchars, msg);
    if (length > 0 && msg[0] != '\0') {
        std::cout << "[ERROR] " << resource << ": " << msg << std::endl;

        if(resources_.find(resource) != resources_.end()) {

            std::string file(resources_[resource].begin(),
                             resources_[resource].end());
            
            std::cout << "----------------------------------------"
                      << "----------------------------------------" << "\n"
                      << file                                       << "\n"
                      << "----------------------------------------"
                      << "----------------------------------------" << "\n";
        }
    }
}

#endif