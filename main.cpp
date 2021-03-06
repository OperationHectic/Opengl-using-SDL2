    #include <string>
    #include <GL/glew.h>
    #include <SDL2/SDL.h>
    #ifdef _MSC_VER
      #pragma comment(lib, "opengl32.lib")
      #pragma comment(lib, "glu32.lib")
    #endif
    void render();
    int main(int argc, char *argv[])
    {
      (void)(argc);
      (void)(argv);
      bool running = true;
      SDL_Window* window;
      SDL_GLContext context;
      SDL_Event event;
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
      if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
      {
        std::string msg = "SDL failed to initialize: ";
        msg.append(SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Init Failed", msg.c_str(), nullptr);
        return 0;
      }
      else
      {
        window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if(window == nullptr)
        {
          std::string msg = "Window could not be created: ";
          msg.append(SDL_GetError());
          SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Init Failed", msg.c_str(), nullptr);
          return 0;
        }
        else
        {
          context = SDL_GL_CreateContext(window);
          if(context == nullptr)
          {
            // Display error message
            std::string msg = "OpenGL context could not be created: ";
            msg.append(SDL_GetError());
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Init Failed", msg.c_str(), nullptr);
            return 0;
          }
          else
          {
            glewInit();
          }
        }
      }
      while (running)
      {
        while(SDL_PollEvent(&event) != 0)
        {
          if(event.type == SDL_QUIT)
          {
            running = false;
          }
        }
        glClearColor(0.2f, 0.2f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        render();
        SDL_GL_SwapWindow(window);
      }
      SDL_GL_DeleteContext(context);
      SDL_DestroyWindow(window);
      window = nullptr;
      SDL_Quit();
      return 0;
    }
    void render()
    {
      GLfloat vertices[] =
      {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
      };
      static const char *vertexShader =
      "#version 330 core\n"
      "layout(location = 0) in vec2 posAttr;\n"
      "void main() {\n"
      "gl_Position = vec4(posAttr, 0.0, 1.0); }";
      static const char *fragmentShader =
      "#version 330 core\n"
      "out vec4 col;\n"
      "void main() {\n"
      "col = vec4(1.0, 0.0, 0.0, 1.0); }";
      GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertexShaderID, 1, &vertexShader, nullptr);
      glCompileShader(vertexShaderID);
      GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragmentShaderID, 1, &fragmentShader , nullptr);
      glCompileShader(fragmentShaderID);
      GLuint shaderProgramID = glCreateProgram();
      glAttachShader(shaderProgramID, vertexShaderID);
      glAttachShader(shaderProgramID, fragmentShaderID);
      glLinkProgram(shaderProgramID);
      glDetachShader(shaderProgramID, vertexShaderID);
      glDetachShader(shaderProgramID, fragmentShaderID);
      glDeleteShader(vertexShaderID);
      glDeleteShader(fragmentShaderID);
      GLuint vertexBufferID;
      glGenBuffers(1, &vertexBufferID);
      glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
      glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
      glUseProgram(shaderProgramID);
      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glUseProgram(NULL);
      glDisableVertexAttribArray(0);
    }
