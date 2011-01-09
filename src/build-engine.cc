// Copyright (C) 2009 Romain Goffe, Alexandre Dupas
//
// Songbook Creator is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// Songbook Creator is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA  02110-1301, USA.
//******************************************************************************

#include "build-engine.hh"
#include "mainwindow.hh"
#include "highlighter.hh"

CBuildEngine::CBuildEngine(CMainWindow* AParent)
  : QWidget()
{
  m_parent = AParent;
  m_workingPath = parent()->workingPath();

  m_process = new QProcess(AParent);
  process()->setWorkingDirectory(parent()->workingPath());
   
  setStatusSuccessMessage(tr("Success!"));
  setStatusErrorMessage(tr("Error!"));
  
  connect(parent(), SIGNAL(workingPathChanged(QString)),
	  this, SLOT(setWorkingPath(QString)));
  
  connect(process(), SIGNAL(finished(int,QProcess::ExitStatus)),
	  this, SLOT(processExit(int,QProcess::ExitStatus)));

  connect(process(), SIGNAL(error(QProcess::ProcessError)),
	  this, SLOT(processError(QProcess::ProcessError)));

  connect(process(), SIGNAL(readyReadStandardOutput()),
	  this, SLOT(readProcessOut()));
  
  connect(process(), SIGNAL(readyReadStandardError()),
	  this, SLOT(readProcessOut()));
}
//------------------------------------------------------------------------------
CMainWindow* CBuildEngine::parent()
{
  if(!m_parent) qWarning() << "CBuildEngine::parent invalid parent" ;
  return m_parent ;
}
//------------------------------------------------------------------------------
void CBuildEngine::processExit(int exitCode, QProcess::ExitStatus exitStatus)
{
  parent()->progressBar()->hide();
  
  if (exitStatus == QProcess::NormalExit && exitCode==0)
    {
      parent()->statusBar()->showMessage(statusSuccessMessage());
      updateDialog();
    }
  else
    processError(QProcess::UnknownError);
}
//------------------------------------------------------------------------------
void CBuildEngine::processError(QProcess::ProcessError error)
{
  parent()->progressBar()->hide();
  
  QMessageBox msgBox;
  msgBox.setIcon(QMessageBox::Critical);
  msgBox.setText(statusErrorMessage());
  msgBox.setDetailedText(parent()->log()->toPlainText());
  msgBox.setStandardButtons(QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Cancel);
  msgBox.exec();
}
//------------------------------------------------------------------------------
void CBuildEngine::readProcessOut()
{
  parent()->log()->append(process()->readAllStandardOutput().data());
}
//------------------------------------------------------------------------------
void CBuildEngine::dialog()
{
  if(!mainWidget()) return;

  m_dialog = new QDialog;
  m_buttonBox = new QDialogButtonBox;

  QPushButton * button = new QPushButton(tr("Close"));
  m_buttonBox->addButton(button, QDialogButtonBox::DestructiveRole);
  connect(button, SIGNAL(clicked()), m_dialog, SLOT(close()) );

  button = new QPushButton(tr("Apply"));
  button->setDefault(true);
  m_buttonBox->addButton(button, QDialogButtonBox::ActionRole);
  connect(button, SIGNAL(clicked()), this, SLOT(action()) );

  updateDialog();

  m_dialog->setWindowTitle(windowTitle());
  m_dialog->setMinimumWidth(450);
  m_dialog->setMinimumHeight(450);
  m_dialog->show();
}
//------------------------------------------------------------------------------
void CBuildEngine::updateDialog()
{
  if(!mainWidget()) return;
  delete m_layout;
  
  m_layout = new QVBoxLayout;
  m_layout->addWidget(mainWidget());
  m_layout->addWidget(m_buttonBox);
  m_dialog->setLayout(m_layout);
}
//------------------------------------------------------------------------------
void CBuildEngine::action()
{
  parent()->statusBar()->showMessage(statusActionMessage());
  parent()->progressBar()->show();
  parent()->log()->clear();
  QApplication::processEvents();
  
  process()->start(fileName(), processOptions());
}
//------------------------------------------------------------------------------
QString CBuildEngine::windowTitle() const
{
  return m_windowTitle;
}
//------------------------------------------------------------------------------
void CBuildEngine::setWindowTitle(const QString & value)
{
  m_windowTitle = value;
}
//------------------------------------------------------------------------------
QString CBuildEngine::fileName() const
{
  return m_fileName;
}
//------------------------------------------------------------------------------
void CBuildEngine::setFileName(const QString & value)
{
  m_fileName = value;
}
//------------------------------------------------------------------------------
QString CBuildEngine::statusSuccessMessage() const
{
  return m_statusSuccessMessage;
}
//------------------------------------------------------------------------------
void CBuildEngine::setStatusSuccessMessage(const QString & value)
{
  m_statusSuccessMessage = value;
}
//------------------------------------------------------------------------------
QString CBuildEngine::statusErrorMessage() const
{
  return m_statusErrorMessage;
}
//------------------------------------------------------------------------------
void CBuildEngine::setStatusErrorMessage(const QString & value)
{
  m_statusErrorMessage = value;
}
//------------------------------------------------------------------------------
QString CBuildEngine::statusActionMessage() const
{
  return m_statusActionMessage;
}
//------------------------------------------------------------------------------
void CBuildEngine::setStatusActionMessage(const QString & value)
{
  m_statusActionMessage = value;
}
//------------------------------------------------------------------------------
QStringList CBuildEngine::processOptions() const
{
  return m_processOptions;
}
//------------------------------------------------------------------------------
void CBuildEngine::setProcessOptions(const QStringList & value)
{
  m_processOptions = value;
}
//------------------------------------------------------------------------------
QProcess* CBuildEngine::process() const
{
  return m_process;
}
//------------------------------------------------------------------------------
QString CBuildEngine::workingPath() const
{
  return m_workingPath;
}
//------------------------------------------------------------------------------
void CBuildEngine::setWorkingPath(QString value)
{
  m_workingPath = value;
}
//------------------------------------------------------------------------------
