/*
 * Copyright 2014-present Roland Hostettler
 *
 * This file is part of swisshockey.
 *
 * swisshockey is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * swisshockey is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * swisshockey. If not, see http://www.gnu.org/licenses/.
 */

import QtQuick 2.0
import Sailfish.Silica 1.0

import "utils.js" as Utils

Page {
    property bool hometeam: true

    SilicaListView {
        id: hometeamRosterView
        visible: hometeam
        width: parent.width
        anchors.fill: parent
        model: hometeamRoster
        header: GameDetailsHeader {}
        delegate: RosterDelegate {}
        //section: bla bla
        //sectionDelegate: bla bla
        VerticalScrollDecorator {}
    }

    SilicaListView {
        id: awayteamRosterView
        visible: !hometeam
        width: parent.width
        anchors.fill: parent
        model: awayteamRoster
        header: GameDetailsHeader {}
        delegate: RosterDelegate {}
        //section: bla bla
        //sectionDelegate: bla bla
        VerticalScrollDecorator {}
    }
}
