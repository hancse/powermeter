#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
  QDialog(parent), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->lblText->setOpenExternalLinks(true);
    ui->lblText->setText("\
<b>PowerMeter</b> is a program for control of the <b>DEIF MIC-2 MKII AC mains power meter</b> \
<br><br> \
<b>PowerMeter</b> is proprietary software. It uses the following software: \
<br><br>\
Qt 5.12 Framework (LGPL license).  \
<br> \
See <a href=\"http://www.qt.io/licensing/\">qt.io/licensing</a> for an overview of Qt licensing. \
<br><br> \
The Qwt - Qt Widgets for Technical Applications project, version 6.4.1 (Qwt license)\
<br>\
See <a href=\"https://qwt.sourceforge.io/\">qwt.sourceforge.io</a>. Authors: Uwe Rathmann, Josef Wilgen \
<br><br> \
FontAwesome Free icons and fonts, version 5.7.2 \
<br>\
See <a href=\"https://fontawesome.com/license/free\">fontawesome.com/license/free</a>. \
<br><br>\
<b>PowerMeter</b> has been written by Paul van Kan, <b>&copy; 2018-2019 Hogeschool van Arnhem en Nijmegen </b> \
<br>and has been developed for the <b> Control Systems Engineering (CSE) group</b> ");
}

//<h1>About AQtuator</h1>
//"<a href=\"http://www.qtcentre.org\" />QtCentre</a>"

AboutDialog::~AboutDialog() {
  delete ui;
}

