#include "userwidget.h"
#include <QLabel>
#include <QDebug>
#include <QVariant>
#include <QString>
#include <QStringList>
#include <QSqlQuery>
#include <typeinfo>

UserWidget::UserWidget(QWidget *pwgt): QWidget(pwgt) {
    userVBoxLayout = new QVBoxLayout;
    userView = new UserView;

    QLabel* userLabel = new QLabel(tr("Filter"));
    userModel = new UserTableModel;

    userVBoxLayout->addWidget(userLabel);
    userVBoxLayout->addWidget(userView);
    this->setLayout(userVBoxLayout);
}








UserWidget::~UserWidget() {

}
