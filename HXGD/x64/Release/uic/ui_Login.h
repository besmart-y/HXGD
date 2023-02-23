/********************************************************************************
** Form generated from reading UI file 'Login.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form_Login
{
public:
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QPushButton *Btn_Cancel;
    QPushButton *Btn_Login;
    QLabel *label_2;
    QLineEdit *LineEd_UserName;
    QLineEdit *LineEd_UserPassword;
    QLabel *label;
    QLabel *label_7;
    QComboBox *cBx_permission;
    QWidget *page_2;
    QPushButton *Btn_LoginEdit;
    QLabel *label_3;
    QLineEdit *LineEd_UserNameEdit;
    QLineEdit *LineEd_UserPasswordEdit;
    QLabel *label_4;
    QPushButton *Btn_CancelEdit;
    QLineEdit *LineEd_UserPasswordEditC;
    QLabel *label_5;
    QLabel *label_6;
    QComboBox *CBox_UserLevelEdit;

    void setupUi(QWidget *Form_Login)
    {
        if (Form_Login->objectName().isEmpty())
            Form_Login->setObjectName(QString::fromUtf8("Form_Login"));
        Form_Login->resize(535, 347);
        Form_Login->setStyleSheet(QString::fromUtf8(""));
        gridLayout = new QGridLayout(Form_Login);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        stackedWidget = new QStackedWidget(Form_Login);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        Btn_Cancel = new QPushButton(page);
        Btn_Cancel->setObjectName(QString::fromUtf8("Btn_Cancel"));
        Btn_Cancel->setGeometry(QRect(310, 220, 91, 31));
        Btn_Cancel->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        Btn_Login = new QPushButton(page);
        Btn_Login->setObjectName(QString::fromUtf8("Btn_Login"));
        Btn_Login->setGeometry(QRect(170, 220, 91, 31));
        Btn_Login->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_2 = new QLabel(page);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(110, 170, 54, 12));
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        LineEd_UserName = new QLineEdit(page);
        LineEd_UserName->setObjectName(QString::fromUtf8("LineEd_UserName"));
        LineEd_UserName->setGeometry(QRect(170, 100, 231, 31));
        LineEd_UserPassword = new QLineEdit(page);
        LineEd_UserPassword->setObjectName(QString::fromUtf8("LineEd_UserPassword"));
        LineEd_UserPassword->setGeometry(QRect(170, 160, 231, 31));
        label = new QLabel(page);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(110, 110, 54, 12));
        label->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_7 = new QLabel(page);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(110, 50, 54, 12));
        label_7->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        cBx_permission = new QComboBox(page);
        cBx_permission->addItem(QString());
        cBx_permission->addItem(QString());
        cBx_permission->addItem(QString());
        cBx_permission->setObjectName(QString::fromUtf8("cBx_permission"));
        cBx_permission->setGeometry(QRect(170, 40, 231, 31));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        Btn_LoginEdit = new QPushButton(page_2);
        Btn_LoginEdit->setObjectName(QString::fromUtf8("Btn_LoginEdit"));
        Btn_LoginEdit->setGeometry(QRect(170, 250, 91, 31));
        Btn_LoginEdit->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_3 = new QLabel(page_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(110, 92, 54, 20));
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        LineEd_UserNameEdit = new QLineEdit(page_2);
        LineEd_UserNameEdit->setObjectName(QString::fromUtf8("LineEd_UserNameEdit"));
        LineEd_UserNameEdit->setGeometry(QRect(190, 40, 231, 31));
        LineEd_UserPasswordEdit = new QLineEdit(page_2);
        LineEd_UserPasswordEdit->setObjectName(QString::fromUtf8("LineEd_UserPasswordEdit"));
        LineEd_UserPasswordEdit->setGeometry(QRect(190, 90, 231, 31));
        LineEd_UserPasswordEdit->setEchoMode(QLineEdit::Password);
        label_4 = new QLabel(page_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(110, 42, 54, 20));
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        Btn_CancelEdit = new QPushButton(page_2);
        Btn_CancelEdit->setObjectName(QString::fromUtf8("Btn_CancelEdit"));
        Btn_CancelEdit->setGeometry(QRect(310, 250, 91, 31));
        Btn_CancelEdit->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        LineEd_UserPasswordEditC = new QLineEdit(page_2);
        LineEd_UserPasswordEditC->setObjectName(QString::fromUtf8("LineEd_UserPasswordEditC"));
        LineEd_UserPasswordEditC->setGeometry(QRect(190, 140, 231, 31));
        LineEd_UserPasswordEditC->setEchoMode(QLineEdit::Password);
        label_5 = new QLabel(page_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(110, 150, 71, 16));
        label_5->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_6 = new QLabel(page_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(113, 192, 61, 20));
        label_6->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        CBox_UserLevelEdit = new QComboBox(page_2);
        CBox_UserLevelEdit->addItem(QString());
        CBox_UserLevelEdit->addItem(QString());
        CBox_UserLevelEdit->addItem(QString());
        CBox_UserLevelEdit->setObjectName(QString::fromUtf8("CBox_UserLevelEdit"));
        CBox_UserLevelEdit->setGeometry(QRect(190, 190, 231, 31));
        CBox_UserLevelEdit->setMinimumSize(QSize(231, 31));
        stackedWidget->addWidget(page_2);

        gridLayout->addWidget(stackedWidget, 0, 0, 1, 1);


        retranslateUi(Form_Login);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Form_Login);
    } // setupUi

    void retranslateUi(QWidget *Form_Login)
    {
        Form_Login->setWindowTitle(QCoreApplication::translate("Form_Login", "\347\231\273\345\275\225", nullptr));
        Btn_Cancel->setText(QCoreApplication::translate("Form_Login", "\345\217\226\346\266\210", nullptr));
        Btn_Login->setText(QCoreApplication::translate("Form_Login", "\347\241\256\350\256\244", nullptr));
        label_2->setText(QCoreApplication::translate("Form_Login", "\345\257\206\347\240\201\357\274\232", nullptr));
        LineEd_UserName->setPlaceholderText(QCoreApplication::translate("Form_Login", "\347\224\250\346\210\267\345\220\215", nullptr));
        LineEd_UserPassword->setPlaceholderText(QCoreApplication::translate("Form_Login", "\345\257\206\347\240\201", nullptr));
        label->setText(QCoreApplication::translate("Form_Login", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        label_7->setText(QCoreApplication::translate("Form_Login", "\346\235\203\351\231\220\357\274\232", nullptr));
        cBx_permission->setItemText(0, QCoreApplication::translate("Form_Login", "\347\256\241\347\220\206\345\221\230", nullptr));
        cBx_permission->setItemText(1, QCoreApplication::translate("Form_Login", "\345\267\245\347\250\213\345\270\210", nullptr));
        cBx_permission->setItemText(2, QCoreApplication::translate("Form_Login", "\346\223\215\344\275\234\345\221\230", nullptr));

        Btn_LoginEdit->setText(QCoreApplication::translate("Form_Login", "\347\241\256\350\256\244", nullptr));
        label_3->setText(QCoreApplication::translate("Form_Login", "\345\257\206\347\240\201\357\274\232", nullptr));
        LineEd_UserNameEdit->setPlaceholderText(QCoreApplication::translate("Form_Login", "\347\224\250\346\210\267\345\220\215", nullptr));
        LineEd_UserPasswordEdit->setPlaceholderText(QCoreApplication::translate("Form_Login", "\345\257\206\347\240\201", nullptr));
        label_4->setText(QCoreApplication::translate("Form_Login", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        Btn_CancelEdit->setText(QCoreApplication::translate("Form_Login", "\345\217\226\346\266\210", nullptr));
        LineEd_UserPasswordEditC->setPlaceholderText(QCoreApplication::translate("Form_Login", "\345\257\206\347\240\201", nullptr));
        label_5->setText(QCoreApplication::translate("Form_Login", "\347\241\256\350\256\244\345\257\206\347\240\201\357\274\232", nullptr));
        label_6->setText(QCoreApplication::translate("Form_Login", "\346\235\203\351\231\220\357\274\232", nullptr));
        CBox_UserLevelEdit->setItemText(0, QCoreApplication::translate("Form_Login", "\347\256\241\347\220\206\345\221\230", nullptr));
        CBox_UserLevelEdit->setItemText(1, QCoreApplication::translate("Form_Login", "\345\267\245\347\250\213\345\270\210", nullptr));
        CBox_UserLevelEdit->setItemText(2, QCoreApplication::translate("Form_Login", "\346\223\215\344\275\234\350\200\205", nullptr));

    } // retranslateUi

};

namespace Ui {
    class Form_Login: public Ui_Form_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
