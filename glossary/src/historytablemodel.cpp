#include "historytablemodel.h"
#include <QDebug>
#include <QStandardItem>
#include <QSqlError>

HistoryTableModel::HistoryTableModel() {

    this->setTable("atlas.history");
    this->select();

}
