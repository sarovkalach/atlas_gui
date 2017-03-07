#include "bookmarktablemodel.h"





BookmarkTableModel::BookmarkTableModel() {
    this->setTable("atlas.bookmark");
    this->select();
}
