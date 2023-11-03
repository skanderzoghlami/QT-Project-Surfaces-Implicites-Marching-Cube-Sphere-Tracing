#ifndef __Qte__
#define __Qte__

#include <QtWidgets/qmainwindow.h>
#include "realtime.h"
#include "meshcolor.h"
#include "implicits.h"
#include "treehierarchy.h"

QT_BEGIN_NAMESPACE
	namespace Ui { class Assets; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT
private:
  Ui::Assets* uiw;           //!< Interface

  MeshWidget* meshWidget;   //!< Viewer
  MeshColor meshColor;		//!< Mesh.

public:
  static TreeHierarchy* swordSDF ;
    static std::vector<SphereSDF> erosionSpheres;
  MainWindow();
  ~MainWindow();
  void CreateActions();
  void UpdateGeometry();

public slots:
  void editingSceneLeft(const Ray&);
  void editingSceneRight(const Ray&);
  void BoxMeshExample();
  void SphereImplicitExample();

  void ShowTorusExample();
  void ShowCapsuleExample();
  void ShowBoiteExample();
  void ShowSwordExample();
  void editingErosion(const Ray& ray);

  void ResetCamera();
  void UpdateMaterial();
};

#endif
