void Rest::post()
{
    if (fileName != "")
    {
        this->saveFile();
    }
    qDebug() << "POSTING TAKES";
    QString string;
    QXmlStreamWriter writer(&string);
    writer.writeStartDocument();
    writer.writeDTD("<!DOCTYPE trs>");
    writer.writeStartElement("trs");

    QHashIterator<QString, QString> i(hash);
    QStandardItemModel * localmodel = playlist->getModel();
    int c=0;
    while (c <= localmodel->rowCount()){
        QString takeName = localmodel->data(localmodel->index(c, 0)).toString();
        QString sittingID = takeName.section("-", -1, -1);
        QString endTime = takeName.section("-", -2, -2);
        QString startTime = takeName.section("-", -3, -3);
        QString trsfileName = localmodel->data(localmodel->index(c, 2)).toString();
        QFile newfile(trsfileName);
        writer.writeStartElement("take");
        writer.writeAttribute("sittingID", sittingID);
        writer.writeAttribute("startTime", startTime);
        writer.writeAttribute("endTime", endTime);
        if (newfile.open(QFile::ReadOnly | QFile::Text)) {
            QXmlStreamReader reader;
            reader.setDevice(&newfile);
            while (!reader.atEnd()) {
                reader.readNext();
                if ((reader.name() == "speech") || (reader.name() == "agenda_item"))
                {
                    writer.writeCurrentToken(reader);
                    writer.writeCharacters(reader.readElementText());
                    writer.writeEndElement();
                }
            }
        }
        writer.writeEndElement();
        newfile.close();
        c++;
    }
    writer.writeEndElement();
    writer.writeEndDocument();
    qDebug() << string;

    FormPostPlugin * post = new FormPostPlugin();

    QSettings settings("transcribe.conf", QSettings::IniFormat);
    settings.beginGroup("Network");
    QString str = "http://"+settings.value("hostname").toString()+":"+settings.value("port").toString()+"/speechxml";
    post->addField("form.xml", string);
    post->addField("form.actions.post", "Post");
    settings.endGroup();
    QNetworkReply *reply = post->postData(str);

    connect( reply, SIGNAL(finished()), this, SLOT(postFinished()) );
}

void Rest::postFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QByteArray ba = reply->readAll();
    qDebug() << ba;
    qDebug() << reply->url();
    QMessageBox::information(0, "Success", "Speeches successfully posted to Bungeni");
}



void Rest::takes()
{
    QSettings settings("transcribe.conf", QSettings::IniFormat);
    settings.beginGroup("Network");
    QString name = settings.value("username").toString();
    qDebug() << "Name : " << name;
    QString password = settings.value("password").toString();
    QString url = "http://"+settings.value("hostname").toString()+":"+settings.value("port").toString()+"/login";
    qDebug() << "url : " << url;
    posta = new FormPostPlugin();
    posta->addField("login", name);
    posta->addField("password", password);
    posta->addField("actions.login", "Login");
    reply = posta->postData(url);
    connect( reply, SIGNAL(finished()), this, SLOT(getTakesLoginComplete()) );
    connect( reply, SIGNAL(finished()), this, SLOT(getMPList()) );
    settings.endGroup();
}

void Rest::getMPList()
{
    QNetworkRequest request;
    QSettings settings("transcribe.conf", QSettings::IniFormat);
    settings.beginGroup("Network");
    QString url = "http://"+settings.value("hostname").toString()+":"+settings.value("port").toString()+"/mpslistxml";
    request.setUrl(QUrl(url));
    qDebug() << url;
    reply = posta->http->get(request);
    connect( reply, SIGNAL(finished()), this, SLOT(MPListReply()) );
    settings.endGroup();
}

void Rest::MPListReply()
{
    QXmlStreamReader reader;
    QByteArray data = posta->response();
    reader.addData(data);
    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.name() == "mp")
        {
            QString MPName = reader.attributes().value("name").toString();
            QString MPid = reader.attributes().value("id").toString();
            if ((MPName != "") && (MPid != ""))
            {
                mphash.insert(MPName, MPid);
            }
        }
    }
    QHashIterator<QString, QString> i(mphash);
    QStringList mplist;
    while (i.hasNext()) {
        i.next();
        mplist.append(i.key());
    }
    delegate->setMPList(mplist);
}

void Rest::getTakesLoginComplete()
{
    QByteArray data = posta->response();
    QNetworkRequest request;
    QSettings settings("transcribe.conf", QSettings::IniFormat);
    settings.beginGroup("Network");
    QString url = "http://"+settings.value("hostname").toString()+":"+settings.value("port").toString()+"/takesxml";
    request.setUrl(QUrl(url));
    qDebug() << url;
    reply = posta->http->get(request);
    connect( reply, SIGNAL(finished()), this, SLOT(takesReply()) );
    settings.endGroup();
}

void Rest::takesReply()
{
    QByteArray data = posta->response();
    qDebug() << "Assignments" << data;
    QXmlStreamReader reader;
    QString sittingName;
    reader.addData(data);
    QStringList files;
    QString fileURL = "";
    QString sittingID = "";
    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.name() == "assignment")
        {
            //mediaLocation = reader.attributes().value("media").toString();
            qDebug() << "assignment";
        }
        else if (reader.name() == "sitting")
        {
            sittingName = reader.attributes().value("name").toString();
            sittingID = reader.attributes().value("id").toString();
            fileURL = reader.attributes().value("file").toString();
            //qDebug() << streamName << "streams";
            qDebug() << "sitting" << fileURL;
        }
        else if (reader.name() == "take")
        {
            QString startTime = reader.attributes().value("startTime").toString();
            QString endTime = reader.attributes().value("endTime").toString();


            if ((fileURL != "") && (startTime != "") && (endTime != ""))
            {
                //files.insert(files.size(),take);
                QString takeName = sittingName + "-" + startTime + "-" + endTime + "-" +sittingID;
                QString takeURL = fileURL + "?t=" + startTime + "/" + endTime;
                qDebug() << "Take Name = " << takeName << ", Take URL" << takeURL;
                hash.insert(takeName, takeURL);
            }

        }
        else if (reader.name() == "agenda_item")
        {
            QString agenda_item_title = reader.attributes().value("title").toString();
            QString agenda_item_id = reader.attributes().value("id").toString();
            if ((agenda_item_title != "") && (agenda_item_id != ""))
            {
                agendahash.insert(agenda_item_title, agenda_item_id);
            }
        }
    }
    // QHash<QString, QString> hash;
    QNetworkRequest request;
    QNetworkAccessManager networkmanager;
    QEventLoop q;
    QTimer tT;

    tT.setSingleShot(true);
    connect(&tT, SIGNAL(timeout()), &q, SLOT(quit()));
    connect( &networkmanager, SIGNAL(finished( QNetworkReply* )), this, SLOT(takesDownload( QNetworkReply* )) );
    connect(&networkmanager, SIGNAL(finished(QNetworkReply*)), &q, SLOT(quit()));

    QHashIterator<QString, QString> a(agendahash);
    QStringList agendalist;
    while (a.hasNext()) {
        a.next();
        agendalist.append(a.key());
    }
    delegate->setAgendaList(agendalist);

    QHashIterator<QString, QString> i(hash);
    QProgressDialog progress("Downloading Takes", "Abort", 0, hash.size(), this);
    progress.setWindowModality(Qt::WindowModal);
    int counter = 1;
    // playlist_Clear(THEPL, true);
    playlist->clearPlaylist();
    model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());
    progress.setValue(0);
    while (i.hasNext()) {
        i.next();


        if (progress.wasCanceled())
            break;
        request.setUrl(QUrl(i.value()));
        qDebug() << i.value();
        networkmanager.get(request);
        //connect( reply, SIGNAL(finished()), this, SLOT(takesDownload()) );
        tT.start(120000); // 120s timeout
        q.exec();
        if(tT.isActive()){
            // download complete
            tT.stop();
        } else {
            qDebug() << "Time out: Please check your network connection";
            break;
        }
        progress.setValue(counter);
        counter++;
    }
    progress.setValue(hash.size());
    /*
    foreach( QString file, files )
    {
        //playlist_Add( );
        //qDebug() << file;
        QNetworkRequest request;
        QNetworkAccessManager *networkmanager = new QNetworkAccessManager();
        request.setUrl(QUrl(file));
        networkmanager->get(request);
        //connect( reply, SIGNAL(finished()), this, SLOT(takesDownload()) );
        connect( networkmanager, SIGNAL(finished( QNetworkReply* )), this, SLOT(takesDownload( QNetworkReply* )) );
    }
    */

}

void Rest::takesDownload( QNetworkReply * reply )
{
    QByteArray data = reply->readAll();
    QString url = reply->url().toString();
    //qDebug() << url;
    QString fileName = hash.key(url);
    QFile file("tmp/"+fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "error opening media file";
    }else
    {
        // QDataStream out(&file);
        //out << data;
        file.write(data);
    }
    file.flush();
    file.close();
    QFileInfo fileInfo(file);
    QByteArray absfilepath = fileInfo.absoluteFilePath().toAscii();
    QString trsfile = "tmp/"+fileName + ".trs";
    playlist->addTakeToPlaylist(fileName, absfilepath, trsfile);
    qDebug() << "File has been downloaded -> " << fileName;
}


void Rest::replyFinished(QNetworkReply* reply)
{
    QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    // "200 OK" received?
    if (statusCodeV.toInt()==200)
    {
        QMessageBox::warning(0, "Get Worked", "Get Worked");
    }
    // Some http error or redirect
    else
    {
        QMessageBox::warning(0, "Error", "Error");
    }

    delete reply;
}

void Rest::slotError(QNetworkReply::NetworkError error)
{
    QString errorValue;
    QMetaObject meta = QNetworkReply::staticMetaObject;
    for (int i=0; i < meta.enumeratorCount(); ++i) {
        QMetaEnum m = meta.enumerator(i);
        if (m.name() == QLatin1String("NetworkError")) {
            errorValue = QLatin1String(m.valueToKey(error));
            break;
        }
    }
    progressWidget->hide();
    QMessageBox::warning(0, "Upload Error", "Error code:"+errorValue);
}

void Rest::progress ( qint64 bytesSent, qint64 bytesTotal )
{
    // int value = (int)((float) bytesSent / (float) bytesTotal * 100);
    // uiProgress.progressBar->setValue(value);
}
