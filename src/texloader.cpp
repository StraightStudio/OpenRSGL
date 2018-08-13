#include <texloader.h>

TexLoader::TexLoader()
{

}

TexLoader::~TexLoader()
{
    for(pair<unistring, GLuint> tex : m_texids)
        glDeleteTextures(1, &tex.second);
}

void TexLoader::loadTexture(unistring tf, unistring alias)
{
    if(m_texids[alias] == 0)
        glGenTextures(1, &m_texids[alias]);

    if(tf == "none")
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
        glBindTexture(GL_TEXTURE_2D, m_texids[alias]);
        glActiveTexture(GL_TEXTURE0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_SIZE, TEX_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, &checkImage);
    }
    else
    {
        tmptex.loadTex(tf);
        glBindTexture(GL_TEXTURE_2D, m_texids[alias]);
		glActiveTexture(GL_TEXTURE0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmptex.width, tmptex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmptex.data);
        tmptex.unloadTex();
    }
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1);

    float *maxA = (float*)calloc(1, sizeof(float));
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, maxA);
    float a_amount = min(4.f, *maxA);
    if(maxA != NULL)
    	free(maxA);
    glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, a_amount);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
}

void TexLoader::loadCubemap(vector<unistring> cbf, int id)
{
    m_cubemaps.resize(id+1, 0);
    if(m_cubemaps[id] == 0)
        glGenTextures(1, &m_cubemaps[id]);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemaps[id]);
    for(int i=0; i < 6; i++)
    {
        tmptex.loadTex(cbf[i], true);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, tmptex.width, tmptex.height,
                     0, GL_RGB, GL_UNSIGNED_BYTE, tmptex.data);
        tmptex.unloadTex();
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void TexLoader::getTextureInfo(unistring alias, GLuint *texid)
{
    if(m_texids.find(alias) == m_texids.end())
    {
        fprintf(stdout, "TInfo won't be retrieved.\n");
        fflush(stdout);
        return;
    }
    *texid = m_texids.at(alias);
}

GLuint TexLoader::cubemap(int id)
{
    if(id < m_cubemaps.size())
        return m_cubemaps[id];
    else
        return 0;
}



void Texture::loadTex(unistring fname, bool cubemap)
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

    if(cubemap)
    {
        for(int i=0; i < height; i++)
            memcpy(data+(row_bytes*i), row_pointers[i], row_bytes);
    }
    else
    {
        for (int i = 0; i < height; i++)
        {
            // note that png is ordered top to
            // bottom, but OpenGL expect it bottom to top
            // so the order or swapped
            memcpy(data+(row_bytes * (height-1-i)), row_pointers[i], row_bytes);
        }
    }

    /* Clean up after the read,
     * and free any memory allocated */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    /* Close the file */
    fclose(fp);
    Logger::log("Texture", "Successfully loaded "+fname);
}

GLuint TexLoader::loadTexture(unistring tf)
{
	GLuint texid;
	glGenTextures(1, &texid);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texid); // Bind texture
	if(tf == "none")
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEX_SIZE, TEX_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, &checkImage);
	}
	else
	{
		tmptex.loadTex(tf);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmptex.width, tmptex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmptex.data);
		tmptex.unloadTex();
	}
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1);
	float *maxA = (float*)calloc(1, sizeof(float));
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, maxA);
	float a_amount = min(4.f, *maxA);
	if(maxA != NULL)
		free(maxA);
	glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, a_amount);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
	return texid;
}
