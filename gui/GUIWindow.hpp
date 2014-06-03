#ifndef GUIWINDOW_HPP
#define GUIWINDOW_HPP

#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QComboBox>
#include "impl/SceneWindow.hpp"

namespace gui {
    class GUIWindow : public QWidget
    {
        Q_OBJECT

        public :
            GUIWindow(QWidget *parent = 0);
            ~GUIWindow();

           void setOpenGLWindow(SceneWindow*);

        private slots:
            void selectModel();
            void loadModel(int i);

        private :
            QPushButton *m_LoadModelButton;
            QPushButton *m_SelectModelButton;

            QLabel  *m_ModelPath;

            QComboBox *m_SelectAnimationBox;

            SceneWindow *m_OpenGLWindow;
    };
}

#endif
