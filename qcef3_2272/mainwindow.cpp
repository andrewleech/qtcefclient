#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QMessageBox>
#include <QTextStream>
#include "cefclient/cefclient.h"
#include "cefclient/client_app_js.h"
#include "include/cef_command_line.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags),
      ui_(new Ui::MainWindow) {
  qDebug() << __FUNCTION__;
  ui_->setupUi(this);
  SetupUi();

  std::string url;

  // Check if a "--url=" value was provided via the command-line. If so, use
  // that instead of the default URL.
  CefRefPtr<CefCommandLine> command_line = CefCommandLine::GetGlobalCommandLine();
  url = command_line->GetSwitchValue("url");
  kiosk_ = command_line->HasSwitch("kiosk");
  if (url.empty())
	  url = "http://www.google.com";
  
  if (kiosk_) {
	  /*ui_->centralwidget->setVisible(false);*/
	  ui_->backButton->setVisible(false);
	  ui_->forwardButton->setVisible(false);
	  ui_->lineEdit->setVisible(false);
	  ui_->reloadButton->setVisible(false);
	  ui_->stopButton->setVisible(false);

	  ui_->menubar->setVisible(false);
	  ui_->statusbar->setVisible(false);
  }
  
  webview_->load(QUrl(QString::fromStdString(url)));
}

MainWindow::~MainWindow() {
  qDebug() << __FUNCTION__;
  delete ui_;
}

bool MainWindow::is_kiosk() {
	return kiosk_;
}

void MainWindow::closeEvent(QCloseEvent* e) {
  e->accept();
}

void MainWindow::onTitleChanged(const QString& title) {
  qDebug() << __FUNCTION__ << title;
  setWindowTitle(title);
}

void MainWindow::onUrlChanged(const QUrl& url) {
  qDebug() << __FUNCTION__ << url;
  ui_->lineEdit->setText(url.toString());
}

void MainWindow::onLoadStarted() {
  qDebug() << __FUNCTION__;

  QString message;
  QTextStream stream(&message);
  stream << __FUNCTION__ << " : " << webview_->url().toString();
  statusBar()->showMessage(message);

  ui_->lineEdit->setEnabled(true);
  ui_->reloadButton->setEnabled(false);
  ui_->stopButton->setEnabled(true);
  ui_->menuTests->setEnabled(false);
}

void MainWindow::onLoadFinished(bool ok) {
  qDebug() << __FUNCTION__;

  QString message;
  QTextStream stream(&message);
  stream << __FUNCTION__ << " : " << webview_->url().toString();
  statusBar()->showMessage(message);

  ui_->reloadButton->setEnabled(true);
  ui_->stopButton->setEnabled(false);
  ui_->menuTests->setEnabled(true);
}

void MainWindow::onNavStateChanged(bool canGoBack, bool canGoForward) {
  //qDebug() << __FUNCTION__ << canGoBack << canGoForward;
  ui_->backButton->setEnabled(canGoBack);
  ui_->forwardButton->setEnabled(canGoForward);
}

void MainWindow::onJsMessage(const QString& name, const QVariantList& args) {
  //qDebug() << __FUNCTION__ << name << args;
  if (name == JS_FUNC_SENDMESSAGE) {
    QString message;
    foreach (QVariant arg, args) {
      message.append(arg.toString());
      message.append(QLatin1Char('\n'));
    }
    QMessageBox::about(this, name, message);
  }
}

void MainWindow::on_backButton_clicked() {
  //qDebug() << __FUNCTION__;
  webview_->back();
}

void MainWindow::on_forwardButton_clicked() {
  //qDebug() << __FUNCTION__;
  webview_->forward();
}

void MainWindow::on_reloadButton_clicked() {
  //qDebug() << __FUNCTION__;
  webview_->reload();
}

void MainWindow::on_stopButton_clicked() {
  //qDebug() << __FUNCTION__;
  webview_->stop();
}

void MainWindow::on_lineEdit_returnPressed() {
  if (ui_->stopButton->isEnabled()) { // loading
    webview_->stop();
  }
  QUrl url(ui_->lineEdit->text());
  qDebug() << __FUNCTION__ << url;
  webview_->load(url);
}

void MainWindow::on_actionExit_triggered() {
  //qApp->quit();
  close();
}

void MainWindow::on_actionAbout_triggered() {
  //qApp->aboutQt();
  QMessageBox::about(this, tr("About"),
    "<a href=\"http://qt-project.org/\">Qt</a> "
    "(<a href=\"https://code.google.com/p/chromiumembedded/\">CEF3</a>)");
}

void MainWindow::on_actionSendMessage_triggered() {
  QString url = QString("file:///%1/www/SendMessage.html").arg(QDir::currentPath());
  webview_->load(QUrl(url));
}

void MainWindow::on_actionLoadHtml_triggered() {
  // If the url is "about:blank" at first, it will fail to setHtml().
  // And the url will be "data:text/html,chromewebdata" now.
  // 
  // It will also lead to back() issue as follows:
  // "about:blank"(at first) > LoadHtml > Back > Forward(fail to load) >
  // Back > the loading issue.
  // 
  // However setHtml() will lead to some navigation problems.
  webview_->setHtml("<h1>Hello everyone!</h1>", QUrl("tests://LoadHtml/"));
}

void MainWindow::SetupUi() {
  webview_ = new QCefWebView(this);
  connect(webview_, SIGNAL(titleChanged(const QString&)),
          this, SLOT(onTitleChanged(const QString&)));
  connect(webview_, SIGNAL(urlChanged(const QUrl&)),
          this, SLOT(onUrlChanged(const QUrl&)));
  connect(webview_, SIGNAL(loadStarted()),
          this, SLOT(onLoadStarted()));
  connect(webview_, SIGNAL(loadFinished(bool)),
          this, SLOT(onLoadFinished(bool)));
  connect(webview_, SIGNAL(navStateChanged(bool, bool)),
          this, SLOT(onNavStateChanged(bool, bool)));
  connect(webview_, SIGNAL(jsMessage(const QString&, const QVariantList&)),
          this, SLOT(onJsMessage(const QString&, const QVariantList&)));

  ui_->verticalLayout->addWidget(webview_);
}