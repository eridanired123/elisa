/*
 * Copyright 2015-2018 Matthieu Gallien <matthieu_gallien@yahoo.fr>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "databasetestdata.h"

#include "databaseinterface.h"
#include "models/datamodel.h"
#include "models/alltracksproxymodel.h"

#include <QObject>
#include <QUrl>
#include <QString>
#include <QHash>
#include <QVector>
#include <QThread>
#include <QMetaObject>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QAbstractItemModelTester>

#include <QDebug>

#include <QtTest>

class AllTracksProxyModelTests: public QObject, public DatabaseTestData
{
    Q_OBJECT

public:

    explicit AllTracksProxyModelTests(QObject *aParent = nullptr) : QObject(aParent)
    {
    }

private Q_SLOTS:

    void initTestCase()
    {
        qRegisterMetaType<QHash<qulonglong,int>>("QHash<qulonglong,int>");
        qRegisterMetaType<QHash<QString,QUrl>>("QHash<QString,QUrl>");
        qRegisterMetaType<QVector<qlonglong>>("QVector<qlonglong>");
        qRegisterMetaType<QHash<qlonglong,int>>("QHash<qlonglong,int>");
    }

    void removeOneTrack()
    {
        DatabaseInterface musicDb;
        DataModel tracksModel;
        QAbstractItemModelTester testModel(&tracksModel);
        AllTracksProxyModel proxyTracksModel;
        QAbstractItemModelTester proxyTestModel(&proxyTracksModel);
        proxyTracksModel.setSourceModel(&tracksModel);

        connect(&musicDb, &DatabaseInterface::tracksAdded,
                &tracksModel, &DataModel::tracksAdded);
        connect(&musicDb, &DatabaseInterface::trackModified,
                &tracksModel, &DataModel::trackModified);
        connect(&musicDb, &DatabaseInterface::trackRemoved,
                &tracksModel, &DataModel::trackRemoved);

        musicDb.init(QStringLiteral("testDb"));

        QSignalSpy beginInsertRowsSpy(&proxyTracksModel, &DataModel::rowsAboutToBeInserted);
        QSignalSpy endInsertRowsSpy(&proxyTracksModel, &DataModel::rowsInserted);
        QSignalSpy beginRemoveRowsSpy(&proxyTracksModel, &DataModel::rowsAboutToBeRemoved);
        QSignalSpy endRemoveRowsSpy(&proxyTracksModel, &DataModel::rowsRemoved);
        QSignalSpy dataChangedSpy(&proxyTracksModel, &DataModel::dataChanged);

        QCOMPARE(beginInsertRowsSpy.count(), 0);
        QCOMPARE(endInsertRowsSpy.count(), 0);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        tracksModel.initialize(nullptr, nullptr, ElisaUtils::Track, ElisaUtils::NoFilter, {}, {}, 0);

        QCOMPARE(beginInsertRowsSpy.count(), 0);
        QCOMPARE(endInsertRowsSpy.count(), 0);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        musicDb.insertTracksList(mNewTracks, mNewCovers);

        QCOMPARE(beginInsertRowsSpy.count(), 1);
        QCOMPARE(endInsertRowsSpy.count(), 1);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        QCOMPARE(proxyTracksModel.rowCount(), 23);

        auto trackId = musicDb.trackIdFromTitleAlbumTrackDiscNumber(QStringLiteral("track1"), QStringLiteral("artist1"),
                                                                    QStringLiteral("album1"), 1, 1);

        auto firstTrack = musicDb.trackDataFromDatabaseId(trackId);

        musicDb.removeTracksList({firstTrack[DataTypes::ResourceRole].toUrl()});

        QCOMPARE(beginInsertRowsSpy.count(), 1);
        QCOMPARE(endInsertRowsSpy.count(), 1);
        QCOMPARE(beginRemoveRowsSpy.count(), 1);
        QCOMPARE(endRemoveRowsSpy.count(), 1);
        QCOMPARE(dataChangedSpy.count(), 0);

        QCOMPARE(proxyTracksModel.rowCount(), 22);
    }

    void removeOneAlbum()
    {
        DatabaseInterface musicDb;
        DataModel tracksModel;
        QAbstractItemModelTester testModel(&tracksModel);
        AllTracksProxyModel proxyTracksModel;
        QAbstractItemModelTester proxyTestModel(&proxyTracksModel);
        proxyTracksModel.setSourceModel(&tracksModel);

        connect(&musicDb, &DatabaseInterface::tracksAdded,
                &tracksModel, &DataModel::tracksAdded);
        connect(&musicDb, &DatabaseInterface::trackModified,
                &tracksModel, &DataModel::trackModified);
        connect(&musicDb, &DatabaseInterface::trackRemoved,
                &tracksModel, &DataModel::trackRemoved);

        musicDb.init(QStringLiteral("testDb"));

        QSignalSpy beginInsertRowsSpy(&proxyTracksModel, &DataModel::rowsAboutToBeInserted);
        QSignalSpy endInsertRowsSpy(&proxyTracksModel, &DataModel::rowsInserted);
        QSignalSpy beginRemoveRowsSpy(&proxyTracksModel, &DataModel::rowsAboutToBeRemoved);
        QSignalSpy endRemoveRowsSpy(&proxyTracksModel, &DataModel::rowsRemoved);
        QSignalSpy dataChangedSpy(&proxyTracksModel, &DataModel::dataChanged);

        QCOMPARE(beginInsertRowsSpy.count(), 0);
        QCOMPARE(endInsertRowsSpy.count(), 0);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        tracksModel.initialize(nullptr, nullptr, ElisaUtils::Track, ElisaUtils::NoFilter, {}, {}, 0);

        QCOMPARE(beginInsertRowsSpy.count(), 0);
        QCOMPARE(endInsertRowsSpy.count(), 0);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        musicDb.insertTracksList(mNewTracks, mNewCovers);

        QCOMPARE(beginInsertRowsSpy.count(), 1);
        QCOMPARE(endInsertRowsSpy.count(), 1);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        QCOMPARE(proxyTracksModel.rowCount(), 23);

        auto firstTrackId = musicDb.trackIdFromTitleAlbumTrackDiscNumber(QStringLiteral("track1"), QStringLiteral("artist2"),
                                                                         QStringLiteral("album3"), 1, 1);
        auto firstTrack = musicDb.trackDataFromDatabaseId(firstTrackId);
        auto secondTrackId = musicDb.trackIdFromTitleAlbumTrackDiscNumber(QStringLiteral("track2"), QStringLiteral("artist2"),
                                                                          QStringLiteral("album3"), 2, 1);
        auto secondTrack = musicDb.trackDataFromDatabaseId(secondTrackId);
        auto thirdTrackId = musicDb.trackIdFromTitleAlbumTrackDiscNumber(QStringLiteral("track3"), QStringLiteral("artist2"),
                                                                         QStringLiteral("album3"), 3, 1);
        auto thirdTrack = musicDb.trackDataFromDatabaseId(thirdTrackId);

        musicDb.removeTracksList({firstTrack[DataTypes::ResourceRole].toUrl()
                                  ,
                                  secondTrack[DataTypes::ResourceRole].toUrl(),
                                  thirdTrack[DataTypes::ResourceRole].toUrl()});

        QCOMPARE(beginInsertRowsSpy.count(), 1);
        QCOMPARE(endInsertRowsSpy.count(), 1);
        QCOMPARE(beginRemoveRowsSpy.count(), 3);
        QCOMPARE(endRemoveRowsSpy.count(), 3);
        QCOMPARE(dataChangedSpy.count(), 0);

        QCOMPARE(proxyTracksModel.rowCount(), 20);
    }

    void addOneTrack()
    {
        DatabaseInterface musicDb;
        DataModel tracksModel;
        QAbstractItemModelTester testModel(&tracksModel);
        AllTracksProxyModel proxyTracksModel;
        QAbstractItemModelTester proxyTestModel(&proxyTracksModel);
        proxyTracksModel.setSourceModel(&tracksModel);

        connect(&musicDb, &DatabaseInterface::tracksAdded,
                &tracksModel, &DataModel::tracksAdded);
        connect(&musicDb, &DatabaseInterface::trackModified,
                &tracksModel, &DataModel::trackModified);
        connect(&musicDb, &DatabaseInterface::trackRemoved,
                &tracksModel, &DataModel::trackRemoved);

        musicDb.init(QStringLiteral("testDb"));

        QSignalSpy beginInsertRowsSpy(&proxyTracksModel, &DataModel::rowsAboutToBeInserted);
        QSignalSpy endInsertRowsSpy(&proxyTracksModel, &DataModel::rowsInserted);
        QSignalSpy beginRemoveRowsSpy(&proxyTracksModel, &DataModel::rowsAboutToBeRemoved);
        QSignalSpy endRemoveRowsSpy(&proxyTracksModel, &DataModel::rowsRemoved);
        QSignalSpy dataChangedSpy(&proxyTracksModel, &DataModel::dataChanged);

        QCOMPARE(beginInsertRowsSpy.count(), 0);
        QCOMPARE(endInsertRowsSpy.count(), 0);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        tracksModel.initialize(nullptr, nullptr, ElisaUtils::Track, ElisaUtils::NoFilter, {}, {}, 0);

        QCOMPARE(beginInsertRowsSpy.count(), 0);
        QCOMPARE(endInsertRowsSpy.count(), 0);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        musicDb.insertTracksList(mNewTracks, mNewCovers);

        QCOMPARE(beginInsertRowsSpy.count(), 1);
        QCOMPARE(endInsertRowsSpy.count(), 1);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        QCOMPARE(proxyTracksModel.rowCount(), 23);

        auto newTrack = DataTypes::TrackDataType{true, QStringLiteral("$23"), QStringLiteral("0"), QStringLiteral("track23"),
                QStringLiteral("artist2"), QStringLiteral("album4"), QStringLiteral("artist2"), 23, 1, QTime::fromMSecsSinceStartOfDay(23),
        {QUrl::fromLocalFile(QStringLiteral("/$23"))},
                QDateTime::fromMSecsSinceEpoch(23),
        {QUrl::fromLocalFile(QStringLiteral("file://image$23"))}, 5, true,
        {}, QStringLiteral("composer1"), QStringLiteral("lyricist1"), false};
        auto newTracks = DataTypes::ListTrackDataType();
        newTracks.push_back(newTrack);

        musicDb.insertTracksList(newTracks, mNewCovers);

        QCOMPARE(beginInsertRowsSpy.count(), 2);
        QCOMPARE(endInsertRowsSpy.count(), 2);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 5);

        QCOMPARE(proxyTracksModel.rowCount(), 24);
    }

    void addOneAlbum()
    {
        DatabaseInterface musicDb;
        DataModel tracksModel;
        QAbstractItemModelTester testModel(&tracksModel);
        AllTracksProxyModel proxyTracksModel;
        QAbstractItemModelTester proxyTestModel(&proxyTracksModel);
        proxyTracksModel.setSourceModel(&tracksModel);

        connect(&musicDb, &DatabaseInterface::tracksAdded,
                &tracksModel, &DataModel::tracksAdded);
        connect(&musicDb, &DatabaseInterface::trackModified,
                &tracksModel, &DataModel::trackModified);
        connect(&musicDb, &DatabaseInterface::trackRemoved,
                &tracksModel, &DataModel::trackRemoved);

        musicDb.init(QStringLiteral("testDb"));

        QSignalSpy beginInsertRowsSpy(&proxyTracksModel, &DataModel::rowsAboutToBeInserted);
        QSignalSpy endInsertRowsSpy(&proxyTracksModel, &DataModel::rowsInserted);
        QSignalSpy beginRemoveRowsSpy(&proxyTracksModel, &DataModel::rowsAboutToBeRemoved);
        QSignalSpy endRemoveRowsSpy(&proxyTracksModel, &DataModel::rowsRemoved);
        QSignalSpy dataChangedSpy(&proxyTracksModel, &DataModel::dataChanged);

        QCOMPARE(beginInsertRowsSpy.count(), 0);
        QCOMPARE(endInsertRowsSpy.count(), 0);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        tracksModel.initialize(nullptr, nullptr, ElisaUtils::Track, ElisaUtils::NoFilter, {}, {}, 0);

        QCOMPARE(beginInsertRowsSpy.count(), 0);
        QCOMPARE(endInsertRowsSpy.count(), 0);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        auto newFiles = QList<QUrl>();
        const auto &constNewTracks = mNewTracks;
        for (const auto &oneTrack : constNewTracks) {
            newFiles.push_back(oneTrack.resourceURI());
        }

        musicDb.insertTracksList(mNewTracks, mNewCovers);

        QCOMPARE(beginInsertRowsSpy.count(), 1);
        QCOMPARE(endInsertRowsSpy.count(), 1);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        QCOMPARE(proxyTracksModel.rowCount(), 23);

        auto newTrack = DataTypes::TrackDataType{true, QStringLiteral("$23"), QStringLiteral("0"), QStringLiteral("track23"),
                QStringLiteral("artist2"), QStringLiteral("album4"), QStringLiteral("artist2"), 23, 1, QTime::fromMSecsSinceStartOfDay(23),
        {QUrl::fromLocalFile(QStringLiteral("/$23"))},
                QDateTime::fromMSecsSinceEpoch(23),
        {QUrl::fromLocalFile(QStringLiteral("file://image$23"))}, 5, true,
        {}, QStringLiteral("composer1"), QStringLiteral("lyricist1"), false};
        auto newTracks = DataTypes::ListTrackDataType();
        newTracks.push_back(newTrack);

        auto newCover = QUrl::fromLocalFile(QStringLiteral("album5"));
        auto newCovers = QHash<QString, QUrl>();
        newCovers[QStringLiteral("album5")] = newCover;

        auto newFiles2 = QList<QUrl>();
        for (const auto &oneTrack : newTracks) {
            newFiles2.push_back(oneTrack.resourceURI());
        }

        musicDb.insertTracksList(newTracks, newCovers);

        QCOMPARE(beginInsertRowsSpy.count(), 2);
        QCOMPARE(endInsertRowsSpy.count(), 2);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 5);

        QCOMPARE(proxyTracksModel.rowCount(), 24);
    }

    void modifyOneTrack()
    {
        DatabaseInterface musicDb;
        DataModel tracksModel;
        QAbstractItemModelTester testModel(&tracksModel);
        AllTracksProxyModel proxyTracksModel;
        QAbstractItemModelTester proxyTestModel(&proxyTracksModel);
        proxyTracksModel.setSourceModel(&tracksModel);

        connect(&musicDb, &DatabaseInterface::tracksAdded,
                &tracksModel, &DataModel::tracksAdded);
        connect(&musicDb, &DatabaseInterface::trackModified,
                &tracksModel, &DataModel::trackModified);
        connect(&musicDb, &DatabaseInterface::trackRemoved,
                &tracksModel, &DataModel::trackRemoved);

        musicDb.init(QStringLiteral("testDb"));

        QSignalSpy beginInsertRowsSpy(&proxyTracksModel, &DataModel::rowsAboutToBeInserted);
        QSignalSpy endInsertRowsSpy(&proxyTracksModel, &DataModel::rowsInserted);
        QSignalSpy beginRemoveRowsSpy(&proxyTracksModel, &DataModel::rowsAboutToBeRemoved);
        QSignalSpy endRemoveRowsSpy(&proxyTracksModel, &DataModel::rowsRemoved);
        QSignalSpy dataChangedSpy(&proxyTracksModel, &DataModel::dataChanged);

        QCOMPARE(beginInsertRowsSpy.count(), 0);
        QCOMPARE(endInsertRowsSpy.count(), 0);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        tracksModel.initialize(nullptr, nullptr, ElisaUtils::Track, ElisaUtils::NoFilter, {}, {}, 0);

        QCOMPARE(beginInsertRowsSpy.count(), 0);
        QCOMPARE(endInsertRowsSpy.count(), 0);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        musicDb.insertTracksList(mNewTracks, mNewCovers);

        QCOMPARE(beginInsertRowsSpy.count(), 1);
        QCOMPARE(endInsertRowsSpy.count(), 1);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        QCOMPARE(proxyTracksModel.rowCount(), 23);

        auto newTrack = DataTypes::TrackDataType{true, QStringLiteral("$1"), QStringLiteral("0"), QStringLiteral("track1"),
                QStringLiteral("artist1"), QStringLiteral("album1"), QStringLiteral("Various Artists"),
                1, 1, QTime::fromMSecsSinceStartOfDay(1), {QUrl::fromLocalFile(QStringLiteral("/$1"))},
                QDateTime::fromMSecsSinceEpoch(1),
        {QUrl::fromLocalFile(QStringLiteral("file://image$1"))}, 5, true, {}, QStringLiteral("composer1"), QStringLiteral("lyricist1"), false};
        auto newTracks = DataTypes::ListTrackDataType();
        newTracks.push_back(newTrack);

        musicDb.insertTracksList(newTracks, mNewCovers);

        QCOMPARE(beginInsertRowsSpy.count(), 1);
        QCOMPARE(endInsertRowsSpy.count(), 1);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 1);

        QCOMPARE(proxyTracksModel.rowCount(), 23);

        const auto &dataChangedSignal =  dataChangedSpy.constFirst();

        QCOMPARE(dataChangedSignal.size(), 3);

        auto changedIndex = dataChangedSignal.constFirst().toModelIndex();

        QCOMPARE(proxyTracksModel.data(changedIndex, DataTypes::ColumnsRoles::RatingRole).isValid(), true);
        QCOMPARE(proxyTracksModel.data(changedIndex, DataTypes::ColumnsRoles::RatingRole).toInt(), 5);
    }

    void addEmptyTracksList()
    {
        DataModel tracksModel;
        QAbstractItemModelTester testModel(&tracksModel);
        AllTracksProxyModel proxyTracksModel;
        QAbstractItemModelTester proxyTestModel(&proxyTracksModel);
        proxyTracksModel.setSourceModel(&tracksModel);

        auto newTracks = DataModel::ListTrackDataType{};

        QSignalSpy beginInsertRowsSpy(&proxyTracksModel, &DataModel::rowsAboutToBeInserted);
        QSignalSpy endInsertRowsSpy(&proxyTracksModel, &DataModel::rowsInserted);
        QSignalSpy beginRemoveRowsSpy(&proxyTracksModel, &DataModel::rowsAboutToBeRemoved);
        QSignalSpy endRemoveRowsSpy(&proxyTracksModel, &DataModel::rowsRemoved);
        QSignalSpy dataChangedSpy(&proxyTracksModel, &DataModel::dataChanged);

        QCOMPARE(beginInsertRowsSpy.count(), 0);
        QCOMPARE(endInsertRowsSpy.count(), 0);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        tracksModel.tracksAdded(newTracks);

        QCOMPARE(beginInsertRowsSpy.count(), 0);
        QCOMPARE(endInsertRowsSpy.count(), 0);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        QCOMPARE(proxyTracksModel.rowCount(), 0);
    }

    void addTracksListTwice()
    {
        DatabaseInterface musicDb;
        DataModel tracksModel;
        QAbstractItemModelTester testModel(&tracksModel);
        AllTracksProxyModel proxyTracksModel;
        QAbstractItemModelTester proxyTestModel(&proxyTracksModel);
        proxyTracksModel.setSourceModel(&tracksModel);

        connect(&musicDb, &DatabaseInterface::tracksAdded,
                &tracksModel, &DataModel::tracksAdded);
        connect(&musicDb, &DatabaseInterface::trackModified,
                &tracksModel, &DataModel::trackModified);
        connect(&musicDb, &DatabaseInterface::trackRemoved,
                &tracksModel, &DataModel::trackRemoved);

        musicDb.init(QStringLiteral("testDb"));

        QSignalSpy beginInsertRowsSpy(&proxyTracksModel, &DataModel::rowsAboutToBeInserted);
        QSignalSpy endInsertRowsSpy(&proxyTracksModel, &DataModel::rowsInserted);
        QSignalSpy beginRemoveRowsSpy(&proxyTracksModel, &DataModel::rowsAboutToBeRemoved);
        QSignalSpy endRemoveRowsSpy(&proxyTracksModel, &DataModel::rowsRemoved);
        QSignalSpy dataChangedSpy(&proxyTracksModel, &DataModel::dataChanged);

        QCOMPARE(beginInsertRowsSpy.count(), 0);
        QCOMPARE(endInsertRowsSpy.count(), 0);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        tracksModel.initialize(nullptr, nullptr, ElisaUtils::Track, ElisaUtils::NoFilter, {}, {}, 0);

        QCOMPARE(beginInsertRowsSpy.count(), 0);
        QCOMPARE(endInsertRowsSpy.count(), 0);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        musicDb.insertTracksList(mNewTracks, mNewCovers);

        QCOMPARE(beginInsertRowsSpy.count(), 1);
        QCOMPARE(endInsertRowsSpy.count(), 1);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        QCOMPARE(proxyTracksModel.rowCount(), 23);

        auto newTrack = DataTypes::TrackDataType{true, QStringLiteral("$23"), QStringLiteral("0"), QStringLiteral("track23"),
                QStringLiteral("artist2"), QStringLiteral("album4"), QStringLiteral("artist2"), 23, 1, QTime::fromMSecsSinceStartOfDay(23),
        {QUrl::fromLocalFile(QStringLiteral("/$23"))},
                QDateTime::fromMSecsSinceEpoch(23),
        {QUrl::fromLocalFile(QStringLiteral("file://image$23"))}, 5, true, {}, QStringLiteral("composer1"), QStringLiteral("lyricist1"), false};

        auto newTracks = DataTypes::ListTrackDataType();
        newTracks.push_back(newTrack);

        musicDb.insertTracksList(newTracks, mNewCovers);

        QCOMPARE(beginInsertRowsSpy.count(), 2);
        QCOMPARE(endInsertRowsSpy.count(), 2);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 5);

        QCOMPARE(proxyTracksModel.rowCount(), 24);
    }
};

QTEST_GUILESS_MAIN(AllTracksProxyModelTests)


#include "alltracksproxymodeltest.moc"
