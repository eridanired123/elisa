/*
 * Copyright 2016-2018 Matthieu Gallien <matthieu_gallien@yahoo.fr>
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

import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QtQuick.Window 2.2
import org.kde.elisa 1.0
import org.kde.kirigami 2.8 as Kirigami

RowLayout {
    id: contentViewContainer
    spacing: 0

    property bool showPlaylist
    property bool showExpandedFilterView
    property alias currentViewIndex: listViews.currentIndex

    function goBack() {
        viewManager.goBack()
    }

    function openArtist(name) {
        viewManager.openChildView(name, '', elisaTheme.artistIcon, 0, ElisaUtils.Artist, ViewManager.NoDiscHeaders)
    }

    function openAlbum(album, artist, image, albumID, showDiscHeader) {
        image = !image ? elisaTheme.defaultAlbumImage : image;
        viewManager.openChildView(album, artist, image, albumID, ElisaUtils.Album, showDiscHeader);
    }

    function openNowPlaying() {
        viewManager.closeAllViews();
    }

    ViewManager {
        id: viewManager

        onSwitchOffAllViews: {
            listViews.setCurrentIndex(pageModel.indexFromViewType(viewType))

            while(browseStackView.depth > 1) {
                browseStackView.pop()
            }
        }

        onOpenGridView: {
            if (expectedDepth === 1) {
                listViews.setCurrentIndex(pageModel.indexFromViewType(viewType))
            }

            while(browseStackView.depth > expectedDepth) {
                browseStackView.pop()
            }

            browseStackView.push(dataGridView, {
                                     viewType: viewType,
                                     filterType: filterType,
                                     mainTitle: pageModel.viewMainTitle(viewType, mainTitle),
                                     secondaryTitle: secondaryTitle,
                                     image: pageModel.viewImageUrl(viewType, imageUrl),
                                     modelType: dataType,
                                     defaultIcon: viewDefaultIcon,
                                     showRating: viewShowRating,
                                     delegateDisplaySecondaryText: viewDelegateDisplaySecondaryText,
                                     genreFilterText: genreNameFilter,
                                     artistFilter: artistNameFilter,
                                     isSubPage: (browseStackView.depth >= 2),
                                     stackView: browseStackView,
                                     opacity: 0,
                                 })
        }

        onOpenListView: {
            listViews.setCurrentIndex(pageModel.indexFromViewType(viewType))

            while(browseStackView.depth > expectedDepth) {
                browseStackView.pop()
            }

            browseStackView.push(dataListView, {
                                     viewType: viewType,
                                     filterType: filterType,
                                     isSubPage: expectedDepth > 1,
                                     mainTitle: mainTitle,
                                     secondaryTitle: secondaryTitle,
                                     databaseId: databaseId,
                                     image: imageUrl,
                                     modelType: dataType,
                                     sortRole: sortRole,
                                     sortAscending: sortOrder,
                                     stackView: browseStackView,
                                     displaySingleAlbum: displaySingleAlbum,
                                     showSection: showDiscHeaders,
                                     opacity: 0,
                                     radioCase: radioCase
                                 })
        }

        onSwitchFilesBrowserView: {
            listViews.setCurrentIndex(pageModel.indexFromViewType(viewType))

            while(browseStackView.depth > expectedDepth) {
                browseStackView.pop()
            }

            browseStackView.push(filesBrowserView, {
                                     viewType: viewType,
                                     mainTitle: mainTitle,
                                     image: imageUrl,
                                     opacity: 0,
                                 })
        }

        onSwitchContextView: {
            listViews.setCurrentIndex(pageModel.indexFromViewType(viewType))

            while(browseStackView.depth > expectedDepth) {
                browseStackView.pop()
            }

            browseStackView.push(albumContext, {
                                     viewType: viewType,
                                     mainTitle: mainTitle,
                                     image: imageUrl,
                                     opacity: 0,
                                 })
        }

        onPopOneView: {
            if (browseStackView.depth > 2) {
                browseStackView.pop() }
        }
    }

    ViewsModel {
        id: pageModel
    }

    ViewSelector {
        id: listViews

        model: pageModel

        Layout.fillHeight: true

        Behavior on Layout.maximumWidth {
            NumberAnimation {
                duration: 150
            }
        }

        onSwitchView: viewManager.openParentView(viewType, pageModel.viewMainTitle(viewType, ""), pageModel.viewImageUrl(viewType, ""))
    }

    Kirigami.Separator {
        id: viewSelectorSeparatorItem
        Layout.fillHeight: true
    }

    FocusScope {
        id: mainContentView

        focus: true

        Layout.fillHeight: true
        Layout.fillWidth: true

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.BackButton
            onClicked: goBack()
        }

        Rectangle {
            radius: 3
            color: myPalette.base

            anchors.fill: parent

            StackView {
                id: browseStackView

                anchors.fill: parent

                clip: true

                initialItem: Item {
                }

                popEnter: Transition {
                    OpacityAnimator {
                        from: 0.0
                        to: 1.0
                        duration: 300
                    }
                }

                popExit: Transition {
                    OpacityAnimator {
                        from: 1.0
                        to: 0.0
                        duration: 300
                    }
                }

                pushEnter: Transition {
                    OpacityAnimator {
                        from: 0.0
                        to: 1.0
                        duration: 300
                    }
                }

                pushExit: Transition {
                    OpacityAnimator {
                        from: 1.0
                        to: 0.0
                        duration: 300
                    }
                }

                replaceEnter: Transition {
                    OpacityAnimator {
                        from: 0.0
                        to: 1.0
                        duration: 300
                    }
                }

                replaceExit: Transition {
                    OpacityAnimator {
                        from: 1.0
                        to: 0.0
                        duration: 300
                    }
                }
            }
        }
    }

    Kirigami.Separator {
        id: playListSeparatorItem
        Layout.fillHeight: true
    }

    MediaPlayListView {
        id: playList

        Layout.fillHeight: true

        onStartPlayback: elisa.audioControl.ensurePlay()
        onPausePlayback: elisa.audioControl.playPause()
    }

    states: [
        State {
            name: "browsingViewsNoPlaylist"
            when: contentViewContainer.showPlaylist === false
            PropertyChanges {
                target: playList
                Layout.minimumWidth: 0
                Layout.maximumWidth: 0
                Layout.preferredWidth: 0
            }
            PropertyChanges {
                target: playListSeparatorItem
                visible: false
            }
        },
        State {
            name: 'browsingViews'
            when: contentViewContainer.showPlaylist === true
            PropertyChanges {
                target: playList
                Layout.minimumWidth: contentViewContainer.width * 0.28
                Layout.maximumWidth: contentViewContainer.width * 0.28
                Layout.preferredWidth: contentViewContainer.width * 0.28
            }
            PropertyChanges {
                target: playListSeparatorItem
                visible: true
            }
        }
    ]
    transitions: Transition {
        NumberAnimation {
            properties: "Layout.minimumWidth, Layout.maximumWidth, Layout.preferredWidth, opacity"
            easing.type: Easing.InOutQuad
            duration: 300
        }
    }

    Component {
        id: dataGridView

        DataGridView {
            StackView.onActivated: viewManager.viewIsLoaded(viewType)
            expandedFilterView: showExpandedFilterView
        }
    }

    Component {
        id: dataListView

        DataListView {
            StackView.onActivated: viewManager.viewIsLoaded(viewType)
            expandedFilterView: showExpandedFilterView
        }
    }

    Component {
        id: filesBrowserView

        FileBrowserView {
            StackView.onActivated: viewManager.viewIsLoaded(viewType)
            expandedFilterView: showExpandedFilterView
        }
    }

    Component {
            id: albumContext

        ContextView {
            StackView.onActivated: viewManager.viewIsLoaded(viewType)

            databaseId: elisa.manageHeaderBar.databaseId
            trackType: elisa.manageHeaderBar.trackType
            title: elisa.manageHeaderBar.title
            artistName: elisa.manageHeaderBar.artist
            albumName: elisa.manageHeaderBar.album
            albumArtUrl: elisa.manageHeaderBar.image
            fileUrl: elisa.manageHeaderBar.fileUrl
        }
    }
}
