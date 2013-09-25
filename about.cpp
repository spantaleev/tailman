#include "about.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

About::About(QWidget *parent) :
    QDialog(parent) {

    createUI();
}

void About::createUI() {

    setFixedSize(350,220);
    setWindowTitle("About");

    setStyleSheet("QLabel { qproperty-alignment:AlignCenter; }");

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QLabel *labelHeadline = new QLabel("TailMan");
    labelHeadline->setStyleSheet("QLabel { font-size:16pt; font-weight:bold; }");

    QLabel *labelVersion = new QLabel(QString(
        "<strong>Version:</strong> ").append(VERSION));
    labelVersion->setStyleSheet("QLabel { font-size:10pt; }");

    QLabel *labelLicense = new QLabel(QString(
        "<strong>License:</strong> GPL"));
    labelLicense->setStyleSheet("QLabel { font-size:10pt; }");

    QLabel *labelAuthor = new QLabel(
        "<strong>Author:</strong> Martin Hjärtmyr " \
        "(<a href='mailto:martin@hjartmyr.se'>martin@hjartmyr.se</a>)");
    labelAuthor->setOpenExternalLinks(true);
    labelAuthor->setStyleSheet("QLabel { font-size:10pt; }");

    QLabel *labelWeb = new QLabel(
        "<a href='http://github.com/martinhjartmyr/tailman'>" \
        "http://github.com/martinhjartmyr/tailman</a>");
    labelWeb->setOpenExternalLinks(true);
    labelWeb->setStyleSheet("QLabel { font-size:10pt; }");

    QPushButton *buttonClose = new QPushButton("Close");
    connect(buttonClose, SIGNAL(clicked()), this, SLOT(accept()));

    mainLayout->addWidget(labelHeadline);
    mainLayout->addWidget(labelLicense);
    mainLayout->addWidget(labelVersion);
    mainLayout->addWidget(labelAuthor);
    mainLayout->addWidget(labelWeb);
    mainLayout->addWidget(buttonClose);

    setLayout(mainLayout);
}

void About::accept() {

    QDialog::accept();
    this->hide();
}
