#include <object3d.h>

Object3d::Object3d() :
    m_scale(1.f),
    pos(glm::vec3(0,0,0)),
    rot(glm::vec3(0,0,0)),
    ModelMatrix(glm::mat4(1.f)),
    m_buffer(0), m_VAO(0),
    selected(false)
{

}

Object3d::~Object3d()
{
    mainTex.unloadTex();
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_buffer);
    glDeleteTextures(1, &m_texid);
}

void Object3d::draw(Shader *shader)
{
    ModelMatrix = glm::mat4(1.f);
    ModelMatrix = glm::translate(ModelMatrix, pos);
    ModelMatrix = glm::rotate(ModelMatrix, rot.x, glm::vec3(1,0,0))*glm::rotate(ModelMatrix, rot.y, glm::vec3(0,1,0))*glm::rotate(ModelMatrix, rot.z, glm::vec3(0,0,1));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(m_scale, m_scale, m_scale));
    shader->setMat4("Model", ModelMatrix);

    glBindVertexArray(m_VAO);
        glActiveTexture(GL_TEXTURE0);           // 0 here  |
        glBindTexture(GL_TEXTURE_2D, m_texid);  //         | for this
        shader->setInt("mainTexture", 0);       // 0 there |
        glDrawArrays(GL_TRIANGLES, 0, vertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void Object3d::setTex(unistring fname)
{
    mainTex.loadTex(fname);

    if(!mainTex.data)
    {
        printf("Error while loading texture: %s\n", fname.c_str());
        return;
    }
}

void Object3d::update(uint draw_type)
{
    if(m_VAO == 0)
        glGenVertexArrays(1, &m_VAO);
    if(m_buffer == 0)
        glGenBuffers(1, &m_buffer);
    if(m_texid == 0)
        glGenTextures(1, &m_texid);

    GLfloat data[vertexData.size()];
    for(int i=0; i < vertexData.size(); i++)
        data[i] = vertexData.at(i);

    glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data, draw_type);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    glBindVertexArray(0);
}

void Object3d::update(unistring targetTex)
{
    if(targetTex == "none")
    {
        uchar checkImage[TEX_SIZE][TEX_SIZE][RGBAC]; // R,G,B,A = 4
        // Create purple-black checker
        int i, j;
        bool c=true;

        for (i = 0; i < TEX_SIZE; i++) {
            for (j = 0; j < TEX_SIZE; j++) {
                checkImage[i][j][0] = (uchar) c == true ? 255: 0;
                checkImage[i][j][1] = (uchar)0;
                checkImage[i][j][2] = (uchar) c == true ? 255: 0;
                checkImage[i][j][3] = (uchar) 255;

                if(j == TEX_SIZE-1)
                {
                    if(c) // if purple
                        c = true;
                    else // if black
                        c = false;
                }
                else
                    c = !c;
            }
        }
        glBindTexture(GL_TEXTURE_2D, m_texid);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_SIZE, TEX_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, &checkImage);
    }
    else
    {
        setTex(targetTex);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texid);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mainTex.width, mainTex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, mainTex.data);
    }
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Object3d::move(glm::vec3 mv)
{
    pos += mv;
    //ModelMatrix = glm::translate(ModelMatrix, pos);
}

void Object3d::rotate(glm::vec3 axis, float angle)
{
    rot += glm::normalize(axis) * glm::radians(angle);
    //ModelMatrix = glm::rotate(ModelMatrix, rot.x, glm::vec3(1,0,0))*glm::rotate(ModelMatrix, rot.y, glm::vec3(0,1,0))*glm::rotate(ModelMatrix, rot.z, glm::vec3(0,0,1));
}

void Object3d::scale(float factor)
{
    m_scale = factor;
    //ModelMatrix = glm::scale(ModelMatrix, glm::vec3(m_scale, m_scale, m_scale));
}

void Object3d::select(bool yes)
{
    selected = yes;
}

bool Object3d::isSelected()
{
    return selected;
}

uint Object3d::vertexCount()
{
    return (vertexData.size()/5);
}

void Texture::loadTex(unistring fname)
{
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int color_type, interlace_type;
    FILE *fp;

    if ((fp = fopen(fname.c_str(), "rb")) == NULL)
        return;

    /* Create and initialize the png_struct
     * with the desired error handler
     * functions.  If you want to use the
     * default stderr and longjump method,
     * you can supply NULL for the last
     * three parameters.  We also supply the
     * the compiler header file version, so
     * that we know if the application
     * was compiled with a compatible version
     * of the library.  REQUIRED
     */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                     NULL, NULL, NULL);

    if (png_ptr == NULL) {
        Logger::err("Texture", "Failed to open for reading: "+fname);
        fclose(fp);
        return;
    }

    /* Allocate/initialize the memory
     * for image information.  REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        Logger::err("Texture", "Failed to initialize memory for fileinfo: "+fname);
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return;
    }

    /* Set error handling if you are
     * using the setjmp/longjmp method
     * (this is the normal method of
     * doing things with libpng).
     * REQUIRED unless you  set up
     * your own error handlers in
     * the png_create_read_struct()
     * earlier.
     */
    if (setjmp(png_jmpbuf(png_ptr))) {
        /* Free all of the memory associated
         * with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        /* If we get here, we had a
         * problem reading the file */
        return;
    }

    /* Set up the output control if
     * you are using standard C streams */
    png_init_io(png_ptr, fp);

    /* If we have already
     * read some of the signature */
    png_set_sig_bytes(png_ptr, sig_read);

    /*
     * If you have enough memory to read
     * in the entire image at once, and
     * you need to specify only
     * transforms that can be controlled
     * with one of the PNG_TRANSFORM_*
     * bits (this presently excludes
     * dithering, filling, setting
     * background, and doing gamma
     * adjustment), then you can read the
     * entire image (including pixels)
     * into the info structure with this
     * call
     *
     * PNG_TRANSFORM_STRIP_16 |
     * PNG_TRANSFORM_PACKING  forces 8 bit
     * PNG_TRANSFORM_EXPAND forces to
     *  expand a palette into RGB
     */
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

    int bit_depth;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, NULL, NULL);

    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    data = (GLubyte*) malloc(row_bytes * height);

    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

    for (int i = 0; i < height; i++) {
        // note that png is ordered top to
        // bottom, but OpenGL expect it bottom to top
        // so the order or swapped
        memcpy(data+(row_bytes * (height-1-i)), row_pointers[i], row_bytes);
    }

    /* Clean up after the read,
     * and free any memory allocated */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    /* Close the file */
    fclose(fp);
    Logger::log("Texture", "Successfully loaded "+fname);
}
