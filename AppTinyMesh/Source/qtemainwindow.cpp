#include "qte.h"
#include "implicits.h"
#include "ui_interface.h"
#include "treehierarchy.h"
#include <chrono>













// Definition and initialization
 TreeHierarchy* MainWindow::swordSDF ;
 std::vector<SphereSDF> MainWindow::erosionSpheres;




MainWindow::MainWindow() : QMainWindow(), uiw(new Ui::Assets)
{
	// Chargement de l'interface
    uiw->setupUi(this);

	// Chargement du GLWidget
	meshWidget = new MeshWidget;
	QGridLayout* GLlayout = new QGridLayout;
	GLlayout->addWidget(meshWidget, 0, 0);
	GLlayout->setContentsMargins(0, 0, 0, 0);
    uiw->widget_GL->setLayout(GLlayout);

	// Creation des connect
	CreateActions();

    meshWidget->SetCamera(Camera(Vector(10, 0, 0), Vector(0.0, 0.0, 0.0)));
}

MainWindow::~MainWindow()
{
	delete meshWidget;
}

void MainWindow::CreateActions()
{
	// Buttons
    connect(uiw->boxMesh, SIGNAL(clicked()), this, SLOT(BoxMeshExample()));
    connect(uiw->sphereImplicit, SIGNAL(clicked()), this, SLOT(SphereImplicitExample()));
    connect(uiw->resetcameraButton, SIGNAL(clicked()), this, SLOT(ResetCamera()));
    connect(uiw->wireframe, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_1, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));
    connect(uiw->radioShadingButton_2, SIGNAL(clicked()), this, SLOT(UpdateMaterial()));

    connect(uiw->showTorusButton, SIGNAL(clicked()), this, SLOT(ShowTorusExample()));
    connect(uiw->showCapsuleButton, SIGNAL(clicked()), this, SLOT(ShowCapsuleExample()));
    connect(uiw->showBoiteButton, SIGNAL(clicked()), this, SLOT(ShowBoiteExample()));
    connect(uiw->Sword, SIGNAL(clicked()), this, SLOT(ShowSwordExample()));
    connect(meshWidget, SIGNAL(_signalErosion(const Ray&)), this, SLOT(editingErosion(const Ray&)));

    // Widget edition
    connect(meshWidget, SIGNAL(_signalEditSceneLeft(const Ray&)), this, SLOT(editingSceneLeft(const Ray&)));
    connect(meshWidget, SIGNAL(_signalEditSceneRight(const Ray&)), this, SLOT(editingSceneRight(const Ray&)));
}

void MainWindow::editingSceneLeft(const Ray&)
{
}

void MainWindow::editingSceneRight(const Ray&)
{
}

void MainWindow::BoxMeshExample()
{
	Mesh boxMesh = Mesh(Box(1.0));

	std::vector<Color> cols;
	cols.resize(boxMesh.Vertexes());
    for (size_t i = 0; i < cols.size(); i++)
		cols[i] = Color(double(i) / 6.0, fmod(double(i) * 39.478378, 1.0), 0.0);

	meshColor = MeshColor(boxMesh, cols, boxMesh.VertexIndexes());
	UpdateGeometry();
}

void MainWindow::SphereImplicitExample()
{
  SphereSDF implicit(0.8);

  Mesh implicitMesh;
  implicit.Polygonize(31, implicitMesh, Box(2.0));
  std::vector<Color> cols;
  cols.resize(implicitMesh.Vertexes());
  for (size_t i = 0; i < cols.size(); i++)
    cols[i] = Color(0.8, 0.8, 0.8);
  implicitMesh.Translate(Vector(uiw->transX->text().toDouble() , uiw->transY->text().toDouble() , uiw->transZ->text().toDouble()));
  implicitMesh.Rotate(uiw->rotx->text().toDouble(),uiw->roty->text().toDouble(),uiw->rotz->text().toDouble());
  implicitMesh.Scale(Matrix3(uiw->scale->text().toDouble() , 0 ,0 , 0 , uiw->scale->text().toDouble() ,0 , 0 ,0 , uiw->scale->text().toDouble()));

  meshColor = MeshColor(implicitMesh, cols, implicitMesh.VertexIndexes());
  UpdateGeometry();
}

void MainWindow::ShowTorusExample()
{
  // Create an instance of your TorusSDF with appropriate parameters
  double param1 = uiw->para1->text().toDouble();
  double param2 = uiw->para2->text().toDouble();
  TorusSDF torusSDF(param1, param2);

  Mesh torusMesh;  // Create an empty mesh to hold the torus surface
  torusSDF.Polygonize(64, torusMesh, Box(2.0));  // Polygonize the SDF

  std::vector<Color> cols(torusMesh.Vertexes(), Color(0.8, 0.8, 0.8));
  torusMesh.Translate(Vector(uiw->transX->text().toDouble() , uiw->transY->text().toDouble() , uiw->transZ->text().toDouble()));
  torusMesh.Rotate(uiw->rotx->text().toDouble(),uiw->roty->text().toDouble(),uiw->rotz->text().toDouble());
  torusMesh.Scale(Matrix3(uiw->scale->text().toDouble() , 0 ,0 , 0 , uiw->scale->text().toDouble() ,0 , 0 ,0 , uiw->scale->text().toDouble()));
  meshColor = MeshColor(torusMesh, cols, torusMesh.VertexIndexes());
  UpdateGeometry();
}



void MainWindow::ShowCapsuleExample()
{
  // Create an instance of your TorusSDF with appropriate parameters
  double xDim = uiw->para1->text().toDouble();
  double yDim = uiw->para2->text().toDouble();
  CapsuleSDF CapSDF(xDim, yDim);

  Mesh CapsuleMesh;  // Create an empty mesh to hold the torus surface
  CapSDF.Polygonize(64, CapsuleMesh, Box(2.0));  // Polygonize the SDF

  std::vector<Color> cols(CapsuleMesh.Vertexes(), Color(0.8, 0.8, 0.8));
  CapsuleMesh.Translate(Vector(uiw->transX->text().toDouble() , uiw->transY->text().toDouble() , uiw->transZ->text().toDouble()));
  CapsuleMesh.Rotate(uiw->rotx->text().toDouble(),uiw->roty->text().toDouble(),uiw->rotz->text().toDouble());
  CapsuleMesh.Scale(Matrix3(uiw->scale->text().toDouble() , 0 ,0 , 0 , uiw->scale->text().toDouble() ,0 , 0 ,0 , uiw->scale->text().toDouble()));

  meshColor = MeshColor(CapsuleMesh, cols, CapsuleMesh.VertexIndexes());
  UpdateGeometry();
}



void MainWindow::ShowBoiteExample()
{
  // Create an instance of your TorusSDF with appropriate parameters
  double xDim = uiw->para1->text().toDouble();
  double yDim = uiw->para2->text().toDouble();
  double zDim = uiw->para3->text().toDouble();
  BoiteSDF bsdf(xDim, yDim,zDim);

  Mesh BoiteMesh;  // Create an empty mesh to hold the torus surface
  bsdf.Polygonize(64, BoiteMesh, Box(2.0));  // Polygonize the SDF

  std::vector<Color> cols(BoiteMesh.Vertexes(), Color(0.8, 0.8, 0.8));
  BoiteMesh.Translate(Vector(uiw->transX->text().toDouble() , uiw->transY->text().toDouble() , uiw->transZ->text().toDouble()));
  BoiteMesh.Rotate(uiw->rotx->text().toDouble(),uiw->roty->text().toDouble(),uiw->rotz->text().toDouble());
  BoiteMesh.Scale(Matrix3(uiw->scale->text().toDouble() , 0 ,0 , 0 , uiw->scale->text().toDouble() ,0 , 0 ,0 , uiw->scale->text().toDouble()));

  meshColor = MeshColor(BoiteMesh, cols, BoiteMesh.VertexIndexes());
  UpdateGeometry();
}

void MainWindow::ShowSwordExample()
{
  auto start_time = std::chrono::high_resolution_clock::now();
  CapsuleSDF capsuleSDF(0.2,10);
  capsuleSDF.ty = 6 ;
  BoiteSDF boiteSDF(0.2,0.4,2.5) ;
  AnalyticScalarField sphere ;
  BoiteSDF boiteLeft(0.2,0.8,0.4) ;
  BoiteSDF boiteRight(0.2,0.8,0.4) ;
  BoiteSDF boite_bottom(0.4,0.2,1) ;
  ConeSDF tipOfSword(6,1.3 ,1.75) ;
  tipOfSword.tx = 0 ;
  tipOfSword.ty =-5.85 ;
  tipOfSword.tz =-0.9 ;
  BoiteSDF boite2(1,6,1) ;

  Mesh BoiteMesh ;
  SmoothUnionNode* unode1 = new SmoothUnionNode(&capsuleSDF,&tipOfSword) ;
  boiteSDF.ty = 7;
  UnionNode* unode2 = new UnionNode(unode1,&boiteSDF) ;
  sphere.ty=7;
  SmoothUnionNode* unode3 = new SmoothUnionNode(unode2,&sphere) ;
  boiteLeft.tz = 2;
  boiteLeft.ty = 7;
  UnionNode* unode4 = new UnionNode(unode3,&boiteLeft) ;

  boiteRight.tz = -2;
  boiteRight.ty = 7;

  UnionNode* unode5 = new UnionNode(unode4,&boiteRight) ;
  boite_bottom.ty = 9.7;
  SmoothUnionNode* unode6 = new SmoothUnionNode(unode5,&boite_bottom) ;
  boite2.ty = 0.5 ;
  boite2.tx = -1.2 ;
  DifferenceNode* diffnode = new DifferenceNode(unode6, &boite2) ;
  BoiteSDF boite3(1,6,1) ;
  boite3.ty = 0.5 ;
  boite3.tx = 1.2 ;
  DifferenceNode* diffnode2 = new DifferenceNode(diffnode, &boite3) ;

  TorusSDF deco1(0.1, 0.4);
  deco1.ty = 8 ;
  SmoothUnionNode* decoration1 = new SmoothUnionNode(diffnode2,&deco1);

  TorusSDF deco2(0.05 , 0.4);
  deco2.ty = 9 ;
  MainWindow::swordSDF=  new SmoothUnionNode(decoration1 , &deco2);
  for ( SphereSDF& erosionSphere : MainWindow::erosionSpheres) {
    // Subtract each erosion sphere from the swordSDF
    swordSDF = new DifferenceNode(swordSDF, &erosionSphere);
  }
  MainWindow::swordSDF->Polygonize(256, BoiteMesh, Box(10.0));  // Polygonize the SDF
//  BoiteMesh.Rotate(90,uiw->roty->text().toDouble(),uiw->rotz->text().toDouble());
//  BoiteMesh.Scale(Matrix3(0.7 , 0 ,0 , 0 , 0.7 ,0 , 0 ,0 , 0.7));
  std::vector<Color> cols(BoiteMesh.Vertexes(), Color(0.8, 0.8, 0.8));
  meshColor = MeshColor(BoiteMesh, cols, BoiteMesh.VertexIndexes());
  BoiteMesh.SaveObj("sword.obj" , "BoiteMesh");

  UpdateGeometry();
  auto end_time = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

  uiw->time->setText(QString::number(duration.count()));
}


void MainWindow::editingErosion(const Ray& ray) {
  CapsuleSDF capsuleSDF(0.2,10);
  capsuleSDF.ty = 6 ;
  BoiteSDF boiteSDF(0.2,0.4,2.5) ;
  AnalyticScalarField sphere ;
  BoiteSDF boiteLeft(0.2,0.8,0.4) ;
  BoiteSDF boiteRight(0.2,0.8,0.4) ;
  BoiteSDF boite_bottom(0.4,0.2,1) ;
  ConeSDF tipOfSword(6,1.3 ,1.75) ;
  tipOfSword.tx = 0 ;
  tipOfSword.ty =-5.85 ;
  tipOfSword.tz =-0.9 ;
  BoiteSDF boite2(1,6,1) ;

  Mesh BoiteMesh ;
  SmoothUnionNode* unode1 = new SmoothUnionNode(&capsuleSDF,&tipOfSword) ;
  boiteSDF.ty = 7;
  UnionNode* unode2 = new UnionNode(unode1,&boiteSDF) ;
  sphere.ty=7;
  SmoothUnionNode* unode3 = new SmoothUnionNode(unode2,&sphere) ;
  boiteLeft.tz = 2;
  boiteLeft.ty = 7;
  UnionNode* unode4 = new UnionNode(unode3,&boiteLeft) ;

  boiteRight.tz = -2;
  boiteRight.ty = 7;

  UnionNode* unode5 = new UnionNode(unode4,&boiteRight) ;
  boite_bottom.ty = 9.7;
  SmoothUnionNode* unode6 = new SmoothUnionNode(unode5,&boite_bottom) ;
  boite2.ty = 0.5 ;
  boite2.tx = -1.2 ;
  DifferenceNode* diffnode = new DifferenceNode(unode6, &boite2) ;
  BoiteSDF boite3(1,6,1) ;
  boite3.ty = 0.5 ;
  boite3.tx = 1.2 ;
  DifferenceNode* diffnode2 = new DifferenceNode(diffnode, &boite3) ;

  TorusSDF deco1(0.1, 0.4);
  deco1.ty = 8 ;
  SmoothUnionNode* decoration1 = new SmoothUnionNode(diffnode2,&deco1);

  TorusSDF deco2(0.05 , 0.4);
  deco2.ty = 9 ;
  MainWindow::swordSDF=  new SmoothUnionNode(decoration1 , &deco2);

  const float lipschitz = 1;
  Vector p = ray.Origin();
  const float minDistance = 0.01;
  const int maxIterations = 100;

  for (int i = 0; i < maxIterations; i++) {
    double distance =  swordSDF->Value(p);
    if (distance < minDistance) {
        // Create a small sphere for erosion
        SphereSDF erosionSphere(0.05);
        for (int j = 0; j < 10; j++) {
            // Calculate the position of each sphere
            double angle = (2 * M_PI * j) / 10;
            double radius = 0.1;  // Adjust the radius as needed
            erosionSphere.tx = -p[0] + radius * cos(angle);
            erosionSphere.ty = -p[1] + radius * sin(angle);
            erosionSphere.tz = -p[2];

            // Add the erosion sphere to the vector
            MainWindow::erosionSpheres.push_back(erosionSphere);
        }

        break;
    }
    p += ray.Direction() * (distance / lipschitz);
  }
  for ( SphereSDF& erosionSphere : MainWindow::erosionSpheres) {
    // Subtract each erosion sphere from the swordSDF
    swordSDF = new DifferenceNode(swordSDF, &erosionSphere);
  }
  MainWindow::swordSDF->Polygonize(64, BoiteMesh, Box(10.0));  // Polygonize the SDF
  std::vector<Color> cols(BoiteMesh.Vertexes(), Color(0.8, 0.8, 0.8));
  meshColor = MeshColor(BoiteMesh, cols, BoiteMesh.VertexIndexes());
  BoiteMesh.SaveObj("sword.obj" , "BoiteMesh");

  UpdateGeometry();


}


void MainWindow::UpdateGeometry()
{
	meshWidget->ClearAll();
    meshWidget->AddMesh("BoxMesh", meshColor);

    uiw->lineEdit->setText(QString::number(meshColor.Vertexes()));
    uiw->lineEdit_2->setText(QString::number(meshColor.Triangles()));

	UpdateMaterial();
}

void MainWindow::UpdateMaterial()
{
    meshWidget->UseWireframeGlobal(uiw->wireframe->isChecked());

    if (uiw->radioShadingButton_1->isChecked())
		meshWidget->SetMaterialGlobal(MeshMaterial::Normal);
	else
		meshWidget->SetMaterialGlobal(MeshMaterial::Color);
}

void MainWindow::ResetCamera()
{
	meshWidget->SetCamera(Camera(Vector(-10.0), Vector(0.0)));
}
