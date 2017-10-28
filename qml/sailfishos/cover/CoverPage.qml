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

import "../pages/utils.js" as Utils

CoverBackground {
    id: cover

    Image {
        id: hometeamLogo
        anchors {
            right: parent.horizontalCenter
            rightMargin: 1.5*Theme.horizontalPageMargin
            bottom: parent.verticalCenter
        }
        height: Theme.fontSizeExtraLarge
        width: Theme.fontSizeExtraLarge
        fillMode: Image.PreserveAspectFit
        source: "../../icons/" + gameDetailsData.hometeamId + ".png"
        onStatusChanged: Utils.checkIcon(hometeamLogo, gameDetailsData.hometeamId)
    }

    Label {
        id: vsLabel
        anchors {
            verticalCenter: hometeamLogo.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }
        font.pixelSize: Theme.fontSizeMedium
        color: Theme.primaryColor
        text: typeof(gameDetailsData) != "undefined" ? "vs" : qsTr("No game<br />selected.")
    }

    Image{
        id: awayteamLogo
        anchors{
            left: parent.horizontalCenter
            leftMargin: 1.5*Theme.horizontalPageMargin
            bottom: parent.verticalCenter
        }
        height: Theme.fontSizeLarge
        width: Theme.fontSizeLarge
        fillMode: Image.PreserveAspectFit
        source: "../../icons/" + gameDetailsData.awayteamId + ".png"
        onStatusChanged: Utils.checkIcon(awayteamLogo, gameDetailsData.awayteamId)
    }

    Label {
        anchors {
            top: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }
        font.pixelSize: Theme.fontSizeLarge
        color: Theme.primaryColor
        text: gameDetailsData.totalScore
    }
}


