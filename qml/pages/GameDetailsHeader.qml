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

/*
 * TODO:
 * * Can we use the same item as the delegate in the overview?
 * * Make sure that we remove explicit dependencies to the details page.
 */

import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    width: parent.width
    height: Theme.itemSizeExtraLarge
    //contentHeight: Theme.itemSizeExtraLarge
    //z: 1

    // Label containing the total score
    Label {
        id: detailsTotalScore
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: detailsPeriodsScore.top
        }
        font {
            family: Theme.fontFamilyHeading
            pixelSize: Theme.fontSizeMedium
        }
        color: Theme.highlightColor

        // Automagically updated because of Q_PROPERTY has NOTIFY set
        text: gameDetailsData.totalScore
    }

    // Label containing the per-period score
    Label {
        id: detailsPeriodsScore
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: detailsHomeLogo.verticalCenter
        }
        font {
            pixelSize: Theme.fontSizeMedium
        }
        color: Theme.secondaryColor

        // Automagically updated because of Q_PROPERTY has NOTIFY set
        text: "(" + gameDetailsData.periodsScore + ")"
    }

    // Label indicating the status of the game
/*        Label {
        id: gameStatus
        font {
            pixelSize: Theme.fontSizeMedium
        }
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: periodsScore.bottom
        }
        color: Theme.secondaryColor
        text: gamestatus
    }*/

    // Hometeam logo: to the left
    Image {
        id: detailsHomeLogo

        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: Theme.horizontalPageMargin
        }

        height: 96
        width: 96
        fillMode: Image.PreserveAspectFit
        source: "../icons/" + gameDetailsData.hometeamId + ".png"
        //onSourceChanged: Utils.checkIcon(detailsHomeLogo, gameDetailsData.hometeamId)
        //onStatusChanged: Utils.checkIcon(detailsHomeLogo, gameDetailsData.hometeamId)
    }

    // Awayteam logo: to the right
    Image {
        id: detailsAwayLogo

        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
            rightMargin: Theme.horizontalPageMargin
        }

        height: 96
        width: 96
        fillMode: Image.PreserveAspectFit
        source: "../icons/" + gameDetailsData.awayteamId + ".png"
        //onSourceChanged: Utils.checkIcon(detailsAwayLogo, gameDetailsData.awayteamId)
        //onStatusChanged: Utils.checkIcon(detailsAwayLogo, gameDetailsData.awayteamId)
    }
}
