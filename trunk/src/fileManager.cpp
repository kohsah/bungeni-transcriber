void FileManager::saveFile()
{
    if (fileName != "")
    {
        this->writeFile(fileName);
    }
    else
    {
        this->saveFileAs();
    }
}

void FileManager::saveFileAs()
{
    QString newfileName = QFileDialog::getSaveFileName(this, tr("File Save As"), "", tr("Transcript Files (*.trs);;Any File (*.*)"));
    fileName = newfileName;

    this->writeFile(newfileName);
    playlist->setTranscriptFileLocation(newfileName, -1);
}
void FileManager::openFile()
{
    QString newfileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Transcript Files (*.trs);;Any File (*.*)"));
    newFile();
    if (!this->loadFile(newfileName))
    {
        QMessageBox::warning(this, tr("Error"),"Cannot Open File");
    }
}

void FileManager::loadNextFileSlot(int currentLogicalIndex, QString newfileName)
{
    qDebug() << "Load File Slot, newfilename = " << newfileName;
    qDebug() << "Load File Slot, filename = " << fileName;
    qDebug() << "Load File Slot, currentLogicalIndex = " << currentLogicalIndex;
   /* if ((fileName == "") && (model->rowCount() > 0 ))
    {
        QMessageBox::warning(this, tr("Error"),"Please save the current transcripts before loading another");
        //this->saveFileAs();
        QString f = QFileDialog::getSaveFileName(this, tr("File Save As"), "", tr("Transcript Files (*.trs);;Any File (*.*)"));
        playlist->setTranscriptFileLocation(f, currentLogicalIndex);
        this->writeFile(f);
    }
    else if (fileName != "")
    {
        this->writeFile(fileName);
    }

    this->loadFile(newfileName); */
}

bool FileManager::loadFile(QString newfileName)
{
    model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());
    if (newfileName == "")
    {
        fileName = newfileName;
        return true;
    }
    fileName = newfileName;
    QFile newfile(newfileName);
    if (!newfile.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }
    QXmlStreamReader reader;
    reader.setDevice(&newfile);

    if (!reader.atEnd()) {
        reader.readNext();

        if (reader.isStartElement()) {
            if (reader.name() != "trs")
            {
                return false;
            }
        }
    }

    if (reader.hasError()) {
        return false;
    }

    QString str;
    QString person, startTimeTxt, endTimeTxt;
    QTime q_endTime, q_startTime;
    model->removeRows(0, model->rowCount(QModelIndex()), QModelIndex());
    int i_startTime, i_endTime;
    int row = 0;
    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.name() == "trs")
        {
            mediaLocation = reader.attributes().value("media").toString();
            sittingName = reader.attributes().value("name").toString();
        }
        else if (reader.name() == "speech")
        {

            startTimeTxt = reader.attributes().value("startTime").toString();
            endTimeTxt = reader.attributes().value("endTime").toString();

            q_startTime = QTime::fromString(startTimeTxt);
            i_startTime = q_startTime.hour() * 3600 + q_startTime.minute()*60 + q_startTime.second();

            q_endTime = QTime::fromString(endTimeTxt);
            i_endTime = q_endTime.hour() * 3600 + q_endTime.minute()*60 + q_endTime.second();

            person = reader.attributes().value("person").toString();
            if (reader.isCDATA())
            {

                str = reader.text().toString();
            }
            else
            {
                str = reader.readElementText();
            }
            qDebug() << str;
            /* model->insertRows(row, 1, QModelIndex());
            model->setData(model->index(row, 0, QModelIndex()), str);
            model->setData(model->index(row, 1, QModelIndex()), person);
            model->setData(model->index(row, 2, QModelIndex()), i_startTime);
            model->setData(model->index(row, 3, QModelIndex()), i_endTime);
            model->setData(model->index(row, 5, QModelIndex()), true); */
            row++;
        }
    }

    newfile.close();
    return true;
}

void FileManager::newFile()
{
    int ret;
    if (fileName != "")
    {
        ret = QMessageBox::warning(this, tr("Transcribe"),
                                   tr("The document has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel,
                                   QMessageBox::Save);
        switch (ret)
        {
        case QMessageBox::Save:
            saveFile();
            setupModelView();
            break;
        case QMessageBox::Discard:
            setupModelView();
            break;
        case QMessageBox::Cancel:
            break;
        }
    }
    else
    {
        setupModelView();
    }


}

bool FileManager::writeFile(QString fileName)
{
    QXmlStreamWriter writer;
    QFile newfile(fileName);
    if (!newfile.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("error"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(newfile.errorString()));
        return false;
    }
    writer.setDevice(&newfile);
    writer.writeStartDocument();
    writer.writeDTD("<!DOCTYPE trs>");
    writer.writeStartElement("trs");
    writer.writeAttribute("media", playlist->getMediaLocation());
    qDebug() << "Media = " << playlist->getMediaLocation();
    writer.writeAttribute("name", playlist->getSittingName());
    qDebug() << "Sitting = " << playlist->getSittingName();
    for (int i = 0; i < model->rowCount(QModelIndex()); ++i)
    {
        bool type = model->index( i , 5).data(Qt::DisplayRole).toBool();
        qDebug() << "Type, Speech or not" << type;
        if (type == true)
        {
            writer.writeStartElement("speech");
            writer.writeAttribute("person", model->data(model->index(i, 1, QModelIndex()),Qt::DisplayRole).toString());
            QString name = model->data(model->index(i, 1, QModelIndex()),Qt::DisplayRole).toString();
            if (mphash.value(name, "") != "")
            {
                writer.writeAttribute("person_id",mphash.value(name, ""));
            }
            else
            {
                writer.writeAttribute("person_id", "");
            }
            writer.writeAttribute("startTime",
                                  timeSecondstoString(model->data(model->index(i,
                                                                               2, QModelIndex()),Qt::DisplayRole).toInt()));
            writer.writeAttribute("endTime",
                                  timeSecondstoString(model->data(model->index(i,
                                                                               3, QModelIndex()),Qt::DisplayRole).toInt()));
            writer.writeCDATA(model->data(model->index(i, 0, QModelIndex()),Qt::DisplayRole).toString());
            //writer.writeCharacters( QDomDocument::createCDATASection ( model->data(model->index(i, 0, QModelIndex()),Qt::DisplayRole).toString() ) );
            writer.writeEndElement();
        }
        else
        {
            writer.writeStartElement("agenda_item");
            writer.writeAttribute("startTime",
                                  timeSecondstoString(model->data(model->index(i,
                                                                               2, QModelIndex()),Qt::DisplayRole).toInt()));
            writer.writeAttribute("endTime",
                                  timeSecondstoString(model->data(model->index(i,
                                                                               3, QModelIndex()),Qt::DisplayRole).toInt()));
            writer.writeAttribute("id",
                                  FileManager::model->data(model->index(i,
                                                                             6, QModelIndex()),Qt::DisplayRole).toString());
            writer.writeCDATA(model->data(model->index(i, 7, QModelIndex()),Qt::DisplayRole).toString());
            writer.writeEndElement();
        }
    }
    writer.writeEndDocument();
    newfile.close();
    return true;
}

QTemporaryFile * FileManager::writeTemp()
{
    QXmlStreamWriter writer;
    QTemporaryFile * file = new QTemporaryFile();
    if (!file->open()) {
        QMessageBox::warning(this, tr("error"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file->errorString()));
        return NULL;
    }
    qDebug( "temp file opened" );
    writer.setDevice(file);
    writer.writeStartDocument();
    writer.writeDTD("<!DOCTYPE trs>");
    writer.writeStartElement("trs");
    for (int i = 0; i < model->rowCount(QModelIndex()); ++i)
    {
        writer.writeStartElement("speech");
        writer.writeAttribute("person", model->data(model->index(i, 1, QModelIndex()),Qt::DisplayRole).toString());
        writer.writeAttribute("startTime", timeSecondstoString(model->data(model->index(i, 2, QModelIndex()),Qt::DisplayRole).toInt()));
        writer.writeAttribute("endTime", timeSecondstoString(model->data(model->index(i, 3, QModelIndex()),Qt::DisplayRole).toInt()));
        writer.writeCharacters(model->data(model->index(i, 0, QModelIndex()),Qt::DisplayRole).toString());
        writer.writeEndElement();
    }
    writer.writeEndDocument();
    qDebug( "file written" );
    file->close();
    return file;
}
