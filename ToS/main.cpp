#include "ToSForm.h"
#include <cstdlib>
using namespace System;
using namespace System::Windows::Forms;
[System::STAThread]
int WinMain() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	ToSF::ToSForm form;
	Application::Run(%form);
	exit(0);
	return 0;
}
