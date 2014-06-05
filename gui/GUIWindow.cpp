#include "GUIWindow.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>

gui::GUIWindow::GUIWindow(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle(QString("Menu"));
    this->setFixedSize(QSize(300, 150));

    QVBoxLayout *verticalLayout = new QVBoxLayout;
    QHBoxLayout *horizontalLayout1 = new QHBoxLayout;
    QHBoxLayout *horizontalLayout2 = new QHBoxLayout;

    this->m_ModelPath = new QLabel("Model : ");
    this->m_LoadModelButton = new QPushButton("Charger un model");
    this->m_SelectAnimationBox = new QComboBox;

    this->m_LightPath = new QLabel("Eclairage : ");
    this->m_LoadLightButton = new QPushButton("Charger un éclairage");
    this->m_ApplyLightButton = new QPushButton("Appliquer l'éclairage");

    verticalLayout->addLayout(horizontalLayout1);

    horizontalLayout1->addWidget(this->m_LoadModelButton);
    horizontalLayout1->addWidget(this->m_ModelPath);

    horizontalLayout2->addWidget(this->m_LoadLightButton);
    horizontalLayout2->addWidget(this->m_LightPath);

    verticalLayout->addWidget(new QLabel("Animations disponibles : "));
    verticalLayout->addWidget(this->m_SelectAnimationBox);

    verticalLayout->addLayout(horizontalLayout2);
    verticalLayout->addWidget(this->m_ApplyLightButton);

    this->setLayout(verticalLayout);

    QWidget::connect(this->m_LoadModelButton, SIGNAL(clicked()), this, SLOT(selectModel()));
    QWidget::connect(this->m_SelectAnimationBox, SIGNAL(currentIndexChanged(int)), this, SLOT(loadModel(int)));

    QWidget::connect(this->m_LoadLightButton, SIGNAL(clicked()), this, SLOT(selectLight()));
    QWidget::connect(this->m_ApplyLightButton, SIGNAL(clicked()), this, SLOT(applyLight()));
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

void
gui::GUIWindow::selectLight()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Selecion d'un fichier d'éclairage"));

    if(file_name.isNull() || file_name.isEmpty())
        return;

    std::string name;
    std::ifstream file;

    file.open(file_name.toStdString().c_str());
    getline(file, name);
    file.close();

    this->m_LightFilePath = file_name.toStdString();
    this->m_LightPath->setText(QString(("Eclairage : " + name).c_str()));
}

void
gui::GUIWindow::applyLight()
{
    if(this->m_LightFilePath.empty())
        return;

    std::ifstream file;
    std::string line;
    scene::DirectionalLight* directional_light = this->m_OpenGLWindow->getLight()->getDirectionalLight();
    scene::Material* material = this->m_OpenGLWindow->getLight()->getMaterial();

    file.open(this->m_LightFilePath.c_str());
    GLint i = -1;

    while(!file.eof())
    {
        getline(file, line);

       if(line.compare(0, 6, "#light") == 0)
       {
           i=1;
           continue;
       }

       if(i == 1)
       {
           if(line.compare(0, 9, "#material") == 0)
           {
                i=2;
                continue;
           }
           else if(line.compare(0, 7, "ambiant") == 0)
           {
                sscanf(line.c_str(), "ambiant %f %f %f\n", &directional_light->ambientColor[0],  &directional_light->ambientColor[1],  &directional_light->ambientColor[2]);
           }
           else if(line.compare(0, 7, "diffuse") == 0)
           {
                sscanf(line.c_str(), "diffuse %f %f %f\n", &directional_light->diffuseColor[0],  &directional_light->diffuseColor[1],  &directional_light->diffuseColor[2]);
           }
           else if(line.compare(0, 8, "specular") == 0)
           {
                sscanf(line.c_str(), "specular %f %f %f\n", &directional_light->specularColor[0],  &directional_light->specularColor[1],  &directional_light->specularColor[2]);
           }
           else if(line.compare(0, 9, "halfplane") == 0)
           {
                sscanf(line.c_str(), "halfplane %f %f %f\n", &directional_light->halfPlane[0],  &directional_light->halfPlane[1],  &directional_light->halfPlane[2]);
           }
           else if(line.compare(0, 9, "direction") == 0)
           {
                sscanf(line.c_str(), "direction %f %f %f\n", &directional_light->direction[0],  &directional_light->direction[1],  &directional_light->direction[2]);
           }
       }
       else if(i == 2)
       {
           if(line.compare(0, 7, "ambiant") == 0)
           {
                sscanf(line.c_str(), "ambiant %f %f %f\n", &material->ambientFactor[0],  &material->ambientFactor[1],  &material->ambientFactor[2]);
           }
           else if(line.compare(0, 7, "diffuse") == 0)
           {
                sscanf(line.c_str(), "diffuse %f %f %f\n", &material->diffuseFactor[0],  &material->diffuseFactor[1],  &material->diffuseFactor[2]);
           }
           else if(line.compare(0, 8, "specular") == 0)
           {
                sscanf(line.c_str(), "specular %f %f %f\n", &material->specularFactor[0],  &material->specularFactor[1],  &material->specularFactor[2]);
           }
           else if(line.compare(0, 9, "shininess") == 0)
           {
                sscanf(line.c_str(), "shininess %f\n", &material->shininess);
           }
       }
    }

    file.close();
}
