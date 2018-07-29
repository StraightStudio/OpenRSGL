#include <shader.h>

void Shader::loadShader(unistring fvname, unistring ffname)
{
    if(ID != 0)
        return;
    GLuint vsh=0, fsh=0;

    unistring tvs, tfs; // TemporaryString (Vertex, Fragment)

    tvs.clear();
    ifstream vshader(fvname);
    getline(vshader, tvs, (char)vshader.eof());
    vshader.close();

    tfs.clear();
    ifstream fshader(ffname);
    getline(fshader, tfs, (char)fshader.eof());
    fshader.close();

    vsh = glCreateShader(GL_VERTEX_SHADER);
    fsh = glCreateShader(GL_FRAGMENT_SHADER);

    //

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Компилируем Вершинный шейдер
    printf("Compiling shader: %s\n", fvname.c_str());
    char const * vsh_src = tvs.c_str();
    glShaderSource(vsh, 1, &vsh_src, NULL);
    glCompileShader(vsh);


    // Выполняем проверку Вершинного шейдера
    glGetShaderiv(vsh, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(vsh, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(vsh, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
    }


    // Компилируем Фрагментный шейдер
    printf("Compiling shader: %s\n", ffname.c_str());
    char const * fsh_src = tfs.c_str();
    glShaderSource(fsh, 1, &fsh_src, NULL);
    glCompileShader(fsh);


    // Проверяем Фрагментный шейдер
    glGetShaderiv(fsh, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(fsh, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(fsh, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
    }


    // Создаем шейдерную программу и привязываем шейдеры к ней
    fprintf(stdout, "Connecting shaders...\n");
    ID = glCreateProgram();
    glAttachShader(ID, vsh);
    glAttachShader(ID, fsh);
    glLinkProgram(ID);


    // Проверяем шейдерную программу
    glGetProgramiv(ID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
    }

    fflush(stdout);
    //
    glDeleteShader(vsh);
    glDeleteShader(fsh);
}

Shader::Shader() :
    ID(0)
{

}

void Shader::use()
{
    if(ID == 0)
        return;
    glUseProgram(ID);
}

void Shader::setMat4(unistring id, glm::mat4 &matrix)
{
    if(ID == 0 || id.empty())
        return;
    glUniformMatrix4fv(glGetUniformLocation(ID, id.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setInt(unistring id, int value)
{
    glUniform1i(glGetUniformLocation(ID, id.c_str()), value);
}

void Shader::setFloat(unistring id, float value)
{
    glUniform1f(glGetUniformLocation(ID, id.c_str()), value);
}
