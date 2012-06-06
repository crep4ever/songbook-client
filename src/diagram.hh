// Copyright (C) 2009-2012, Romain Goffe <romain.goffe@gmail.com>
// Copyright (C) 2009-2012, Alexandre Dupas <alexandre.dupas@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301, USA.
//******************************************************************************
#ifndef __DIAGRAM_HH__
#define __DIAGRAM_HH__

#include "diagram.hh"
#include "utils/utils.hh"
#include "notification.hh"

#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QRadioButton>

/**
 * \file diagram.hh
 * \class CDiagram
 * \brief CDiagram is a widget representing a guitar/ukulele chord diagram
 *
 */
class CDiagram : public QWidget
{
  Q_OBJECT

public:
  enum StringCount {
    GuitarStringCount=6,
    UkuleleStringCount=4
  };

  CDiagram(const QString & chord, const ChordType & type = GuitarChord, QWidget *parent = 0);
  ~CDiagram();

  QString toString();
  void fromString(const QString & gtab);

  QString chord() const;
  void setChord(const QString & name);

  QString fret() const;
  void setFret(const QString & fret);

  QString strings() const;
  void setStrings(const QString & chord);

  ChordType type() const;
  void setType(const ChordType & type);

  bool isImportant() const;
  void setImportant(bool value);

  bool isValidChord() const;

  QSize minimumSizeHint() const;
  QSize sizeHint() const;

protected:
  void paintEvent(QPaintEvent * event);

private:
  void fillEllipse(QPainter* painter, const QRect & rect, const QBrush & brush);

  ChordType m_type;
  QString m_chord;
  QString m_fret;
  QString m_strings;
  bool m_important;

  static QRegExp reChord;
  static QRegExp reFret;
  static QRegExp reStringsFret;
  static QRegExp reStringsNoFret;
};

class QMouseEvent;
class QLabel;

/**
 * \file diagram.hh
 * \class CDiagramWidget
 * \brief CDiagramWidget embeds a CDiagram and related actions
 *
 */
class CDiagramWidget : public QWidget
{
  Q_OBJECT

public:
  CDiagram* m_diagram;

  /// Constructor.
  /// @param chord a gtab macro content such as B&m}{1:X0222 representing a chord
  CDiagramWidget(const QString & chord, const ChordType & type = GuitarChord, QWidget *parent = 0);

  ///Destructor.
  ~CDiagramWidget();

  bool isSelected() const;
  void setSelected(bool value);
  int stringCount() const;
  void updateCircleIcon(QLabel *Label, bool isValid);

  QRadioButton *m_guitar;
  QLineEdit *m_stringsLineEdit;
  QLineEdit *m_nameLineEdit;
  QLabel *m_messageLabel;
  QLabel *m_iconChordNameValid;
  QLabel *m_iconStringLineEditValid;

  QString toString();
  ChordType type() const;

protected:
  virtual void mouseDoubleClickEvent(QMouseEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);

signals:
  void changed();
  void clicked();
  void diagramCloseRequested();
  void diagramChanged();

private slots:
  void updateBackground();
  void updateChordName();
  void updateChord();

public slots:
  bool editChord();
  void removeChord();

private:
  QLabel *m_chordName;
  bool m_selected;
};

class QSpacerItem;
class QToolButton;
class QBoxLayout;

/**
 * \file diagram.hh
 * \class CDiagramArea
 * \brief CDiagramArea contains a list of diagrams
 *
 */
class CDiagramArea : public QWidget
{
  Q_OBJECT

public:
  CDiagramArea(QWidget *parent=0);

  CDiagramWidget * addDiagram(const QString & chord, const ChordType & type);
  QList<CDiagramWidget*> diagrams() const;

protected:
  virtual void keyPressEvent(QKeyEvent *event);

private:
  void addNewDiagramButton();

private slots:
  void onDiagramChanged();
  void onDiagramClicked();
  CDiagramWidget * addDiagram();
  void removeDiagram();

signals:
  void contentsChanged();

private:
  QBoxLayout *m_layout;
  QToolButton *m_addDiagramButton;
  QSpacerItem *m_spacer;
  QList<CDiagramWidget*> m_diagrams;
};

#endif // __DIAGRAM_HH__
