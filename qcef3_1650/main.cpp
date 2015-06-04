#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include "cefclient/cefclient.h"

#include <windows.h>
#include <tchar.h>

namespace {

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process;

}  // namespace

// Detect whether the operating system is a 64-bit.
// http://msdn.microsoft.com/en-us/library/windows/desktop/ms684139%28v=vs.85%29.aspx
BOOL IsWow64() {
  BOOL bIsWow64 = FALSE;

  fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
      GetModuleHandle(TEXT("kernel32")),
      "IsWow64Process");

  if (NULL != fnIsWow64Process) {
    if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64)) {
      // handle error
    }
  }
  return bIsWow64;
}

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	//QCoreApplication::setApplicationName("cefclient");
	//QCoreApplication::setApplicationVersion("1.0");
	
	//QCommandLineParser parser;
	//parser.setApplicationDescription("cefclient");
	//parser.addHelpOption();
	//parser.addVersionOption();
	//parser.addPositionalArgument("url", QCoreApplication::translate("main", "url to load"));
	//QCommandLineOption kiosk("kiosk", QCoreApplication::translate("main", "load full screen"));
	//parser.addPositionalArgument("url", "url to load");
	//QCommandLineOption kiosk("kiosk", "load full screen");
	//parser.addOption(kiosk);

	//parser.process(a);

	int result = CefInit(argc, argv);
	if (result >= 0)
		return result;
	
	// Load flash system plug-in on Windows.
	CefLoadPlugins(IsWow64());

	MainWindow w;
	if (w.is_kiosk()){
		w.showFullScreen();
	}
	else {
		w.show();
	}

	//a.setQuitOnLastWindowClosed(false);
	result = a.exec();

	CefQuit();

	return result;
}
