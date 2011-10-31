FormPostPlugin * posta;
QNetworkReply *reply;
QHash<QString, QString> hash;
QHash<QString, QString> mphash;
QHash<QString, QString> agendahash;
QTemporaryFile * writeTemp();
bool writeFile(QString fileName);
QWidget *progressWidget;
Ui::uploadProgress uiProgress;
bool loadFile(QString newfileName);


oid slotError(QNetworkReply::NetworkError);
        void getTakesLoginComplete();
        void progress ( qint64, qint64 );
        void takesReply();
        void takesDownload( QNetworkReply * reply );
        void postFinished();

        void takes();
        void getMPList();
        void MPListReply();
        void post();
        void replyFinished(QNetworkReply*);
        void saveFileAs();
        void saveFile();
        void openFile();
        void newFile();
        void playItem(const QModelIndex& );
        void loadNextFileSlot(int currentLogicalIndex,QString newfileName);
