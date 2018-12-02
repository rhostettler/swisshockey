/*
 * Copyright 2014-2017 Roland Hostettler
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

// The selection dialog for choosing the filter
Page {
    SilicaListView{
        id: leagueListView
        anchors.fill: parent
        model: leagueList

        header: PageHeader {
            title: qsTr("Select League")
        }

        delegate: ListItem {
            width: parent.width
            id: leagueListDelegate
            //contentHeight: Theme.itemSizeSmall

            Label {
                x: Theme.horizontalPageMargin
                text: name
                anchors.verticalCenter: parent.verticalCenter
                color: leagueListDelegate.highlighted ? Theme.highlightColor : Theme.primaryColor
            }
            onClicked: {
                overviewPage.leagueName = name;
                appWindow.leagueChanged(id);
                pageStack.pop();
            }
        }
    }
}
