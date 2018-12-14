/*
 * Copyright 2018 Matthieu Gallien <matthieu_gallien@yahoo.fr>
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

#ifndef MODELDATALOADER_H
#define MODELDATALOADER_H

#include <QObject>

#include "elisautils.h"
#include "databaseinterface.h"

class ModelDataLoader : public QObject
{

    Q_OBJECT

public:

    using ListAlbumDataType = DatabaseInterface::ListAlbumDataType;
    using ListArtistDataType = DatabaseInterface::ListArtistDataType;
    using ListGenreDataType = DatabaseInterface::ListGenreDataType;
    using ListTrackDataType = DatabaseInterface::ListTrackDataType;

    explicit ModelDataLoader(QObject *parent = nullptr);

    void setDatabase(DatabaseInterface *database);

Q_SIGNALS:

    void allAlbumsData(const ListAlbumDataType &allData);

    void allArtistsData(const ListArtistDataType &allData);

    void allGenresData(const ListGenreDataType &allData);

    void allTracksData(const ListTrackDataType &allData);

public Q_SLOTS:

    void loadData(ElisaUtils::PlayListEntryType dataType);

private:

    DatabaseInterface *mDatabase = nullptr;

};

#endif // MODELDATALOADER_H
