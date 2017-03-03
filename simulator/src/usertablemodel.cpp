#include "usertablemodel.h"

UserTableModel::UserTableModel() {

    this->setTable("atlas.users");
    this->select();

}
