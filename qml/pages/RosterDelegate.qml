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

ListItem {
    width: parent.width
    //height: Theme.itemSizeLarge
    height: Theme.itemSizeSmall

    // Jersey number
    Label {
        id: jerseyNumber
        width: 96
        //x: Theme.horizontalPageMargin
        anchors {
            left: parent.left
            bottom: parent.verticalCenter
            leftMargin: Theme.horizontalPageMargin
        }
        font {
            family: Theme.fontFamilyHeading
            pixelSize: Theme.fontSizeMedium
        }
        color: Theme.highlightColor
        text: model.player.jerseyNumber
    }

    // Name
    Label {
        id: player
        anchors {
            left: jerseyNumber.right
            bottom: parent.verticalCenter
        }
        font {
            family: Theme.fontFamilyHeading
            pixelSize: Theme.fontSizeMedium
        }
        color: Theme.highlightColor
        text: model.player.name
    }

    // Stats
    /*
    Label {
        id: stats
        anchors {
            left: jerseyNumber.right
            top: parent.verticalCenter
        }
        font {
            pixelSize: Theme.fontSizeMedium
        }
        color: Theme.secondaryColor
        text: model.player.stats
    }
    */

    // Position
    Label {
        id: position
        horizontalAlignment: Text.Right
        anchors {
            right: parent.right
            rightMargin: Theme.horizontalPageMargin
            bottom: parent.verticalCenter
            //verticalCenter: parent.verticalCenter
        }
        font {
            family: Theme.fontFamilyHeading
            pixelSize: Theme.fontSizeMedium
        }
        color: Theme.highlightColor
        text: model.player.position
    }
}
