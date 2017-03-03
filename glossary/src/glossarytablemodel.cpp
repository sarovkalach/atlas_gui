#include "glossarytablemodel.h"
#include <QDebug>
#include <QSqlTableModel>

GlossaryTableModel::GlossaryTableModel()  {

    this->setTable("atlas.glossary");
    this->setHeaderData(1, Qt::Horizontal,tr("Notion"));
    this->setHeaderData(2, Qt::Horizontal,tr("Description"));
    this->select();
}
