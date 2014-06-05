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

            void selectLight();
            void applyLight();

        private :
            QPushButton *m_LoadModelButton;
            QPushButton *m_SelectModelButton;

            QPushButton *m_LoadLightButton;
            QPushButton *m_ApplyLightButton;

            QLabel  *m_ModelPath;
            QLabel  *m_LightPath;

            QComboBox *m_SelectAnimationBox;

            SceneWindow *m_OpenGLWindow;

            std::string m_LightFilePath;
    };
}

#endif
