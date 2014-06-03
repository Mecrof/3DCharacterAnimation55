#include "GUIWindow.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>

gui::GUIWindow::GUIWindow(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle(QString("Menu"));
    this->setFixedSize(QSize(300, 100));

    QVBoxLayout *verticalLayout = new QVBoxLayout;
    QHBoxLayout *horizontalLayout1 = new QHBoxLayout;

    this->m_ModelPath = new QLabel("Model : ");
    this->m_LoadModelButton = new QPushButton("Charger un model");
    this->m_SelectAnimationBox = new QComboBox;

    QWidget::connect(this->m_LoadModelButton, SIGNAL(clicked()), this, SLOT(selectModel()));
    QWidget::connect(this->m_SelectAnimationBox, SIGNAL(currentIndexChanged(int)), this, SLOT(loadModel(int)));

    verticalLayout->addLayout(horizontalLayout1);

    horizontalLayout1->addWidget(this->m_LoadModelButton);
    horizontalLayout1->addWidget(this->m_ModelPath);

    verticalLayout->addWidget(new QLabel("Animations disponibles : "));
    verticalLayout->addWidget(this->m_SelectAnimationBox);

    this->setLayout(verticalLayout);
}

gui::GUIWindow::~GUIWindow()
{
}

void
gui::GUIWindow::selectModel()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Selecion d'un model 3D"));

    if(file_name.isNull() || file_name.isEmpty())
        return;

    std::string file_name_str = file_name.toStdString();
    std::string line;
    std::string animation_file;
    std::ifstream file;

    std::string path = file_name_str.substr(0, file_name_str.find_last_of('/')) + "/";

    file.open(file_name.toStdString().c_str());
    GLint i = 0;

    this->m_SelectAnimationBox->clear();
    this->m_SelectAnimationBox->addItem("Defaut");

    while(!file.eof())
    {
        getline(file, line);

        switch(i)
        {
            case 0:
                this->m_ModelPath->setText(QString(("Model : " + std::string(line.c_str())).c_str()));
                i++;
                break;
            case 1:
                this->m_OpenGLWindow->setModel(QString((path + std::string(line.c_str())).c_str()));
                i++;
                break;
            default:
                this->m_SelectAnimationBox->addItem(QString(line.c_str()));
                this->m_OpenGLWindow->addAnimation((path + std::string(line.c_str())));
                break;
        }
    }

    file.close();
}

void
gui::GUIWindow::loadModel(int i)
{
    if( i < 0)
        i = 0;

    this->m_OpenGLWindow->runAnimation(i);
}

void
gui::GUIWindow::setOpenGLWindow(SceneWindow* scene)
{
    this->m_OpenGLWindow = scene;
}
