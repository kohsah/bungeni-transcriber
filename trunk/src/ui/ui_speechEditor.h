/********************************************************************************
** Form generated from reading UI file 'speechEditor.ui'
**
** Created: Mon Oct 31 17:43:08 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPEECHEDITOR_H
#define UI_SPEECHEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTimeEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_speechEditor
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLineEdit *name;
    QTimeEdit *startTime;
    QSpacerItem *horizontalSpacer;
    QPushButton *bold;
    QPushButton *italics;
    QPushButton *underline;
    QCheckBox *incomplete;
    QSpacerItem *verticalSpacer;
    QTimeEdit *endTime;
    QPushButton *cancel;
    QPushButton *save;
    QGridLayout *gridLayout_4;
    QPushButton *play;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *speechEditor)
    {
        if (speechEditor->objectName().isEmpty())
            speechEditor->setObjectName(QString::fromUtf8("speechEditor"));
        speechEditor->resize(498, 350);
        gridLayout_2 = new QGridLayout(speechEditor);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        name = new QLineEdit(speechEditor);
        name->setObjectName(QString::fromUtf8("name"));

        gridLayout->addWidget(name, 1, 0, 1, 7);

        startTime = new QTimeEdit(speechEditor);
        startTime->setObjectName(QString::fromUtf8("startTime"));

        gridLayout->addWidget(startTime, 0, 0, 1, 3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 3, 1, 1);

        bold = new QPushButton(speechEditor);
        bold->setObjectName(QString::fromUtf8("bold"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(bold->sizePolicy().hasHeightForWidth());
        bold->setSizePolicy(sizePolicy);
        bold->setMinimumSize(QSize(32, 32));
        bold->setMaximumSize(QSize(32, 32));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        bold->setFont(font);

        gridLayout->addWidget(bold, 2, 0, 1, 1);

        italics = new QPushButton(speechEditor);
        italics->setObjectName(QString::fromUtf8("italics"));
        sizePolicy.setHeightForWidth(italics->sizePolicy().hasHeightForWidth());
        italics->setSizePolicy(sizePolicy);
        italics->setMinimumSize(QSize(32, 32));
        italics->setMaximumSize(QSize(32, 32));
        QFont font1;
        font1.setBold(false);
        font1.setItalic(true);
        font1.setWeight(50);
        font1.setKerning(false);
        italics->setFont(font1);

        gridLayout->addWidget(italics, 2, 1, 1, 1);

        underline = new QPushButton(speechEditor);
        underline->setObjectName(QString::fromUtf8("underline"));
        sizePolicy.setHeightForWidth(underline->sizePolicy().hasHeightForWidth());
        underline->setSizePolicy(sizePolicy);
        underline->setMaximumSize(QSize(32, 32));
        QFont font2;
        font2.setUnderline(true);
        underline->setFont(font2);

        gridLayout->addWidget(underline, 2, 2, 1, 1);

        incomplete = new QCheckBox(speechEditor);
        incomplete->setObjectName(QString::fromUtf8("incomplete"));

        gridLayout->addWidget(incomplete, 4, 0, 1, 4);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 3, 0, 1, 1);

        endTime = new QTimeEdit(speechEditor);
        endTime->setObjectName(QString::fromUtf8("endTime"));
        endTime->setTime(QTime(0, 0, 0));

        gridLayout->addWidget(endTime, 0, 6, 1, 1);

        cancel = new QPushButton(speechEditor);
        cancel->setObjectName(QString::fromUtf8("cancel"));

        gridLayout->addWidget(cancel, 4, 5, 1, 2);

        save = new QPushButton(speechEditor);
        save->setObjectName(QString::fromUtf8("save"));

        gridLayout->addWidget(save, 4, 4, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        play = new QPushButton(speechEditor);
        play->setObjectName(QString::fromUtf8("play"));
        sizePolicy.setHeightForWidth(play->sizePolicy().hasHeightForWidth());
        play->setSizePolicy(sizePolicy);
        play->setMaximumSize(QSize(32, 32));
        play->setLayoutDirection(Qt::LeftToRight);
        QIcon icon;
        icon.addFile(QString::fromUtf8("../pixmaps/play.png"), QSize(), QIcon::Normal, QIcon::On);
        play->setIcon(icon);

        gridLayout_4->addWidget(play, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_2, 0, 0, 1, 1);


        gridLayout->addLayout(gridLayout_4, 2, 4, 1, 3);


        gridLayout_2->addLayout(gridLayout, 0, 1, 1, 1);


        retranslateUi(speechEditor);

        QMetaObject::connectSlotsByName(speechEditor);
    } // setupUi

    void retranslateUi(QWidget *speechEditor)
    {
        speechEditor->setWindowTitle(QApplication::translate("speechEditor", "Form", 0, QApplication::UnicodeUTF8));
        startTime->setDisplayFormat(QApplication::translate("speechEditor", "hh:mm:ss", 0, QApplication::UnicodeUTF8));
        bold->setText(QApplication::translate("speechEditor", "B", 0, QApplication::UnicodeUTF8));
        italics->setText(QApplication::translate("speechEditor", "i", 0, QApplication::UnicodeUTF8));
        underline->setText(QApplication::translate("speechEditor", "u", 0, QApplication::UnicodeUTF8));
        incomplete->setText(QApplication::translate("speechEditor", "Mark transcript as incomplete", 0, QApplication::UnicodeUTF8));
        endTime->setDisplayFormat(QApplication::translate("speechEditor", "hh:mm:ss", 0, QApplication::UnicodeUTF8));
        cancel->setText(QApplication::translate("speechEditor", "Cancel", 0, QApplication::UnicodeUTF8));
        save->setText(QApplication::translate("speechEditor", "Save", 0, QApplication::UnicodeUTF8));
        play->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class speechEditor: public Ui_speechEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPEECHEDITOR_H
