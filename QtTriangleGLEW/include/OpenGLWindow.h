#ifndef OPENGLWINDOW_H_
#define OPENGLWINDOW_H_

#include <QOpenGLWindow>


class OpenGLWindow : public QOpenGLWindow
{
  // need to tell Qt to run the MOC
  Q_OBJECT
  public:
    //---------------------------------------------------------------------------------------
    /// @brief ctor for OpenGL window must set the surface type to OpenGL
    /// @param [in] parent the parent window to the class
    //---------------------------------------------------------------------------------------
    explicit OpenGLWindow();
    //---------------------------------------------------------------------------------------
    /// @brief dtor, remember to remove the device once finished
    //---------------------------------------------------------------------------------------

    virtual ~OpenGLWindow();
    //---------------------------------------------------------------------------------------
    /// @brief render method called every update
    //---------------------------------------------------------------------------------------
    void paintGL()  override;
    //---------------------------------------------------------------------------------------
    /// @brief pure virtual initialize method we override in our base class to do our drawing
    /// this is only called one time, just after we have a valid GL context use this to init any global GL elements
    //---------------------------------------------------------------------------------------
   void initializeGL() override;
   //---------------------------------------------------------------------------------------
   /// @brief this is called everytime we want to draw the scene
   //---------------------------------------------------------------------------------------
   void resizeGL(int _w, int _h);

  private:
    //---------------------------------------------------------------------------------------
    /// @brief process key events
    //---------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *) override;
    GLuint m_vaoID;
    GLuint m_shaderID;
     
    int m_width;
    int m_height;

};

#endif
