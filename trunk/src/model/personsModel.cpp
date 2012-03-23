PersonsModel(QObject *parent = 0){

}

QVariant PersonsModel::data(const QModelIndex &index, int role) const{

}

Qt::ItemFlags PersonsModel::flags(const QModelIndex &index) const{

}

QVariant PersonsModel::headerData(int section, Qt::Orientation orientation,
                     int role = Qt::DisplayRole) const{

}

QModelIndex PersonsModel::index(int row, int column,
                   const QModelIndex &parent = QModelIndex()) const{

}

QModelIndex parent(const QModelIndex &index) const{

}

int rowCount(const QModelIndex &parent = QModelIndex()) const{

}

int columnCount(const QModelIndex &parent = QModelIndex()) const{

}

void insertItem(QModelIndex &parent, PlaylistItem *item){

}
